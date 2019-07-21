#include "hextilemap.h"
#include "types.h"
#include "hexsideskey.h"

namespace vin {

	namespace {

		const HexSides NONE_SIDES ( HexSides {HexSide::NONE} );

	}

	std::vector<Hexi> createFlatHexShape(int radiusNbr) {
		std::vector<Hexi> hexes;
		for (int q = -radiusNbr; q <= radiusNbr; q++) {
			int r1 = std::max(-radiusNbr, -q - radiusNbr);
			int r2 = std::min(radiusNbr, -q + radiusNbr);
			for (int r = r1; r <= r2; r++) {
				hexes.emplace_back(q, r);
			}
		}
		return hexes;
	}

	std::vector<Hexi> createParallelogramShape(int columns, int rows) {
		std::vector<Hexi> hexes;
		for (int i = -10; i < 10; ++i) {
			for (int j = -10; j < 10; ++j) {
				hexes.emplace_back(i, j);
			}
		}
		return hexes;
	}

	bool HexTileMap::isInside(Hexi hex) const {
		return hexes_.find(hex) != hexes_.end();
	}

	bool HexTileMap::isEmpty(const Hexi& hex) {
		auto it = hexes_.find(hex);
		if (it == hexes_.end()) {
			return false;
		}

		return it->second.getHexSides() == NONE_SIDES;
	}

} // Namespace vin.
