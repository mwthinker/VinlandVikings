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

	bool HexTileMap::isInside(const Hexi& hex) const {
		return hexes_.find(hex) != hexes_.end();
	}

	bool HexTileMap::isEmpty(const Hexi& hex) const {
		auto it = hexes_.find(hex);
		if (it == hexes_.end()) {
			return false;
		}

		return it->second.getHexSides() == NONE_SIDES;
	}

	bool HexTileMap::isAllowed(HexTile hexTile) const {
		logger()->info("isAllowed");

		if (!isInside(hexTile.getHexi())) {
			logger()->info("Not inside");
			return false;
		}

		if (!isEmpty(hexTile.getHexi())) {
			logger()->info("Not empty");
			return false;
		}

		HexSides sides = hexTile.getHexSides();
		for (int i = 0; i < 6; ++i) {
			Hexi pos = CUBE_DIRECTIONS[i] + hexTile.getHexi();
			//HexTile hexTile = hexes.
			HexTile neighbor = getHexTile(pos);

			HexSide neighborSide = neighbor.getHexSides()[i];			
			logger()->info("neighborSide {}: {}", i, toString(neighborSide));
			if (neighborSide == HexSide::NONE) {
				continue;
			}

			if (sides[i] != neighborSide) {
				return false;
			}
		}

		return true;
	}

	HexTile HexTileMap::getHexTile(Hexi hex) const {
		auto it = hexes_.find(hex);
		if (it == hexes_.end()) {
			return HexTile(hex, NONE_SIDES);
		}
		return it->second;
	}

    void HexTileMap::clear() {
        for (auto& hexTile : hexes_) {
            hexTile.second = HexTile(hexTile.first, {HexSide::NONE});
        }
	}

	bool HexTileMap::put(const HexTile& tile) {
		if (!isAllowed(tile)) {
			return false;
		}
		hexes_[tile.getHexi()] = tile;
		return true;
	}

} // Namespace vin.
