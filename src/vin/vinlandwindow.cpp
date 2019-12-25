#include "vinlandwindow.h"
#include "canvas.h"
#include "logger.h"
#include "imguiextra.h"

#include <filesystem>
#include <regex>

namespace fs = std::filesystem;

namespace vin {

	namespace {

		const ImGuiWindowFlags ImGuiNoWindow = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove;
		
		const ImGuiWindowFlags ImGuiNoWindow2 = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

		void HelpMarker(const std::string& text) {
			if (ImGui::IsItemHovered()) {
				ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::TextUnformatted(text.c_str());
				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
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

	}

	VinlandWindow::VinlandWindow() {
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
		hexCanvas_.eventUpdate(windowEvent);		

		switch (windowEvent.type) {
			case SDL_WINDOWEVENT:
				switch (windowEvent.window.event) {
					case SDL_WINDOWEVENT_LEAVE:
						break;
					case SDL_WINDOWEVENT_CLOSE:
						sdl::Window::quit();
				}
				break;
			case SDL_KEYDOWN:
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

	void VinlandWindow::imGuiPreUpdate(const std::chrono::high_resolution_clock::duration& deltaTime) {
		hexCanvas_.drawCanvas(deltaTime);
	}

	void VinlandWindow::addFileInMenuBar() {
		static bool menuActive = false;
		if (bool tmp = menuActive; menuActive = ImGui::BeginMenu("File", true))
		{
			if (menuActive != tmp) {
				jsonFiles_ = listJsonFiles();
				for (auto str : jsonFiles_) {
					std::cout << str << "\n";
				}
			}
		} else {
			return;
		}

		if (ImGui::MenuItem("New map")) {}
		if (ImGui::BeginMenu("Open"))
		{
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
			ImGui::EndMenu();
		}
		/*
		ImGui::Separator();
		if (ImGui::BeginMenu("Options"))
		{
			static bool enabled = true;
			ImGui::MenuItem("Enabled", "", &enabled);
			ImGui::BeginChild("child", ImVec2(0, 60), true);
			for (int i = 0; i < 10; i++)
				ImGui::Text("Scrolling Text %d", i);
			ImGui::EndChild();
			static float f = 0.5f;
			static int n = 0;
			static bool b = true;
			ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
			ImGui::InputFloat("Input", &f, 0.1f);
			ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
			ImGui::Checkbox("Check", &b);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Colors"))
		{
			float sz = ImGui::GetTextLineHeight();
			for (int i = 0; i < ImGuiCol_COUNT; i++)
			{
				const char* name = ImGui::GetStyleColorName((ImGuiCol) i);
				ImVec2 p = ImGui::GetCursorScreenPos();
				ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), ImGui::GetColorU32((ImGuiCol) i));
				ImGui::Dummy(ImVec2(sz, sz));
				ImGui::SameLine();
				ImGui::MenuItem(name);
			}
			ImGui::EndMenu();
		}
		*/
		if (ImGui::MenuItem("Quit", "Alt+F4")) {
			quit();
		}

		ImGui::EndMenu();
	}

	void VinlandWindow::addEditInMenuBar() {
		ImGui::PushItemWidth(600);

		if (!ImGui::BeginMenu("Edit", true)) {
			return;
		}
		if (ImGui::MenuItem("Undo")) {}
		if (ImGui::MenuItem("Redo")) {}
		ImGui::MenuItem(clearAction_);
		ImGui::MenuItem(generateMapAction_);
		ImGui::Separator();
		ImGui::EndMenu();
		ImGui::PopItemWidth();
	}

	void VinlandWindow::addViewInMenuBar() {
		if (!ImGui::BeginMenu("View", true)) {
			return;
		}

		if (ImGui::BeginMenu("Zoom")) {
			if (ImGui::MenuItem("Zoom in")) {
				hexCanvas_.zoomIn();
			}
			if (ImGui::MenuItem("Zoom out")) {
				hexCanvas_.zoomOut();
			}
			ImGui::EndMenu();
		}
		ImGui::Separator();
		
		
		ImGui::Checkbox(gridAction_, hexCanvas_.isGrid());	
		ImGui::Checkbox(xyCoordsAction_, hexCanvas_.isXYCoords());
		ImGui::Checkbox(hexCoordsAction_, hexCanvas_.isHexCoords());

		ImGui::Separator();
		ImGui::EndMenu();
		
	}

	void VinlandWindow::showMenuBar() {
		ImGui::BeginMenuBar();
		
		
		addFileInMenuBar();
		addEditInMenuBar();
		addViewInMenuBar();
		//ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(50.f, 50.f));
		//ImGui::PopStyleVar();

		ImGui::EndMenuBar();
	}

    void VinlandWindow::imGuiUpdate(const std::chrono::high_resolution_clock::duration& deltaTime) {
		beginMain();

		showMenuBar();

		ImGui::Text("KEYS:");
		ImGui::Text("ARROWS - Move window");
		ImGui::Text("PAGE_DOWN/UP - Tilt camera");
		ImGui::NewLine();

		ImGui::Text("MOUSE:");
		ImGui::Text("Drag MIDDLE - Move window");
		ImGui::Text("Push MIDDLE - Rotate card");
		ImGui::Text("Left - Place card");
		//ImGui::Text("Left/right click - Replace with Next/Previous tile");
		drawHexTypesButtons();

		hexCanvas_.drawImgui();
		endMain();
    }

	void VinlandWindow::drawHexTypesButtons() {
		ImGuiStyle& style = ImGui::GetStyle();
		
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
		setShowDemoWindow(false);
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

	void VinlandWindow::beginMain() {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, {1.f, 1.f, 1.f, 0.f});

		ImGui::SetNextWindowPos({0.f, 0.f});
		auto [width, height] = sdl::Window::getSize();
		ImGui::SetNextWindowSize({(float) width, (float) height});

		ImGui::Begin("Main", nullptr, ImGuiNoWindow);
	}

	void VinlandWindow::endMain() {
		ImGui::End();

		ImGui::PopStyleColor();
		ImGui::PopStyleVar(3);
	}

} // Namespace vin.
