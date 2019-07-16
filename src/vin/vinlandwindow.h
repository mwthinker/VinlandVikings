#ifndef VINLANDVIKINGS_VIN_VINLANDWINDOW_H
#define VINLANDVIKINGS_VIN_VINLANDWINDOW_H

#include "hexagonbatch.h"
#include "canvas.h"

#include <sdl/imguiwindow.h>
#include <sdl/sprite.h>
#include <sdl/textureatlas.h>

namespace vin {

	class VinlandWindow : public sdl::ImGuiWindow {
	public:
		VinlandWindow();

		~VinlandWindow();

		void beginMain();

		void endMain();

        enum class Page { START };
	private:
		void initPreLoop() override;

		void initOpenGl() override;

		void update(double deltaTime) override;

        void updateImGui(double deltaTime) override;

		void eventUpdate(const SDL_Event& windowEvent) override;

		Canvas canvas_;
		Page page_;
		HexagonBatch hexagonBatch_;
		sdl::Texture whiteSquare_;
		float zoom_;
		float x_, y_;
	};

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_VINLANDWINDOW_H
