#ifndef VINLANDVIKINGS_VIN_HEX_TILE_H
#define VINLANDVIKINGS_VIN_HEX_TILE_H

#include "hex.h"
#include "hexsides.h"

namespace vin::hex {

	class Tile {
	public:
		Tile() = default;

		Tile(const Hexi& hex, const HexSides& sides)
			: hex_{hex}, sides_{sides} {
		}

		Tile(const Tile&) = default;
		Tile(Tile&&) = default;

		Tile& operator=(const Tile& tile) {
			hex_ = tile.hex_;
			sides_ = tile.sides_;
			return *this;
		}

		Tile& operator=(Tile&&) = default;

		bool operator==(const Tile& tile) const {
			return hex_ == tile.hex_ && sides_ == tile.sides_;
		}

		bool operator!=(const Tile& tile) const {
			return hex_ != tile.hex_ || sides_ != tile.sides_;
		}

		const Hexi& getHexi() const {
			return hex_;
		}

		const HexSides& getHexSides() const {
			return sides_;
		}

	private:
		Hexi hex_;
		HexSides sides_{};
	};

} // Namespace vin::hex.


#endif // VINLANDVIKINGS_VIN_HEX_TILE_H
