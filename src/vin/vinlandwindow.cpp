#include "vinlandwindow.h"

namespace vin {	

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
		ImGuiWindow::update(deltaTime);
	}

	void VinlandWindow::initOpenGl() {
		ImGuiWindow::initOpenGl();
	}

	void VinlandWindow::initPreLoop() {
		ImGuiWindow::initPreLoop();
	}

} // Namespace tetris.
