#include "vinlandwindow.h"
#include "canvas.h"
#include "logger.h"
#include "imguiextra.h"

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

	}

	VinlandWindow::VinlandWindow() {
	}

	VinlandWindow::~VinlandWindow() {
	}

	void VinlandWindow::eventUpdate(const SDL_Event& windowEvent) {
		sdl::ImGuiWindow::eventUpdate(windowEvent);
		hexWorldCanvas.eventUpdate(windowEvent);

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

	void VinlandWindow::imGuiPreUpdate(double deltaTime) {
		hexWorldCanvas.drawCanvas(deltaTime);
	}

	void VinlandWindow::showExampleMenuFile()
	{
		ImGui::MenuItem("(dummy menu)", NULL, false, false);
		if (ImGui::MenuItem("New")) {}
		if (ImGui::MenuItem("Open", "Ctrl+O")) {}
		if (ImGui::BeginMenu("Open Recent"))
		{
			ImGui::MenuItem("fish_hat.c");
			ImGui::MenuItem("fish_hat.inl");
			ImGui::MenuItem("fish_hat.h");
			if (ImGui::BeginMenu("More.."))
			{
				ImGui::MenuItem("Hello");
				ImGui::MenuItem("Sailor");
				if (ImGui::BeginMenu("Recurse.."))
				{
					showExampleMenuFile();
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Save", "Ctrl+S")) {}
		if (ImGui::MenuItem("Save As..")) {}
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
		if (ImGui::BeginMenu("Disabled", false)) // Disabled
		{
			IM_ASSERT(0);
		}
		if (ImGui::MenuItem("Checked", NULL, true)) {}
		if (ImGui::MenuItem("Quit", "Alt+F4")) {
			quit();
		}
	}

    void VinlandWindow::imGuiUpdate(double deltaTime) {
		beginMain();

		ImGui::BeginMenuBar();
		static bool menu = true;
		
		if (ImGui::BeginMenu("Menu", &menu))
		{
			showExampleMenuFile();
			ImGui::EndMenu();
		}		
		ImGui::EndMenuBar();

		ImGui::Text("KEYS:");
		ImGui::Text("ARROWS - Move window");
		ImGui::Text("PAGE_DOWN/UP - Tilt camera");
		ImGui::Text("C - Clear tiles");
		ImGui::Text("G - Grid on/off");
		ImGui::Text("x - View XY Coords");
		ImGui::Text("h - View Hex Coords");
		ImGui::NewLine();

		ImGui::Text("MOUSE:");
		ImGui::Text("Drag MIDDLE - Move window");
		ImGui::Text("Left/right click - Replace with Next/Previous tile");
		drawHexTypesButtons();

		hexWorldCanvas.drawImgui();
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
				if (hexImages[index].getImage() == hexWorldCanvas.currentHexSprite()) {
					index = (index + 1) % static_cast<int>(hexImages.size());
				}
				hexWorldCanvas.activateHexagon(hexImages[index]);
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
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        sdl::ImGuiWindow::initOpenGl();
	}

	void VinlandWindow::initPreLoop() {
        sdl::ImGuiWindow::initPreLoop();
		auto [w, h] = sdl::ImGuiWindow::getSize();
        glViewport(0, 0, w, h);

		auto hexImages = HexData::getInstance().loadHexImages();
		for (const auto& image : hexImages) {
			hexTypes_[image.getHexSides()].hexImages_.push_back(image);
			hexTypes_[image.getHexSides()].hexImages_.push_back(image);
			hexTypes_[image.getHexSides()].hexImages_.push_back(image);
			hexTypes_[image.getHexSides()].hexImages_.push_back(image);
			hexTypes_[image.getHexSides()].hexImages_.push_back(image);
		}

		HexImage waterImage{};
		for (const auto& hexImage : hexImages) {
			hexTypes_[hexImage.getHexSides()].hexImages_.push_back(hexImage);
			if (isFullWater(hexImage.getHexSides())) {
				waterImage = hexImage;
			}
		}
		hexWorldCanvas.setDefaultHexSprite(waterImage);
		hexWorldCanvas.setHexImagesMap(hexTypes_);

		//for (const auto& image : hexTypes_) {}

		hexWorldCanvas.setDeck(hexImages);
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
