#ifndef VINLANDVIKINGS_VIN_VINLANDWINDOW_H
#define VINLANDVIKINGS_VIN_VINLANDWINDOW_H

#include "hexcanvas.h"
#include "hexdata.h"
#include "hex/hexsideskey.h"
#include "tilelexicon.h"
#include "action.h"

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

		enum class Page { START, EDITOR };
	private:
		void initPreLoop() override;

		void initOpenGl() override;

		void imGuiPreUpdate(const std::chrono::high_resolution_clock::duration& deltaTime) override;
		void imGuiUpdate(const std::chrono::high_resolution_clock::duration& deltaTime) override;

		void eventUpdate(const SDL_Event& windowEvent) override;

		void drawHexTypesButtons();

		void showMenuBar();

		void showAddHexImagePopup();
		
		void addFileInMenuBar();
		void addEditInMenuBar();
		void addViewInMenuBar();

		void initData();

		HexCanvas hexCanvas_;
		Page page_{Page::START};
		TileLexicon tileLexicon_;
		HexImagesMap hexTypes_;
		sdl::TextureView hexImage_;

		std::vector<std::string> jsonFiles_;
		ActionManager actionManager_;
		
		Action zoomIn_;
		Action zoomOut_;
		Action undo_;
		Action redo_;
		Action gridAction_;
		Action clearAction_;
		Action xyCoordsAction_;
		Action hexCoordsAction_;
		Action generateMapAction_;
	};

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_VINLANDWINDOW_H
