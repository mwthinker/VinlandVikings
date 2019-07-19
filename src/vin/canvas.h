#ifndef VINLANDVIKINGS_VIN_CANVAS_H
#define VINLANDVIKINGS_VIN_CANVAS_H

#include "types.h"
#include "hexagonbatch.h"
#include "hexagon.h"

#include <sdl/batch.h>
#include <sdl/imguishader.h>
#include <sdl/sprite.h>
#include <sdl/vertexarrayobject.h>

#include <imgui.h>

#include <unordered_set>

namespace std {
	template <> struct hash<vin::Hexi> {
		size_t operator()(const vin::Hexi& h) const {
			hash<int> int_hash;
			size_t hq = int_hash(h.q());
			size_t hr = int_hash(h.r());
			return hq ^ (hr + 0x9e3779b9 + (hq << 6) + (hq >> 2));
		}
	};
}

namespace vin {

	void addHexagon(ImDrawList* drawList, ImVec2 center, float innerSize, float outerSize, ImU32 color);

	void HexagonImage(const sdl::Sprite& image, ImVec2 pos, ImVec2 size, float angle);

    class Canvas {
    public:
		Canvas();

		void update(double deltaTime);

		void draw();

		void eventUpdate(const SDL_Event& windowEvent);

		void activateHexagon(const sdl::Sprite& sprite) {
			image_ = sprite;
			activateHexagon_ = true;
		}

		void inactivateHexagon() {
			activateHexagon_ = false;
		}

    private:
        sdl::Batch<ImDrawVert> batch_;

		HexagonBatch hexagonBatch_;
		sdl::Texture whiteSquare_;
		float zoom_;
		float x_, y_;
		bool hasFocus_;
		bool activateHexagon_;
		sdl::Sprite image_;
		float imageAngle_;
		std::unordered_set<Hexi> hexes_;

    };

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_HEXAGONBATCH_H
