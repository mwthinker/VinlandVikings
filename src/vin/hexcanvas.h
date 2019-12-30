#ifndef VINLANDVIKINGS_VIN_HEXCANVAS_H
#define VINLANDVIKINGS_VIN_HEXCANVAS_H

#include "types.h"
#include "canvas.h"
#include "hexagonbatch.h"
#include "hex/hexsideskey.h"
#include "hex/hexagon.h"
#include "hex/tileboard.h"
#include "heximage.h"
#include "shader.h"
#include "graphic.h"
#include "camera.h"
#include "tilesgraphic.h"
#include "hex/mapgenerator.h"
#include "tilelexicon.h"

#include <sdl/sprite.h>
#include <sdl/vertexarrayobject.h>

#include "logger.h"

#include <imgui.h>

#include <unordered_set>

namespace vin {

	void addHexagon(ImDrawList* drawList, ImVec2 center, float innerSize, float outerSize, ImU32 color);

	void HexagonImage(const sdl::TextureView& image, ImVec2 pos, ImVec2 size, float angle);

	class HexCanvas {
	public:
		HexCanvas();

		void drawCanvas(const std::chrono::high_resolution_clock::duration& deltaTime);

		void drawImgui();

		void eventUpdate(const SDL_Event& windowEvent);

		void activateHexagon(const Tile& tile) {
			currentTile_ = tile;
			activateHexagon_ = true;
		}

		void inactivateHexagon() {
			activateHexagon_ = false;
			currentTile_.sprite.rotations = 0;
			currentTile_.sides = {};
		}

		sdl::TextureView currentHexSprite() const {
			return currentTile_.sprite.sprite;
		}

		void setDeck(const std::vector<HexImage>& hexImages);

		void setTileLexicon(const TileLexicon& tileLexicon);

		void clear();

		void zoomIn();

		void zoomOut();

		void setHexCoords(bool activate);
		void setXYCoords(bool activate);
		void setGrid(bool activate);

		bool isHexCoords() const;
		bool isXYCoords() const;
		bool isGrid() const;

		void clearAndGenerateMap();

	private:
		void addTileMapToGraphic();

		hex::Hexi worldToHex(Vec2 pos) const;
		Vec2 hexToWorld(hex::Hexi pos) const;

		void addMouseHexToGraphic();
		void updateCanvasSize();

		Vec2 screenDeltaPosToWorld(Vec2 pos);

		hex::Hexi getHexFromMouse(Uint32 windowsId, int x, int y) const;
		hex::Hexi getHexFromMouse() const;

		sdl::Texture whiteSquare_;
		float zoom_ = 1.f;
		bool hasFocus_ = false;;
		bool isHovering_ = false;;
		bool activateHexagon_;
		hex::TileBoard tileBoard_;
		HexagonBatch hexagonBatch_;
		Shader shader_;
		Graphic graphic_;
		Mat2 hexToWorldModel_;

		Tile currentTile_;

		TilesGraphic tilesGraphic_;
		hex::MapGenerator hexMapGenerator_;

		bool lastAllowed_ = false;
		TileLexicon tileLexicon_;

		Vec2 sdlMousePos{};

		Camera camera_;

		ViewPort viewPort_{Vec2{0, 0}, Vec2{0 ,0}};
		Mat4 projection_;
		std::vector<HexImage> deck_;
	};

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_HEXCANVAS_H
