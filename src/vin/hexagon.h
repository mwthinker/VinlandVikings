#ifndef VINLANDVIKINGS_VIN_HEXAGON_H
#define VINLANDVIKINGS_VIN_HEXAGON_H

#include "types.h"
#include "logger.h"
#include "hexagonbatch.h"

#include <sdl/batch.h>
#include <sdl/imguishader.h>
#include <sdl/sprite.h>
#include <sdl/vertexarrayobject.h>

#include <imgui.h>

#include <iostream>
#include <array>
#include <type_traits>

namespace vin {

	enum class HexSide {
		NONE,
		MOUNTAIN,
		GRASS,
		WATER,
		FOREST,

		// Only used to count elements.
		COUNT_ELEMENTS
	};

	using HexSides = std::array<HexSide, 6>;

	constexpr char* toString(HexSide side) {
		switch (side) {
			case HexSide::NONE:
				return "NONE";
			case HexSide::MOUNTAIN:
				return "MOUNTAIN";
			case HexSide::GRASS:
				return "GRASS";
			case HexSide::WATER:
				return "WATER";
			case HexSide::FOREST:
				return "FOREST";
			default:
				logger()->error("Incorrect side type (toString): {}", (int) side);
				std::exit(1);
		}
	}

	template <class Type>
	class Hex {
	public:
		constexpr Hex() : q_(0), r_(0) {
			IS_ARITHMETIC<Type>();
		}

		constexpr Hex(Type q, Type r) : q_(q), r_(r) {
			IS_ARITHMETIC<Type>();
		}

		constexpr Hex(const Hex& hex) = default;
		constexpr Hex(Hex&&) = default;

		Hex& operator=(const Hex& hex) = default;
		Hex& operator=(Hex&&) = default;

		constexpr Type q() const { return q_; }
		constexpr Type r() const { return r_; }
		constexpr Type s() const { return -q_ - r_; }

		constexpr bool operator==(Hex hex) const { return q_ == hex.q_ && r_ == hex.r_; }

		constexpr Type operator-() const {
			return Type(-q_, -r_);
		}

		constexpr Type operator*(Type nbr) const {
			return Type(q_ * nbr, r_ * nbr);
		}

		constexpr Type operator+(Type hex) const {
			return Type(q_ + hex.q_, r_ + hex.r_);
		}

		constexpr Type operator-(Type hex) const {
			return Type(q_ - hex.q_, r_ - hex.r_);
		}

	private:
		template <class AritmeticType>
		constexpr void IS_ARITHMETIC() {
			static_assert(std::is_arithmetic<AritmeticType>(), "Hex type must be of arithmetic type");
		}

		Type q_, r_;
	};

	using Hexi = Hex<int>;
	using Hexf = Hex<float>;

	constexpr Hexi HEX_ZERO(0, 0);
	
	constexpr Hexi HEX_Q(1, 0);
	
	constexpr Hexi HEX_R(0, 1);
	
	constexpr Hexi HEX_S(-1, 1);

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
		auto angleDeg = 60 * nbr - 30;
		auto angleRad = PI / 180 * angleDeg;
		return {center.x + size * std::cos(angleRad), center.y + size * std::sin(angleRad)};
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

	constexpr float SQRT_3 = 1.7320508075688772935274463f;

	class Orientation {
	public:
		const float f0, f1, f2, f3;
		const float b0, b1, b2, b3;

		const float startAngle; // in multiples of 60°
		constexpr Orientation(float f0_, float f1_, float f2_, float f3_,
			float b0_, float b1_, float b2_, float b3_,
			float start_angle_)
			: f0(f0_), f1(f1_), f2(f2_), f3(f3_),
			b0(b0_), b1(b1_), b2(b2_), b3(b3_),
			startAngle(start_angle_) {}
	};

	constexpr Orientation layoutPointy
		= Orientation(SQRT_3, SQRT_3 / 2.f, 0.f, 3.f / 2.f,
		SQRT_3 / 3.f, -1.f / 3.f, 0.f, 2.f / 3.f,
		0.5f);

	constexpr Orientation layoutFlat
		= Orientation(3.f / 2.f, 0.f, SQRT_3 / 2.f, SQRT_3,
		2.f / 3.f, 0.f, -1.f / 3.f, SQRT_3 / 3.f,
		0.f);

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

	/*
	template <class HexType>
	std::ostream& operator<<(std::ostream& os, Hex<HexType> hex) {
		return os << "(" << hex.q() << ", " << hex.r() << ", " << hex.s();
	}
	*/

	/*
	std::ostream& operator<<(std::ostream& os, Hexi hex) {
		return os << "(" << hex.q() << ", " << hex.r() << ", " << hex.s();
	}

	std::ostream& operator<<(std::ostream& os, Hexf hex) {
		return os << "(" << hex.q() << ", " << hex.r() << ", " << hex.s();
	}
	*/

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_HEXAGON_H
