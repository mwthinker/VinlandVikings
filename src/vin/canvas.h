#ifndef VINLANDVIKINGS_VIN_CANVAS_H
#define VINLANDVIKINGS_VIN_CANVAS_H

#include "types.h"
#include "hexagonbatch.h"

#include <sdl/batch.h>
#include <sdl/imguishader.h>
#include <sdl/sprite.h>
#include <sdl/vertexarrayobject.h>

#include <imgui.h>

namespace vin {

	void addHexagon(ImDrawList* drawList, ImVec2 center, float innerSize, float outerSize, ImU32 color);

	void grid(float zoom, float x, float y);

    class Canvas {
    public:
		Canvas();

		void update(double deltaTime);

		void draw();

		void eventUpdate(const SDL_Event& windowEvent);

    private:
        sdl::Batch<ImDrawVert> batch_;

		HexagonBatch hexagonBatch_;
		sdl::Texture whiteSquare_;
		float zoom_;
		float x_, y_;
		bool hasFocus_;
    };

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_HEXAGONBATCH_H
