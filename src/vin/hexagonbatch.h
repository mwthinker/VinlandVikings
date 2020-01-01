#ifndef VINLANDVIKINGS_VIN_HEXAGONBATCH_H
#define VINLANDVIKINGS_VIN_HEXAGONBATCH_H

#include "types.h"

#include <sdl/batch.h>
#include <sdl/shader.h>
#include <sdl/sprite.h>
#include <sdl/vertexarrayobject.h>

#include <imgui.h>

namespace vin {

    class HexagonBatch {
    public:
		HexagonBatch();

        virtual ~HexagonBatch() = default;

		void init(const sdl::Shader& shader);

		void draw(const sdl::Shader& shader) const;

		void uploadToGraphicCard();

		void clear();

        void addHexagon(float x, float y, float size, ImU32 color = WHITE.toImU32());

		void addHexagon(float x, float y, float innerSize, float outerSize, ImU32 color = WHITE.toImU32());

		void addHexagonImage(float x, float y, float size, const sdl::Sprite& sprite, ImU32 color = WHITE.toImU32());

		void addRectangle(float x, float y, float w, float h, const sdl::Sprite& sprite, ImU32 color = WHITE.toImU32());

		void addRectangle(const ImDrawVert& v1, const ImDrawVert& v2, const ImDrawVert& v3, const ImDrawVert& v4, ImU32 color = WHITE.toImU32());

		void addCircle(float x, float y, float radius, ImU32 color = WHITE.toImU32(), int iterations = 40);

		void addLine(float x1, float y1, float x2, float y2, float w, ImU32 color = WHITE.toImU32());

		void addAnnulus(float x, float y, float outerRadius, float innerRadius, ImU32 color = WHITE.toImU32(), int iterations = 40);

		void addTriangle(const ImDrawVert& v1, const ImDrawVert& v2, const ImDrawVert& v3);

    private:
        sdl::Batch<ImDrawVert> batch_;
        sdl::VertexArrayObject vao_;
    };

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_HEXAGONBATCH_H
