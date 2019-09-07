#ifndef VINLANDVIKINGS_VIN_ORIENTATION_H
#define VINLANDVIKINGS_VIN_ORIENTATION_H

#include "hex.h"
#include "types.h"

namespace vin {

	/*
	class Orientation {
	public:
		const float f0, f1, f2, f3;
		const float b0, b1, b2, b3;

		const float startAngle; // in multiples of 60?
		constexpr Orientation(float f0_, float f1_, float f2_, float f3_,
			float b0_, float b1_, float b2_, float b3_,
			float start_angle_)
			: f0(f0_), f1(f1_), f2(f2_), f3(f3_),
			b0(b0_), b1(b1_), b2(b2_), b3(b3_),
			startAngle(start_angle_) {}
	};

	constexpr float SQRT_3 = 1.7320508075688772935274463f;

	constexpr Orientation layoutPointy
		= Orientation(SQRT_3, SQRT_3 / 2.f, 0.f, 3.f / 2.f,
		SQRT_3 / 3.f, -1.f / 3.f, 0.f, 2.f / 3.f,
		0.5f);

	constexpr Orientation layoutFlat
		= Orientation(
		3.f / 2.f, 0.f,
		SQRT_3 / 2.f, SQRT_3,
		
		2.f / 3.f, 0.f, -1.f / 3.f, SQRT_3 / 3.f,
		0.f);

	constexpr glm::mat2x2 SIZE = {
		1, 0, // width
		0, 1, // height
	};

	constexpr glm::mat2x2 majorRowToColumn(float f0, float f1, float f2, float f3) {
		return glm::mat2x2(1);
	};
	*/


	/*
	glm::mat3x3 orientationFlatGlm = {
		3.f / 2.f, 0.f, 
		SQRT_3 / 2.f, SQRT_3, 0.f,
		0.f, 0.f, 0.f
	};
	*/

	/*
	Layout createFlatLayout(float x, float y, float zoom) {
		return Layout(layoutPointy, {createInnerRadius(zoom), createOuterRadius(zoom)}, {300.f + x, 300.f + y});
	}
	
	

	glm::mat3x3 createFlatLayoutGlm(Vec2 pos, Vec2 size) {
		return glm::mat3x3{};
	}

	Vec2 hexToXY(float angle, Hexi h) {
		return 
	}

	constexpr glm::mat2x2 FLAT = {
		3.f / 2.f, SQRT_3 / 2.f,
		0.f, SQRT_3
	};
	*/

	/*
	glm::mat2x2 orientationFlat(glm::vec2 pos, glm::vec2 size) {
		return glm::mat2x2(size.x, 0.f, 0.f, size.y) * FLAT * pos;
	};
	*/

	/*
	constexpr glm::mat2x2 POINTY = {
		3.f / 2.f, SQRT_3 / 2.f,
		0.f, SQRT_3
	};
	*/


} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_ORIENTATION_H
