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
	
	class Tile {
	public:
		using Array = std::array<HexSide, 6>;
		using const_iterator = Array::const_iterator;
		using iterator = Array::iterator;

		constexpr HexSide& operator[](int index) {
			return sides_[index];
		}

		constexpr const HexSide& operator[](int index) const{
			return sides_[index];
		}

		iterator begin() noexcept {
			return sides_.begin();
		}

		iterator end() noexcept {
			return sides_.end();
		}

		constexpr const_iterator begin() const noexcept {
			return sides_.begin();
		}

		constexpr const_iterator end() const noexcept {
			return sides_.end();
		}

		bool operator==(const Tile& hexSides) const {
			return sides_ == hexSides.sides_;
		}

		bool operator!=(const Tile& hexSides) const {
			return sides_ != hexSides.sides_;
		}
		
	private:
		std::array<HexSide, 6> sides_{};
	};

	//using HexSides = std::array<HexSide, 6>;
	constexpr Tile HEXSIDES_NONE;

	inline void rotate(hex::Tile& hexSides, int rotations) {
		std::rotate(hexSides.begin(), hexSides.begin() + 6 - rotations % 6, hexSides.end());
	}

	inline hex::Tile rotateCopy(hex::Tile hexSides, int rotations) {
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
				spdlog::error("Incorrect side type (toString): {}", (int)side);
				std::exit(1);
		}
	}

} // Namespace vin::hex.

#endif // VINLANDVIKINGS_VIN_HEXSIDES_H
