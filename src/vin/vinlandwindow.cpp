#include "vinlandwindow.h"

namespace vin {

    namespace {

        const ImGuiWindowFlags ImGuiNoWindow = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove;

        const ImGuiWindowFlags ImGuiNoWindow2 = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

    }

	VinlandWindow::VinlandWindow() : whiteSquare_(1, 1, 255, 255, 255, 255) {

	}

	VinlandWindow::~VinlandWindow() {

	}

	void VinlandWindow::eventUpdate(const SDL_Event& windowEvent) {
		sdl::ImGuiWindow::eventUpdate(windowEvent);

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
		}
	}

	void VinlandWindow::update(double deltaTime) {
        sdl::ImGuiWindow::update(deltaTime);
        const auto& shader = getImGuiShader();
        shader.setMatrix(glm::mat4x4(1));
        shader.setTextureId(0);
		whiteSquare_.bindTexture();
        hexagonBatch_.draw(getImGuiShader());
	}

    void VinlandWindow::updateImGui(double deltaTime) {

    }

	void VinlandWindow::initOpenGl() {
        sdl::ImGuiWindow::initOpenGl();
	}

	void VinlandWindow::initPreLoop() {
        sdl::ImGuiWindow::initPreLoop();
        //glViewport(0,0, 400, 400);
		hexagonBatch_.addHexagon(0.1f, 0.1f, 0.5f);
        hexagonBatch_.init(getImGuiShader());

	}

} // Namespace vin.
