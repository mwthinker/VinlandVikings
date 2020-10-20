#ifndef VINLANDVIKINGS_VIN_HEXCANVAS_H
#define VINLANDVIKINGS_VIN_HEXCANVAS_H

#include "types.h"
#include "hexagonbatch.h"
#include "hex/tilekey.h"
#include "hex/hexagon.h"
#include "hex/tileboard.h"
#include "heximage.h"
#include "graphic.h"
#include "camera.h"
#include "tilesgraphic.h"
#include "hex/mapgenerator.h"
#include "tilelexicon.h"
#include "commandmanager.h"
#include "logger.h"
#include "deck.h"

#include <sdl/shader.h>
#include <sdl/sprite.h>
#include <sdl/vertexarrayobject.h>

#include <unordered_set>
#include <stack>

namespace vin {

	struct ViewPort {
		Vec2 pos;
		Vec2 size;
	};

	struct CanvasSnapshot {
		hex::TileBoard tileBoard;
		TilesGraphic::Map tileMap;
	};

	class HexCanvas {
	public:
		HexCanvas();

		void drawCanvas(const sdl::Shader& shader, const std::chrono::high_resolution_clock::duration& deltaTime);

		void eventUpdate(const SDL_Event& windowEvent);

		void activateHexagon(const SpriteTile& tile) {
			currentTile_ = tile;
			activateHexagon_ = true;
		}

		void inactivateHexagon() {
			activateHexagon_ = false;
			currentTile_.sprite.rotations = 0;
			currentTile_.tile = {};
		}

		sdl::TextureView currentHexSprite() const {
			return currentTile_.sprite.sprite;
		}

		Deck getDeck() const;
		void setDeck(const Deck& deck);

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
		void updateCanvasSize(const Vec2& pos, const Vec2& size);

		void redo();
		void undo();

		CanvasSnapshot getSnapshot() const;

		void setSnapshot(const CanvasSnapshot& snapshot);

	private:
		void addTileMapToGraphic();

		hex::Hexi worldToHex(Vec2 pos) const;
		Vec2 hexToWorld(hex::Hexi pos) const;

		void addMouseHexToGraphic();

		Vec2 screenDeltaPosToWorld(Vec2 pos);

		hex::Hexi getHexFromMouse(Uint32 windowsId, int x, int y) const;
		hex::Hexi getHexFromMouse() const;

		void rotateCurrentTile(hex::Hexi hex);
		
		sdl::Texture whiteSquare_;
		float zoom_{0.048f};
		bool activateHexagon_;
		hex::TileBoard tileBoard_;
		HexagonBatch hexagonBatch_;
		Graphic graphic_;
		Mat2 hexToWorldModel_;

		SpriteTile currentTile_;

		TilesGraphic tilesGraphic_;
		hex::MapGenerator hexMapGenerator_;

		bool lastAllowed_{};
		TileLexicon tileLexicon_;

		Vec2 sdlMousePos{};
		Camera camera_;

		ViewPort viewPort_{};
		Mat4 projection_;
		Deck deck_;

		CommandManager<HexCanvas, CanvasSnapshot> commandManager_;
	};

}

#endif
