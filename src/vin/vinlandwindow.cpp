#include "vinlandwindow.h"
#include "logger.h"
#include "imguiextra.h"
#include "hex/tileboard.h"

#include <filesystem>
#include <regex>
#include <fstream>

namespace fs = std::filesystem;

using nlohmann::json;

namespace vin::hex {

	NLOHMANN_JSON_SERIALIZE_ENUM(hex::HexSide, {
		{hex::HexSide::Forest, "WATER"},
		{hex::HexSide::Grass, "GRASS"},
		{hex::HexSide::Mountain, "MOUNTAIN"},
		{hex::HexSide::Water, "WATER"},
		{hex::HexSide::None, "NONE"}
	})

	void to_json(json& j, const Hexi& hex) {
		j = json{{"q", hex.q()}, {"r", hex.r()}};
	}

	void from_json(const json& json, Hexi& hex) {
		hex = Hexi{json["q"].get<Hexi::value_type>(), json["r"].get<Hexi::value_type>()};
	}

	void to_json(json& j, const Tile& tile) {
		for (const auto& side : tile) {
			j.push_back(side);
		}
	}

	void from_json(const json& json, Tile& tile) {
		for (int i = 0; i < 6; ++i) {
			tile[i] = json[i].get<HexSide>();
		}
	}

	void to_json(json& json, const TileBoard& tileBoard) {
		for (const auto& [hex, tile] : tileBoard) {
			json.push_back({{"hex", hex}, {"tile", tile}});
		}
	}

	void from_json(const json& json, TileBoard& tileBoard) {
		for (const auto& tileHex : json) {
			tileBoard.put(tileHex["hex"].get<hex::Hexi>(), tileHex["tile"].get<hex::Tile>());
		}
	}

}

namespace vin {

	namespace {

		const ImGuiWindowFlags ImGuiNoWindow = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove;

		constexpr std::string_view IMAGE_TILES_FOLDER = "imageTiles";

		void HelpMarker(const std::string& text) {
			if (ImGui::IsItemHovered()) {
				ImGui::Tooltip([&]() {
					ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
					ImGui::TextUnformatted(text.c_str());
					ImGui::PopTextWrapPos();
				});
			}
		}

		bool isFullWater(const hex::Tile& tile) {
			for (const auto& side : tile) {
				if (side != hex::HexSide::Water) {
					return false;
				}
			}
			return true;
		}

		std::vector<std::string> listFiles(const std::string& regexStr) {
			std::vector<std::string> files;
			try {
				std::regex jsonRegex{regexStr};
				for (auto p : fs::recursive_directory_iterator(IMAGE_TILES_FOLDER)) {
					auto pathStr = p.path().string();
					if (std::regex_match(pathStr, jsonRegex)) {
						std::replace(pathStr.begin(), pathStr.end(), '\\', '/');
						files.push_back(pathStr);
					}
				}
			} catch (std::exception& e) {
				spdlog::warn(e.what());
			}
			return files;
		}

		std::vector<std::string> listJsonFiles() {
			return listFiles(".*.([j][s][o][n])");
		}

		std::vector<std::string> listImageFiles() {
			return listFiles(".*.([Pp][Nn][Gg]|[Jj][Pp][Ee]?[Gg]|[Bb][Mm][Pp])");
		}

		const std::string IconFile = "icon.ico";

		constexpr const char* POPUP_ADDHEXIMAGE = "Add Hex Image";
		constexpr const char* POPUP_HELP = "Help";
		constexpr const char* POPUP_ABOUT = "About";

		void save(const std::string& file, const HexCanvas& hexCanvas) {
			spdlog::info("[HexData] Current working directory {}", std::filesystem::current_path().string());


			/*if (std::filesystem::exists("USE_APPLICATION_JSON")) {
				jsonPath_ = jsonFile;
			} else {
				// Find default path to save/load file from.
				jsonPath_ = SDL_GetPrefPath("mwthinker", "VinlandVikings") + jsonFile;
			}*/

			const auto& snapshot = hexCanvas.getSnapshot();

			json json;
			json["tileboard"] = snapshot.tileBoard;

			std::ofstream output{file};
			output << json.dump(4);
		}

		hex::TileBoard load(const std::string& file) {
			json json;
			std::ifstream input{file};
			input >> json;
			return json.get<hex::TileBoard>();
		}

	}

