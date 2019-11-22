#include "vinlandwindow.h"
#include "canvas.h"
#include "logger.h"
#include "imguiextra.h"

namespace vin {

	namespace {

		const ImGuiWindowFlags ImGuiNoWindow = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove;

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

	}

	VinlandWindow::VinlandWindow() {
	}

	VinlandWindow::~VinlandWindow() {
	}

	void VinlandWindow::eventUpdate(const SDL_Event& windowEvent) {
		sdl::ImGuiWindow::eventUpdate(windowEvent);
		canvas_.eventUpdate(windowEvent);

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
		canvas_.drawCanvas(deltaTime);
	}

    void VinlandWindow::imGuiUpdate(double deltaTime) {
		beginMain();

		ImGui::Button("hejasdasdasdasd");
		ImGui::SameLine();
		ImGui::Button("hejasdasdasdasd");
		ImGui::SameLine();
		ImGui::Button("hejasdasdasdasd");
		ImGui::SameLine();
		ImGui::Button("hejasdasdasdasd");
		ImGui::SameLine();
		ImGui::Button("hejasdasdasdasd");
		ImGui::SameLine();
		ImGui::Button("hejasdasdasdasd");
		drawHexTypesButtons();

		canvas_.drawImgui();
		endMain();
    }

	void VinlandWindow::drawHexTypesButtons() {
		ImGuiStyle& style = ImGui::GetStyle();
		const auto buttonsCount = hexTypes_.size();
		float windowVisible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
		
		int n = 0;
		for (auto& pair : hexTypes_) {
			auto& index = pair.second.index_;
			auto& hexImages = pair.second.hexImages_;
			ImVec2 buttonSize{50.f, 50.f};
			if (ImGui::ImageButton(hexImages[index].getImage(), buttonSize)) {
				if (hexImages[index].getImage() == canvas_.currentHexSprite()) {
					index = (index + 1) % hexImages.size();
				}
				canvas_.activateHexagon(hexImages[index]);
			}
			HelpMarker(hexImages[index].getFilename());
			float lastButton_x2 = ImGui::GetItemRectMax().x;
			float nextButton_x2 = lastButton_x2 + style.ItemSpacing.x + buttonSize.x; // Expected position if next button was on same line
			if (n + 1 < buttonsCount && nextButton_x2 < windowVisible_x2)
				ImGui::SameLine();
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
		canvas_.init(getImGuiShader());

		hexImages_ = HexData::getInstance().loadHexImages();
		for (const auto& hexImage : hexImages_) {
			hexTypes_[hexImage.getHexSides()].hexImages_.push_back(hexImage);
		}
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
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
	}

} // Namespace vin.
