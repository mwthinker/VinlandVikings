#include "vinlandwindow.h"
#include "canvas.h"
#include "logger.h"

namespace vin {

    namespace {

        const ImGuiWindowFlags ImGuiNoWindow = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove;

        const ImGuiWindowFlags ImGuiNoWindow2 = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

    }

	VinlandWindow::VinlandWindow() : page_(Page::START), whiteSquare_(1, 1, 255, 255, 255, 255) {
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
			case SDL_QUIT:
				sdl::Window::quit();
				break;
			case SDL_MOUSEWHEEL:
				if (windowEvent.wheel.y > 0) // scroll up
				{
					zoom_ *= 1.1f;
					// Put code for handling "scroll up" here!
					logger()->info("windowEvent.wheel.y: {}", windowEvent.wheel.y);
				} else if (windowEvent.wheel.y < 0) // scroll down
				{
					zoom_ *= 1/1.1f;
					// Put code for handling "scroll down" here!
					logger()->info("windowEvent.wheel.y: {}", windowEvent.wheel.y);
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

	void VinlandWindow::update(double deltaTime) {
        sdl::ImGuiWindow::update(deltaTime);
        const auto& shader = getImGuiShader();
        shader.setMatrix(glm::mat4x4(1));
        shader.setTextureId(1);
//		glActiveTexture(GL_TEXTURE1);
//		whiteSquare_.bindTexture();
//        hexagonBatch_.draw(getImGuiShader());
	}

    void VinlandWindow::updateImGui(double deltaTime) {
		beginMain();
		//grid(zoom_, x_, y_);
		canvas_.draw();
		endMain();
    }

	void VinlandWindow::initOpenGl() {
        sdl::ImGuiWindow::initOpenGl();
	}

	void VinlandWindow::initPreLoop() {
        sdl::ImGuiWindow::initPreLoop();

		
        //glViewport(0,0, 400, 400);

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
	}

	void VinlandWindow::beginMain() {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);

		ImGui::SetNextWindowPos({0.f, 0.f});
		auto [width, height] = sdl::Window::getSize();
		ImGui::SetNextWindowSize({(float)width, (float)height});

		ImGui::Begin("Main", nullptr, ImGuiNoWindow);
	}

	void VinlandWindow::endMain() {
		ImGui::End();

		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
	}

} // Namespace vin.