	VinlandWindow::VinlandWindow() {
		setIcon(IconFile);
		
		load_ = actionManager_.add(Action{SDLK_o, SDLK_LCTRL, "Load", [&]() {
			spdlog::info("[VinlandWindow] Load");

			auto tileBoard = load("TestBoardFile.txt");
			TilesGraphic::Map tileMap;

			auto deck = hexCanvas_.getDeck();

			for (const auto& [hex, tile] : tileBoard) {
				tileLexicon_.getTiles(hex::TileKey{tile});
			}
		}});
		save_ = actionManager_.add(Action{SDLK_s, SDLK_LCTRL, "Save", [&]() {
			spdlog::info("[VinlandWindow] Save");
			save("TestBoardFile.txt", hexCanvas_);
		}});
		saveAs_ = actionManager_.add(Action{"Save as", [&]() {
			spdlog::info("[VinlandWindow] Save as");
		}});
		zoomIn_ = actionManager_.add(Action{SDLK_KP_PLUS, "Zoom in", [&]() {
			hexCanvas_.zoomIn();
		}});
		zoomOut_ = actionManager_.add(Action{SDLK_KP_MINUS, "Zoom out", [&]() {
			hexCanvas_.zoomOut();
		}});
		undo_ = actionManager_.add(Action{SDLK_z, SDLK_LCTRL, "Undo", [&]() {
			spdlog::info("[VinlandWindow] Undo");
			hexCanvas_.undo();
		}});
		redo_ = actionManager_.add(Action{SDLK_y, SDLK_LCTRL, "Redo", [&]() {
			spdlog::info("[VinlandWindow] Redo");
			hexCanvas_.redo();
		}});
		gridAction_ = actionManager_.add(Action{SDLK_g, "Grid", [&]() {
			hexCanvas_.setGrid(!hexCanvas_.isGrid());
		}});
		clearAction_ = actionManager_.add(Action{SDLK_c, "Clear tiles", [&]() {
			hexCanvas_.clear();
		}});
		xyCoordsAction_ = actionManager_.add(Action{SDLK_x, "XY", [&]() {
			hexCanvas_.setXYCoords(!hexCanvas_.isXYCoords());
		}});
		hexCoordsAction_ = actionManager_.add(Action{SDLK_h, "HEX", [&]() {
			hexCanvas_.setHexCoords(!hexCanvas_.isHexCoords());
		}});
		generateMapAction_ = actionManager_.add(Action{SDLK_r, "Generate map", [&]() {
			hexCanvas_.clearAndGenerateMap();
		}});
	}

	VinlandWindow::~VinlandWindow() {
	}

	void VinlandWindow::eventUpdate(const SDL_Event& windowEvent) {
		sdl::ImGuiWindow::eventUpdate(windowEvent);

		auto& io = ImGui::GetIO();

		switch (windowEvent.type) {
			case SDL_WINDOWEVENT:
				switch (windowEvent.window.event) {
					case SDL_WINDOWEVENT_LEAVE:
						break;
					case SDL_WINDOWEVENT_CLOSE:
						sdl::Window::quit();
				}
				break;
			case SDL_DROPFILE: { // In case if dropped file
				std::string dropFile{windowEvent.drop.file};
				hexImage_ = Configuration::getInstance().loadSprite(dropFile);
				spdlog::warn(dropFile);
				//SDL_free(windowEvent.drop.file).
				break;
			}
			case SDL_MOUSEBUTTONUP: [[fallthrough]];
			case SDL_MOUSEBUTTONDOWN: [[fallthrough]];
			case SDL_MOUSEMOTION: [[fallthrough]];
			case SDL_MOUSEWHEEL:
				if (!io.WantCaptureMouse) {
					hexCanvas_.eventUpdate(windowEvent);
				}
				break;
			case SDL_KEYUP:
				if (!io.WantCaptureKeyboard) {
					hexCanvas_.eventUpdate(windowEvent);
				}
				break;
			case SDL_KEYDOWN:
				if (!io.WantCaptureKeyboard) {
					hexCanvas_.eventUpdate(windowEvent);
				}
				actionManager_.update(windowEvent.key.keysym.sym);
				switch (windowEvent.key.keysym.sym) {
					case SDLK_ESCAPE:
						sdl::Window::quit();
						break;
				}
				break;
			case SDL_QUIT:
				sdl::Window::quit();
				break;
		}
	}

