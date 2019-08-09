#ifndef VINLANDVIKINGS_VIN_HEXSIDES_H
#define VINLANDVIKINGS_VIN_HEXSIDES_H

#include "logger.h"

#include <array>

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

	constexpr int getNumberOfHexTypes() {
		return static_cast<int>(HexSide::COUNT_ELEMENTS);
	}

	using HexSides = std::array<HexSide, 6>;
	constexpr HexSides HEXSIDES_NONE = {HexSide::NONE};

	constexpr const char* toString(HexSide side) {
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
				logger()->error("Incorrect side type (toString): {}", (int)side);
				std::exit(1);
		}
	}

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_HEXSIDES_H
