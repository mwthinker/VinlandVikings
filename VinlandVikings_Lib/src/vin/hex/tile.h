#ifndef VINLANDVIKINGS_VIN_HEXSIDES_H
#define VINLANDVIKINGS_VIN_HEXSIDES_H

#include "../logger.h"

#include <array>

namespace vin::hex {

	enum class HexSide {
		None,
		Mountain,
		Grass,
		Water,
		Forest,

		// Only used to count elements.
		CountElements
	};

	constexpr int getNumberOfHexTypes() {
		return static_cast<int>(HexSide::CountElements);
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

		constexpr iterator begin() noexcept {
			return sides_.begin();
		}

		constexpr iterator end() noexcept {
			return sides_.end();
		}

		constexpr const_iterator begin() const {
			return sides_.begin();
		}

		constexpr const_iterator end() const {
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

	constexpr Tile HexSidesNone;

	inline void rotate(hex::Tile& hexSides, int rotations) {
		std::rotate(hexSides.begin(), hexSides.begin() + 6 - rotations % 6, hexSides.end());
	}

	inline hex::Tile rotateCopy(hex::Tile hexSides, int rotations) {
		std::rotate(hexSides.begin(), hexSides.begin() + rotations % 6, hexSides.end());
		return hexSides;
	}

	constexpr const char* toString(HexSide side) {
		switch (side) {
			case HexSide::None:
				return "None";
			case HexSide::Mountain:
				return "Mountain";
			case HexSide::Grass:
				return "Grass";
			case HexSide::Water:
				return "Water";
			case HexSide::Forest:
				return "Forest";
			default:
				spdlog::error("Incorrect side type (toString): {}", (int)side);
				std::exit(1);
		}
	}

}

#endif
