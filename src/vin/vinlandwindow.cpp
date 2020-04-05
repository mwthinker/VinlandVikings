#include "vinlandwindow.h"
#include "logger.h"
#include "imguiextra.h"

#include <filesystem>
#include <regex>

namespace fs = std::filesystem;

namespace vin {

	namespace {

		const ImGuiWindowFlags ImGuiNoWindow = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove;

		void HelpMarker(const std::string& text) {
			if (ImGui::IsItemHovered()) {
				ImGui::Tooltip([&]() {
					ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
					ImGui::TextUnformatted(text.c_str());
					ImGui::PopTextWrapPos();
				});
			}
		}

		bool isFullWater(const hex::HexSides& hexSides) {
			for (const auto& side : hexSides) {
				if (side != hex::HexSide::WATER) {
					return false;
				}
			}
			return true;
		}

		std::vector<std::string> listFiles(const std::string& regexStr) {
			std::vector<std::string> files;
			try {
				std::regex jsonRegex{regexStr};
				for (auto p : fs::recursive_directory_iterator("images")) {
					auto pathStr = p.path().string();
					if (std::regex_match(pathStr, jsonRegex)) {
						std::replace(pathStr.begin(), pathStr.end(), '\\', '/');
						files.push_back(pathStr);
					}
				}
			} catch (std::exception& e) {
				logger()->warn(e.what());
			}
			return files;
		}

		std::vector<std::string> listJsonFiles() {
			return listFiles(".*.([j][s][o][n])");
		}

		std::vector<std::string> listImageFiles() {
			return listFiles(".*.([Pp][Nn][Gg]|[Jj][Pp][Ee]?[Gg]|[Bb][Mm][Pp])");
		}

		const std::string ICONFILE = "icon.ico";

	}

	VinlandWindow::VinlandWindow()
		: hexCanvas_{getShader()} {
		
		setIcon(ICONFILE);
		
		zoomIn_ = actionManager_.add(Action{SDLK_KP_PLUS, "Zoom in", [&]() {
			hexCanvas_.zoomIn();
		}});
		zoomOut_ = actionManager_.add(Action{SDLK_KP_MINUS, "Zoom out", [&]() {
			hexCanvas_.zoomOut();
		}});
		undo_ = actionManager_.add(Action{SDLK_z, SDLK_LCTRL, "Undo", [&]() {
			logger()->info("[VinlandWindow] Undo");
			hexCanvas_.undo();
		}});
		redo_ = actionManager_.add(Action{SDLK_y, SDLK_LCTRL, "Redo", [&]() {
			logger()->info("[VinlandWindow] Redo");
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
				hexImage_ = HexData::getInstance().loadSprite(dropFile);
				logger()->warn(dropFile);
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
				for (auto str : jsonFiles_) {
					std::cout << str << "\n";
				}
			}

			if (ImGui::MenuItem("New map")) {}
			
			if (ImGui::MenuItem("Add hex image")) {
				popup = true;
			}

			ImGui::Menu("Open", [&]() {
				for (const auto& file : jsonFiles_) {
					bool selectedFile = (file == HexData::getInstance().getLoadedFilename());
					if (ImGui::MenuItem(file.c_str(), nullptr, selectedFile)) {
						if (!selectedFile) {
							logger()->info("[VinlandWindow] {}", file);
							HexData::getInstance().load(file);
							initData();
						}
					}
				}
			});
			
			if (ImGui::MenuItem("Quit", "Alt+F4")) {
				quit();
			}
		});

		if (popup) {
			ImGui::OpenPopup("Deleteee");
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

	void VinlandWindow::showMenuBar() {
		ImGui::MenuBar([&]() {
			addFileInMenuBar();
			addEditInMenuBar();
			addViewInMenuBar();
			//ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(50.f, 50.f));
			//ImGui::PopStyleVar();
			showAddHexImagePopup();
		});
	}

	void VinlandWindow::showAddHexImagePopup() {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{10, 10});
		ImGui::PopupModal("Deleteee", nullptr, ImGuiWindowFlags_AlwaysAutoResize, [&]() {
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

	void VinlandWindow::imGuiPreUpdate(const std::chrono::high_resolution_clock::duration& deltaTime) {
		hexCanvas_.drawCanvas(deltaTime);
	}

	void VinlandWindow::imGuiUpdate(const std::chrono::high_resolution_clock::duration& deltaTime) {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0});
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, {1.f, 1.f, 1.f, 0.f});

		ImGui::SetNextWindowPos({0.f, 0.f});
		auto [width, height] = sdl::Window::getSize();
		ImGui::SetNextWindowSize({(float) width, 300.f});

		ImGui::Window("Main", nullptr, ImGuiNoWindow, [&]() {
			showMenuBar();

			ImGui::Text("KEYS:");
			ImGui::Text("ARROWS - Move window");
			ImGui::Text("PAGE_DOWN/UP - Tilt camera");
			ImGui::NewLine();

			ImGui::Text("MOUSE:");
			ImGui::Text("Drag MIDDLE - Move window");
			ImGui::Text("Push MIDDLE - Rotate card");
			ImGui::Text("Left - Place card");
			
			drawHexTypesButtons();
		});

		if (auto canvasHeight = static_cast<float>(height) - 300.f;
			canvasHeight > 0) {
			
			hexCanvas_.updateCanvasSize({0.f, 0.f}, {width, canvasHeight});
		}
		
		ImGui::PopStyleColor();
		ImGui::PopStyleVar(3);
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
				hexCanvas_.activateHexagon(hexImages[index]);
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
	}
	
	void VinlandWindow::initData() {
		tileLexicon_.clear();
		hexTypes_.clear();
		hexCanvas_.clear();

		auto [w, h] = sdl::ImGuiWindow::getSize();
		glViewport(0, 0, w, h);

		auto hexImages = HexData::getInstance().loadHexImages();
		tileLexicon_.add(hexImages);
		hexCanvas_.setTileLexicon(tileLexicon_);

		for (const auto& image : hexImages) {
			hexTypes_[image.getHexSides()].hexImages_.push_back(image);
		}

		int size = static_cast<int>(hexImages.size());
		int nbrOfEach = static_cast<int>(100.0 / size);
		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < nbrOfEach; ++j) {
				hexImages.push_back(hexImages[i]);
			}
		}

		hexCanvas_.setDeck(hexImages);
	}

} // Namespace vin.
