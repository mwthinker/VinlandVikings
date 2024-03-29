#ifndef VINLANDVIKINGS_VIN_VINLANDWINDOW_H
#define VINLANDVIKINGS_VIN_VINLANDWINDOW_H

#include "hexcanvas.h"
#include "configuration.h"
#include "hex/tilekey.h"
#include "tilelexicon.h"
#include "action.h"
#include "commandmanager.h"

#include <sdl/imguiwindow.h>
#include <sdl/sprite.h>
#include <sdl/shader.h>
#include <sdl/textureatlas.h>

#include <vector>
#include <map>

namespace vin {

	struct HexImageTypeVector {
		int index_ = 0;
		std::vector<HexImage> hexImages_;
	};

	using HexImagesMap = std::map<hex::TileInvariantKey, HexImageTypeVector>;

	class VinlandWindow : public sdl::ImGuiWindow {
	public:
		VinlandWindow();

		~VinlandWindow() override;

		enum class Page { START, EDITOR };
	private:
		void initPreLoop() override;

		void initOpenGl() override;

		void imGuiPreUpdate(const sdl::DeltaTime& deltaTime) override;
		void imGuiUpdate(const sdl::DeltaTime& deltaTime) override;

		void imGuiEventUpdate(const SDL_Event& windowEvent) override;

		void drawHexTypesButtons();
		
		void showMenuBar();

		void showAddHexImagePopup();
		void showHelpPopup();
		void showAboutPopup();

		void addFileInMenuBar();
		void addEditInMenuBar();
		void addViewInMenuBar();
		void addHelpInMenuBar();

		void initData();

		HexCanvas hexCanvas_;
		Page page_{Page::START};
		TileLexicon tileLexicon_;
		HexImagesMap hexTypes_;
		sdl::TextureView hexImage_;
		sdl::Shader shader_;

		std::vector<std::string> jsonFiles_;
		ActionManager actionManager_;
		
		Action load_;
		Action save_;
		Action saveAs_;
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

}

#endif
