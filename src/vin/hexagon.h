#ifndef VINLANDVIKINGS_VIN_HEXAGON_H
#define VINLANDVIKINGS_VIN_HEXAGON_H

#include "types.h"
#include "hex.h"
#include "hexsides.h"
#include "orientation.h"

#include <glm/gtx/rotate_vector.hpp>

#include <sdl/opengl.h>

#include <array>

namespace vin {

	constexpr std::array<Hexi, 6> CUBE_DIRECTIONS = {
		Hexi(1, -1), // s = 0
		Hexi(1, 0), // s = -1
		Hexi(0, 1), // s = -1
		Hexi(-1, 1), // s = 0
		Hexi(-1, 0), // s = 1
		Hexi(0, -1) // s = 1
	};

	const auto PI = glm::pi<GLfloat>();

	inline ImVec2 getHexCorner(ImVec2 center, float size, int nbr) {
		auto rad = PI / 3 * nbr - PI / 6;
		return {center.x + size * std::cos(rad), center.y + size * std::sin(rad)};
	}

	inline Vec2 getHexCorner(int nbr, float startAngle = 0) {
		return glm::rotate(Vec2{1, 0.f}, PI / 3 * nbr - PI / 6 + startAngle);
	}

	inline Vec2 getHexCorner(Vec2 center, GLfloat size, int nbr, float startAngle = 0) {
		return center + size * getHexCorner(nbr, startAngle);
	}

    inline std::array<Vec2, 6> getHexCorners(Vec2 center, GLfloat radius, float startAngle = 0) {
        std::array<Vec2, 6> corners;
        for (int i = 0; i < 6; ++i) {
            corners[i] = getHexCorner(center, radius, i, startAngle);
        }
		return corners;
	}

	inline ImDrawVert createHexCornerVertex(const ImDrawVert& vertex, float size, int nbr) {
		return  {getHexCorner(vertex.pos, size, nbr), vertex.uv, vertex.col};
	}

	constexpr Hexi cubeDirection(int direction) {
		return CUBE_DIRECTIONS[direction];
	}

	constexpr int abs(int value) {
		return value > 0 ? value : -value;
	}

	//constexpr CubeCoord cubeNeighbor(CubeCoord cube, CubeCoord direction) {
		//return cube_add(cube, cube_direction(direction))
	//}

	constexpr int cubeDistance(Hexi a, Hexi b) {
		return (abs(a.q() - b.q()) + abs(a.r() - b.r()) + abs(a.s() - b.s())) / 2;
	}

	struct Layout {
		const Orientation orientation;
		const Vec2 size;
		const Vec2 origin;
		constexpr Layout(const Orientation& orientation_, Vec2 size_, Vec2 origin_)
			: orientation(orientation_), size(size_), origin(origin_) {}
	};

	constexpr Vec2 hexToPixel(const Layout& layout, Hexi h) {
		const Orientation& M = layout.orientation;
		float x = (M.f0 * h.q() + M.f1 * h.r()) * layout.size.x;
		float y = (M.f2 * h.q() + M.f3 * h.r()) * layout.size.y;
		return {x + layout.origin.x, y + layout.origin.y};
	}

	inline Hexf pixelToHex(Layout layout, Vec2 p) {
		const Orientation& M = layout.orientation;
		Vec2 pt = Vec2((p.x - layout.origin.x) / layout.size.x,
			(p.y - layout.origin.y) / layout.size.y);
		float q = M.b0 * pt.x + M.b1 * pt.y;
		float r = M.b2 * pt.x + M.b3 * pt.y;
		return Hexf(q, r);
	}

	inline Hexi hexRound(Hexf h) {
		int q = (int) round(h.q());
		int r = (int) round(h.r());
		int s = (int) round(h.s());
		float q_diff = std::abs(q - h.q());
		float r_diff = std::abs(r - h.r());
		float s_diff = std::abs(s - h.s());
		if (q_diff > r_diff && q_diff > s_diff) {
			q = -r - s;
		} else if (r_diff > s_diff) {
			r = -q - s;
		} else {
			s = -q - r;
		}
		return Hexi(q, r);
	}

	inline Hexi oddToCube(int x, int y) {
		auto q = x - (y - (y & 1)) / 2;
		auto s = y;
		auto r = -q - s;
		return Hexi(q, r);
	}


} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_HEXAGON_H
