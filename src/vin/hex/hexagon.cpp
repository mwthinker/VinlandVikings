#include "hexagon.h"

namespace vin::hex {

	ImVec2 getHexCorner(ImVec2 center, float size, int nbr) {
		auto rad = Pi / 3 * nbr;
		return {center.x + size * std::cos(rad), center.y + size * std::sin(rad)};
	}

	Vec2 getHexCorner(int nbr, float startAngle) {
		return glm::rotate(Vec2{1, 0.f}, Pi / 3 * nbr + startAngle);
	}

	Vec2 getHexCorner(Vec2 center, gl::GLfloat size, int nbr, float startAngle) {
		return center + size * getHexCorner(nbr, startAngle);
	}

	std::array<Vec2, 6> getHexCorners(Vec2 center, gl::GLfloat radius, float startAngle) {
		std::array<Vec2, 6> corners;
		for (int i = 0; i < 6; ++i) {
			corners[i] = getHexCorner(center, radius, i, startAngle);
		}
		return corners;
	}

	ImDrawVert createHexCornerVertex(const ImDrawVert& vertex, float size, int nbr) {
		return {getHexCorner(vertex.pos, size, nbr), vertex.uv, vertex.col};
	}

	Hexi hexRound(Hexf h) {
		auto q = static_cast<int>(std::round(h.q()));
		auto r = static_cast<int>(std::round(h.r()));
		auto s = static_cast<int>(std::round(h.s()));
		auto q_diff = std::abs(q - h.q());
		auto r_diff = std::abs(r - h.r());
		auto s_diff = std::abs(s - h.s());
		if (q_diff > r_diff && q_diff > s_diff) {
			q = -r - s;
		} else if (r_diff > s_diff) {
			r = -q - s;
		}
		return {q, r};
	}

}
