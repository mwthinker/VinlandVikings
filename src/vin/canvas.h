#ifndef VINLANDVIKINGS_VIN_CANVAS_H
#define VINLANDVIKINGS_VIN_CANVAS_H

#include "types.h"
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

	void HexagonImage(const vin::SpriteView& image, ImVec2 pos, ImVec2 size, float angle);

	struct ViewPort {
		Vec2 pos;
		Vec2 size;
	};

	class Canvas {
	public:
		virtual ~Canvas() = default;

		virtual void drawImgui() = 0;

		virtual void drawCanvas(double deltaTime) = 0;

		void eventUpdate(const SDL_Event& windowEvent);

		void updateCanvasSize();

		Mat4 getProjection() const;

	private:
		bool hasFocus_ = false;;
		bool isHovering_ = false;;
		Mat4 projection_{1};
		ViewPort viewPort_{Vec2{0, 0}, Vec2{0 ,0}};
	};

    class HexWorldCanvas {
    public:
		HexWorldCanvas();

		void drawCanvas(double deltaTime);

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

		SpriteView currentHexSprite() const {
			return currentTile_.sprite.sprite;
		}

		void setDeck(const std::vector<HexImage>& hexImages);

		void setTileLexicon(const TileLexicon& tileLexicon);

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
		
		ViewPort viewPort_{ Vec2{0, 0}, Vec2{0 ,0}};
		Mat4 projection_;
		std::vector<HexImage> deck_;
    };

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_CANVAS_H
