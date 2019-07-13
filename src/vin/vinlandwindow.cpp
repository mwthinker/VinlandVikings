#include "vinlandwindow.h"

namespace vin {

    namespace {

        const ImGuiWindowFlags ImGuiNoWindow = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove;

        const ImGuiWindowFlags ImGuiNoWindow2 = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

    }

	VinlandWindow::VinlandWindow() {

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
        shader.setTexture(false);
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
        hexagonBatch_.init(getImGuiShader());
        hexagonBatch_.addHexagon(100, 100, 30);
        hexagonBatch_.uploadToGraphicCard();
	}

} // Namespace vin.
