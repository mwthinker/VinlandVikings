#ifndef VINLANDVIKINGS_VIN_HEXSIDES_H
#define VINLANDVIKINGS_VIN_HEXSIDES_H

#include "../logger.h"

#include <array>

namespace vin::hex {

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

	/*
	class HexSides {
	public:
		using Array = std::array<HexSide, 6>;
		using const_iterator = Array::const_iterator;

		inline constexpr HexSide& operator[](int index) {
			return sides_[index];
		}

		inline constexpr const HexSide& operator[](int index) const{
			return sides_[index];
		}

		inline constexpr const_iterator begin() const {
			return sides_.begin();
		}

		inline constexpr const_iterator end() const {
			return sides_.end();
		}

		inline bool operator==(const HexSides& hexSides) const {
			return sides_ == hexSides.sides_;
		}

		inline bool operator!=(const HexSides& hexSides) const {
			return sides_ != hexSides.sides_;
		}
		
	private:
		std::array<HexSide, 6> sides_{};
	};
	*/

	using HexSides = std::array<HexSide, 6>;
	constexpr HexSides HEXSIDES_NONE{};

	inline void rotate(hex::HexSides& hexSides, int rotations) {
		std::rotate(hexSides.begin(), hexSides.begin() + 6 - rotations % 6, hexSides.end());
	}

	inline hex::HexSides rotateCopy(hex::HexSides hexSides, int rotations) {
		std::rotate(hexSides.begin(), hexSides.begin() + rotations % 6, hexSides.end());
		return hexSides;
	}

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

} // Namespace vin::hex.

#endif // VINLANDVIKINGS_VIN_HEXSIDES_H
