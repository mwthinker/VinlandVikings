#ifndef VINLANDVIKINGS_VIN_HEXAGON_H
#define VINLANDVIKINGS_VIN_HEXAGON_H

#include "../types.h"
#include "hex.h"
#include "hexsides.h"

#include <glm/gtx/rotate_vector.hpp>

#include <sdl/opengl.h>

#include <array>

namespace vin::hex {

	constexpr std::array<Hexi, 6> CUBE_DIRECTIONS{
		Hexi{1, -1}, // s = 0
		Hexi{1, 0}, // s = -1
		Hexi{0, 1}, // s = -1
		Hexi{-1, 1}, // s = 0
		Hexi{-1, 0}, // s = 1
		Hexi{0, -1} // s = 1
	};

	ImVec2 getHexCorner(ImVec2 center, float size, int nbr);

	Vec2 getHexCorner(int nbr, float startAngle = 0);

	Vec2 getHexCorner(Vec2 center, GLfloat size, int nbr, float startAngle = 0);

	std::array<Vec2, 6> getHexCorners(Vec2 center, GLfloat radius, float startAngle = 0);

	ImDrawVert createHexCornerVertex(const ImDrawVert& vertex, float size, int nbr);

	constexpr Hexi cubeDirection(int direction) {
		return CUBE_DIRECTIONS[direction];
	}

	constexpr int abs(int value) {
		return value > 0 ? value : -value;
	}

	constexpr int cubeDistance(Hexi a, Hexi b) {
		return (abs(a.q() - b.q()) + abs(a.r() - b.r()) + abs(a.s() - b.s())) / 2;
	}

	constexpr float SQRT_3 = 1.7320508075688772935274463f;

	inline Mat2 createHexToCoordModel(float angle = 0.f, float hexSize = 1.f) {
		float cos = hexSize * std::cos(angle);
		float sin = hexSize * std::sin(angle);
		return Mat2{cos, sin, -sin, cos} * Mat2{3.f / 2.f, SQRT_3 / 2, 0.f, SQRT_3};
	}

	Hexi hexRound(Hexf h);

	Hexi oddToCube(int x, int y);

} // Namespace vin::hex.

#endif // VINLANDVIKINGS_VIN_HEXAGON_H