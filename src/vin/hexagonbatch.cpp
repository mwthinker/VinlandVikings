#include "hexagonbatch.h"

#include <sdl/opengl.h>

#include <imgui.h>
#include <glm/gtc/constants.hpp>

namespace vin {

    namespace {

        const auto PI = glm::pi<GLfloat>();

        inline ImDrawVert createVertex(float x, float y, ImU32 color) {
           return  {{x, y}, ImVec2(1.f, 1.f), color};
        }

        inline ImVec2 getHexCorner(ImVec2 center, float size, int nbr) {
            auto angleDeg = 60 * nbr - 30;
            auto angleRad = PI / 180 * angleDeg;
            return {center.x + size * std::cos(angleRad), center.y + size * std::sin(angleRad)};
        }

        inline ImDrawVert createHexCornerVertex(const ImDrawVert& vertex, float size, int nbr) {
            return  {getHexCorner(vertex.pos, size, nbr), vertex.uv, vertex.col};
        }

    }

    /*
    struct ImDrawVert
    {
        ImVec2  pos;
        ImVec2  uv;
        ImU32   col;
    };
     */

    void HexagonBatch::addHexagon(float x, float y, float size, ImU32 color) {
        auto center = createVertex(x, y, color);

        for (int i = 0; i < 6; ++i) {
            batch_.add(center);
            batch_.add(createHexCornerVertex(center, size, i));
            batch_.add(createHexCornerVertex(center, size, (i + 1) % 6));
        }
    }


} // Namespace vin.