	void VinlandWindow::addFileInMenuBar() {
		bool popup = false;
		ImGui::Menu("File", [&]() {
			if (static bool firstTime = true; firstTime) {
				firstTime = false;
				jsonFiles_ = listJsonFiles();
				for (const auto& str : jsonFiles_) {
					spdlog::info("[VinlandWindow] Found json file: {}", str);
				}
			}

			if (ImGui::MenuItem("New map")) {}
			
			if (ImGui::MenuItem("Add hex image")) {
				popup = true;
			}

			ImGui::Menu("Open", [&]() {
				for (const auto& file : jsonFiles_) {
					bool selectedFile = (file == Configuration::getInstance().getLoadedFilename());
					if (ImGui::MenuItem(file.c_str(), nullptr, selectedFile)) {
						if (!selectedFile) {
							spdlog::info("[VinlandWindow] {}", file);
							Configuration::getInstance().load(file);
							initData();
						}
					}
				}
			});

			ImGui::MenuItem(load_);
			ImGui::MenuItem(save_);
			ImGui::MenuItem(saveAs_);
			
			if (ImGui::MenuItem("Quit", "Alt+F4")) {
				quit();
			}
		});

		if (popup) {
			ImGui::OpenPopup(POPUP_ADDHEXIMAGE);
		}
	}

	void VinlandWindow::addEditInMenuBar() {
		ImGui::Menu("Edit", true, [&]() {
			ImGui::MenuItem(undo_);
			ImGui::MenuItem(redo_);
			ImGui::MenuItem(clearAction_);
			ImGui::MenuItem(generateMapAction_);
			ImGui::Separator();
		});
	}

	void VinlandWindow::addViewInMenuBar() {
		ImGui::Menu("View", true, [&]() {
			ImGui::Menu("Zoom", [&]() {
				ImGui::MenuItem(zoomIn_);
				ImGui::MenuItem(zoomOut_);
			});
			ImGui::Separator();

			ImGui::Checkbox(gridAction_, hexCanvas_.isGrid());
			ImGui::Checkbox(xyCoordsAction_, hexCanvas_.isXYCoords());
			ImGui::Checkbox(hexCoordsAction_, hexCanvas_.isHexCoords());

			ImGui::Separator();
		});
		
	}

	void VinlandWindow::addHelpInMenuBar() {
		bool popupHelp = false;
		bool popupAbout = false;
		ImGui::Menu("Help ", true, [&]() {
			if (ImGui::MenuItem("Help  ")) {
				popupHelp = true;
			}
			if (ImGui::MenuItem("About VinlandVikings")) {
				popupAbout = true;
			}
		});

		if (popupHelp) {
			ImGui::OpenPopup(POPUP_HELP);
		}
		if (popupAbout) {
			ImGui::OpenPopup(POPUP_ABOUT);
		}
	}

	void VinlandWindow::showMenuBar() {
		ImGui::MenuBar([&]() {
			addFileInMenuBar();
			addEditInMenuBar();
			addViewInMenuBar();
			addHelpInMenuBar();
			//ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(50.f, 50.f));
			//ImGui::PopStyleVar();
			showAddHexImagePopup();
			showAboutPopup();
			showHelpPopup();
		});
	}

