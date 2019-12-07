#ifndef VINLANDVIKINGS_VIN_VINLANDWINDOW_H
#define VINLANDVIKINGS_VIN_VINLANDWINDOW_H

#include "hexagonbatch.h"
#include "canvas.h"
#include "hexdata.h"
#include "hex/hexsideskey.h"
#include "tilelexicon.h"

#include <sdl/imguiwindow.h>
#include <sdl/sprite.h>
#include <sdl/textureatlas.h>

#include <vector>
#include <map>

namespace vin {

	struct HexImageTypeVector {
		int index_{0};
		std::vector<HexImage> hexImages_;
	};

	using HexImagesMap = std::map<hex::HexSidesInvariantKey, HexImageTypeVector>;

	class VinlandWindow : public sdl::ImGuiWindow {
	public:
		VinlandWindow();

		~VinlandWindow();

		void beginMain();

		void endMain();

        enum class Page { START, EDITOR };
	private:
		void initPreLoop() override;

		void initOpenGl() override;

		void imGuiPreUpdate(double deltaTime) override;

        void imGuiUpdate(double deltaTime) override;

		void eventUpdate(const SDL_Event& windowEvent) override;

		void drawHexTypesButtons();

		void showExampleMenuFile();

		HexWorldCanvas hexWorldCanvas;
		Page page_{Page::START};
		HexagonBatch hexagonBatch_;
		TileLexicon tileLexicon_;
		HexImagesMap hexTypes_;
	};

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_VINLANDWINDOW_H
