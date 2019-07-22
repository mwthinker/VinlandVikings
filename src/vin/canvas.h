#ifndef VINLANDVIKINGS_VIN_CANVAS_H
#define VINLANDVIKINGS_VIN_CANVAS_H

#include "types.h"
#include "hexagonbatch.h"
#include "hexagon.h"
#include "hextilemap.h"
#include "heximage.h"

#include <sdl/sprite.h>
#include <sdl/vertexarrayobject.h>

#include <imgui.h>

#include <unordered_set>

namespace vin {

	void addHexagon(ImDrawList* drawList, ImVec2 center, float innerSize, float outerSize, ImU32 color);

	void HexagonImage(const sdl::Sprite& image, ImVec2 pos, ImVec2 size, float angle);

    class Canvas {
    public:
		Canvas();

		void update(double deltaTime);

		void draw();

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

		const sdl::Sprite& currentHexSprite() const {
			return hexImage_.getImage();
		}

    private:
		Hexi getHexFromMouse() const;

		sdl::Texture whiteSquare_;
		float zoom_;
		float x_, y_;
		bool hasFocus_;
		bool activateHexagon_;
		HexImage hexImage_;
		//float imageAngle_;
		HexTileMap hexTileMap_;

		HexTile lastHexTile_;
		bool lastAllowed_;
		std::unordered_map<Hexi, HexImage> hexImages_;
		int rotations_;
    };

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_HEXAGONBATCH_H