	void VinlandWindow::showAddHexImagePopup() {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{10, 10});
		ImGui::PopupModal(POPUP_ADDHEXIMAGE, nullptr, ImGuiWindowFlags_AlwaysAutoResize, [&]() {
			ImGui::Text("All those beautiful files will be deleted.\nThis operation cannot be undone!\n\n");
			ImGui::Separator();

			static bool flatHex = true;
			ImGui::Checkbox("Flat hex", &flatHex);
			static float slider = 100;
			ImGui::SliderFloat("Pixel size in %", &slider, 1, 100);

			sdl::TextureView view{hexImage_,
				(1 - slider / 100.f) / 2.f, (1 - slider / 100.f) / 2.f,
				slider / 100.f, slider / 100.f};
			ImGui::Separator();
			ImGui::Hexagon(view, 500.f, flatHex);
			ImGui::Separator();

			if (ImGui::Button("OK", {120, 0})) { ImGui::CloseCurrentPopup(); }
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", {120, 0})) { ImGui::CloseCurrentPopup(); }
		});
		ImGui::PopStyleVar();
	}

	void VinlandWindow::showHelpPopup() {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{10, 10});
		ImGui::PopupModal(POPUP_HELP, nullptr, ImGuiWindowFlags_AlwaysAutoResize, [&]() {
			ImGui::Text("KEYS:");
			ImGui::Text("ARROWS - Move window");
			ImGui::Text("PAGE_DOWN/UP - Tilt camera");
			ImGui::NewLine();

			ImGui::Text("MOUSE:");
			ImGui::Text("Drag MIDDLE - Move window");
			ImGui::Text("Push MIDDLE - Rotate card");
			ImGui::Text("Left - Place card");

			if (ImGui::Button("OK", {120, 0})) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::SetItemDefaultFocus();
		});
		ImGui::PopStyleVar();
	}
	
	void VinlandWindow::showAboutPopup() {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{10, 10});
		ImGui::PopupModal(POPUP_ABOUT, nullptr, ImGuiWindowFlags_AlwaysAutoResize, [&]() {
			ImGui::Text("Vinland Vikings");
			ImGui::NewLine();
						
			ImGui::Text("Version: v%s", PROJECT_VERSION);
			ImGui::Text("Git Hash: %s", GIT_VERSION);

			if (ImGui::Button("OK", {120, 0})) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::SetItemDefaultFocus();
		});
		ImGui::PopStyleVar();
	}


	void VinlandWindow::imGuiPreUpdate(const std::chrono::high_resolution_clock::duration& deltaTime) {
		hexCanvas_.drawCanvas(getShader(), deltaTime);
	}

	void VinlandWindow::imGuiUpdate(const std::chrono::high_resolution_clock::duration& deltaTime) {
		//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0});
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
		//ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
		//ImGui::PushStyleColor(ImGuiCol_WindowBg, {1.f, 1.f, 1.f, 0.f});

		ImGui::SetNextWindowPos({0.f, 0.f});
		auto [width, height] = sdl::Window::getSize();

		float hexWindowHeight = 200.f;
		ImGui::SetNextWindowSize({(float) width, hexWindowHeight});

		ImGui::Window("Main", nullptr, ImGuiNoWindow, [&]() {
			showMenuBar();
			
			drawHexTypesButtons();
		});

		if (auto canvasHeight = static_cast<float>(height) - hexWindowHeight;
			canvasHeight > 0) {
			
			hexCanvas_.updateCanvasSize({0.f, 0.f}, {width, canvasHeight});
		}
		
		//ImGui::PopStyleColor();
		ImGui::PopStyleVar();
	}

	void VinlandWindow::drawHexTypesButtons() {
		auto& style = ImGui::GetStyle();
		
		const int buttonsCount = static_cast<int>(hexTypes_.size());
		const float windowVisible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
		
		int n = 0;
		for (auto& [key, hexImageType] : hexTypes_) {
			auto& index = hexImageType.index_;
			auto& hexImages = hexImageType.hexImages_;
			const ImVec2 buttonSize{50.f, 50.f};
			if (ImGui::ImageButton(hexImages[index].getImage(), buttonSize)) {
				if (hexImages[index].getImage() == hexCanvas_.currentHexSprite()) {
					index = (index + 1) % static_cast<int>(hexImages.size());
				}
				hexCanvas_.activateHexagon(SpriteTile{hexImages[index]});
			}
			HelpMarker(hexImages[index].getFilename());
			const float lastButton_x2 = ImGui::GetItemRectMax().x;
			const float nextButton_x2 = lastButton_x2 + style.ItemSpacing.x + buttonSize.x; // Expected position if next button was on same line
			if (n + 1 < buttonsCount && nextButton_x2 < windowVisible_x2) {
				ImGui::SameLine();
			}
			++n;
		}
	}

	void VinlandWindow::initOpenGl() {
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        sdl::ImGuiWindow::initOpenGl();
	}

	void VinlandWindow::initPreLoop() {
		//glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);

        sdl::ImGuiWindow::initPreLoop();
#if IMGUI_DEMO_WINDOW
		setShowDemoWindow(true);
#endif
		initData();

		ImGui::GetIO().Fonts->AddFontFromFileTTF("fonts/Ubuntu-B.ttf", 16);
		//ImGui::GetIO().Fonts->AddFontFromFileTTF("fonts/Norse-Bold.otf", 16);
		ImGui::GetIO().FontDefault = nullptr;
	}
	
	void VinlandWindow::initData() {
		tileLexicon_.clear();
		hexTypes_.clear();
		hexCanvas_.clear();

		auto [w, h] = sdl::ImGuiWindow::getSize();
		glViewport(0, 0, w, h);

		auto hexImages = Configuration::getInstance().loadHexImages();
		tileLexicon_.add(hexImages);
		hexCanvas_.setTileLexicon(tileLexicon_);

		for (const auto& image : hexImages) {
			hexTypes_[hex::TileInvariantKey{image.getTile()}].hexImages_.push_back(image);
		}

		Deck deck;

		int size = static_cast<int>(hexImages.size());
		int nbrOfEach = static_cast<int>(100.0 / size);
		for (const auto hexImage : hexImages) {
			deck.push(hex::TileInvariantKey{hexImage.getTile()}, nbrOfEach);
		}

		hexCanvas_.setDeck(deck);
	}

}
