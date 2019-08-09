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

	VinlandWindow::VinlandWindow() : page_(Page::START), whiteSquare_(1, 1, 255, 255, 255, 255) {
		hexImages_ = HexData::getInstance().loadHexImages();
		for (const auto& hexImage : hexImages_) {
			hexTypes_[hexImage.getHexSides()].hexImages_.push_back(hexImage);
		}
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
			case SDL_MOUSEWHEEL:
				if (windowEvent.wheel.y > 0) // scroll up
				{
					zoom_ *= 1.1f;
					// Put code for handling "scroll up" here!
					//logger()->info("windowEvent.wheel.y: {}", windowEvent.wheel.y);
				} else if (windowEvent.wheel.y < 0) // scroll down
				{
					zoom_ *= 1/1.1f;
					// Put code for handling "scroll down" here!
					//logger()->info("windowEvent.wheel.y: {}", windowEvent.wheel.y);
				}

				if (windowEvent.wheel.x > 0) // scroll right
				{
					// ...
				} else if (windowEvent.wheel.x < 0) // scroll left
				{
					// ...
				}
				break;
			case SDL_MOUSEMOTION:
				if (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
					x_ += windowEvent.motion.xrel;
					y_ += windowEvent.motion.yrel;
				}
				break;
		}
	}

	void VinlandWindow::imGuiPreUpdate(double deltaTime) {
        //const auto& shader = getImGuiShader();
        //shader.setMatrix(glm::mat4x4(1));
        //shader.setTextureId(1);
//		glActiveTexture(GL_TEXTURE1);
//		whiteSquare_.bindTexture();
//        hexagonBatch_.draw(getImGuiShader());
		auto [width, height] = sdl::Window::getSize();
		canvas_.update((float) width, (float) height, getImGuiShader(), deltaTime);
	}

    void VinlandWindow::imGuiUpdate(double deltaTime) {
		beginMain();

		//ImGui::ImageBackground(hexImages_[0].getImage());
		drawHexTypesButtons();

		//canvas_.draw();
		endMain();
		//ImGui::
    }

	void VinlandWindow::drawHexTypesButtons() {
		ImGuiStyle& style = ImGui::GetStyle();
		const auto buttonsCount = hexTypes_.size();
		float windowVisible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
		
		int n = 0;
		for (auto& pair : hexTypes_) {
			auto& index = pair.second.index_;
			auto& hexImages = pair.second.hexImages_;
			ImVec2 buttonSize(50.f, 50.f);
			if (ImGui::ImageButton(hexImages[index].getImage(), buttonSize)) {
				if (hexImages[index].getImage().getTexture() == canvas_.currentHexSprite().getTexture()) {
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
        sdl::ImGuiWindow::initOpenGl();
	}

	void VinlandWindow::initPreLoop() {
        sdl::ImGuiWindow::initPreLoop();
		auto [w, h] = sdl::ImGuiWindow::getSize();
        glViewport(0,0, w, h);

		/*
		const auto& shader = getImGuiShader();
		hexagonBatch_.addHexagon(0.1f, 0.1f, 0.5f);

		const ImU32 RED = Color(1.0f, 0.f, 0.f);
		hexagonBatch_.addHexagon(0.1f, 0.1f, 0.6f, 0.8f, RED);
		constexpr float innerRadius = 0.19f;
		constexpr float outerRadius = 0.2f;

		constexpr Layout layout(layoutPointy, {outerRadius, outerRadius}, {0.f, 0.f});

		for (int i = -5; i < 5; ++i) {
			for (int j = -5; j < 5; ++j) {
				auto pos = hex_to_pixel(layout, Hex(i, j, 0));
				hexagonBatch_.addHexagon(pos.x, pos.y, innerRadius, outerRadius, RED);
			}
		}

        hexagonBatch_.init(shader);
		glActiveTexture(GL_TEXTURE1);
		whiteSquare_.bindTexture();
		*/
		canvas_.init(getImGuiShader());
	}

	void VinlandWindow::beginMain() {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, {1.f, 1.f, 1.f, 0.f});

		ImGui::SetNextWindowPos({0.f, 0.f});
		auto [width, height] = sdl::Window::getSize();
		ImGui::SetNextWindowSize({(float)width, (float)height});

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
