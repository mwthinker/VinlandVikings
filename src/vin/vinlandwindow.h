#ifndef VINLANDVIKINGS_VIN_VINLANDWINDOW_H
#define VINLANDVIKINGS_VIN_VINLANDWINDOW_H

#include "hexagonbatch.h"
#include "canvas.h"
#include "hexdata.h"
#include "hexsideskey.h"

#include <sdl/imguiwindow.h>
#include <sdl/sprite.h>
#include <sdl/textureatlas.h>

#include <vector>
#include <map>

namespace vin {
	
	class HexImageTypeVector {
	public:
		HexImageTypeVector() : index_(0) {
		}

		int index_;
		std::vector<HexImage> hexImages_;
	};

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

		void imGuiPreUpdate(double deltaTime) override;

        void imGuiUpdate(double deltaTime) override;

		void eventUpdate(const SDL_Event& windowEvent) override;

		void drawHexTypesButtons();

		Canvas canvas_;
		Page page_;
		HexagonBatch hexagonBatch_;
		sdl::Texture whiteSquare_;		
		std::vector<HexImage> hexImages_;
		std::map<HexSidesKey, HexImageTypeVector> hexTypes_;
	};

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_VINLANDWINDOW_H
