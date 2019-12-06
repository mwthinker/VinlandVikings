#ifndef VINLANDVIKINGS_VIN_CANVAS_H
#define VINLANDVIKINGS_VIN_CANVAS_H

#include "types.h"
#include "hexagonbatch.h"
#include "hex/hash.h"
#include "hex/hexagon.h"
#include "hex/tileboard.h"
#include "heximage.h"
#include "shader.h"
#include "graphic.h"
#include "camera.h"
#include "tilesgraphic.h"
#include "hex/mapgenerator.h"

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

	struct HexImageTypeVector {
		int index_{0};
		std::vector<HexImage> hexImages_;
	};

	using HexImagesMap = std::map<hex::HexSidesInvariantKey, HexImageTypeVector>;

	/*
	class HexImageLexicon {
	public:
		HexImageLexicon() {
			
		}
	private:
		HexImagesMap map_;
	};
	*/

    class HexWorldCanvas {
    public:
		HexWorldCanvas();

		void drawCanvas(double deltaTime);

		void drawImgui();

		void eventUpdate(const SDL_Event& windowEvent);

		void activateHexagon(const HexImage& hexImage) {
			hexImage_ = hexImage;
			activateHexagon_ = true;
			//rotations_ = rotations;
		}

		void inactivateHexagon() {
			activateHexagon_ = false;
			rotations_ = 0;
		}

		SpriteView currentHexSprite() const {
			return hexImage_.getImage();
		}

		void setDefaultHexSprite(const HexImage& hexImage);

		void setDeck(const std::vector<HexImage>& hexImages);

		void setHexImagesMap(const HexImagesMap& hexImagesMap);

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
		HexImage hexImage_;
		hex::TileBoard tileBoard_;
		HexagonBatch hexagonBatch_;
		Shader shader_;
		Graphic graphic_;
		Mat2 hexToWorldModel_;
		TilesGraphic tilesGraphic_;
		hex::MapGenerator hexMapGenerator_;

		hex::Tile lastHexTile_;
		bool lastAllowed_ = false;
		int rotations_ = 0;

		Vec2 sdlMousePos{};
		
		Camera camera_;
		
		ViewPort viewPort_{ Vec2{0, 0}, Vec2{0 ,0}};
		Mat4 projection_;
		std::vector<HexImage> deck_;
		HexImagesMap hexTypes_;
		//std::unordered_map<hex::HexSides, HexImageTypeVector> hexImages_;
    };

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_HEXAGONBATCH_H
