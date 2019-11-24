#include "hextilemap.h"
#include "../types.h"
#include "hexsideskey.h"

namespace vin::hex {

	std::vector<Hexi> createHexShape(int radiusNbr) {
		std::vector<Hexi> hexes;
		for (int q = -radiusNbr; q <= radiusNbr; ++q) {
			int r1 = std::max(-radiusNbr, -q - radiusNbr);
			int r2 = std::min(radiusNbr, -q + radiusNbr);
			for (int r = r1; r <= r2; ++r) {
				hexes.emplace_back(q, r);
			}
		}
		return hexes;
	}

	std::vector<Hexi> createParallelogramShape(int columns, int rows) {
		std::vector<Hexi> hexes;
		for (int i = -columns; i < columns; ++i) {
			for (int j = -rows; j < rows; ++j) {
				hexes.emplace_back(i, j);
			}
		}
		return hexes;
	}

	std::vector<Hexi> createRectangleShape(int columns, int rows) {
		std::vector<Hexi> hexes;
		for (int x = -columns; x < columns; ++x) {
			for (int y = -rows; y < rows; ++y) {
				auto hex = oddToCube(x, y);
				hexes.push_back(hex);
			}
		}
		return hexes;
	}

    HexTileMap::HexTileMap() {
    }

	HexTileMap::HexTileMap(HexTileMap&& map) noexcept : hexes_{std::move(map.hexes_)} {
    }

    HexTileMap& HexTileMap::operator=(HexTileMap&& map) noexcept {
        hexes_ = std::move(map.hexes_);
        return *this;
    }

    HexTileMap::HexTileMap(const std::vector<Hexi>& hexes) {
        for (const auto& hex : hexes) {
			hexes_.insert({hex, Tile{hex, HEXSIDES_NONE}});
        }
    }

	bool HexTileMap::isInside(const Hexi& hex) const {
		return hexes_.find(hex) != hexes_.end();
	}

	bool HexTileMap::isEmpty(const Hexi& hex) const {
		auto it = hexes_.find(hex);
		if (it == hexes_.end()) {
			return false;
		}

		return it->second.getHexSides() == HEXSIDES_NONE;
	}

	bool HexTileMap::isAllowed(Tile hexTile) const {
		//logger()->info("isAllowed");

		if (!isInside(hexTile.getHexi())) {
			//logger()->info("Not inside");
			return false;
		}

		if (!isEmpty(hexTile.getHexi())) {
			//logger()->info("Not empty");
			return false;
		}

		return isNeighborsMatching(hexTile);
	}

    bool HexTileMap::isNeighborsMatching(Tile hexTile) const {
        HexSides sides = hexTile.getHexSides();
        for (int i = 0; i < 6; ++i) {
            Hexi pos = CUBE_DIRECTIONS[i] + hexTile.getHexi();
			Tile neighbor = getTile(pos);

            int oppositeSide = (i + 3) % 6;
            HexSide neighborSide = neighbor.getHexSides()[oppositeSide];
            if (neighborSide == HexSide::NONE) {
                continue;
            }

            if (sides[i] != neighborSide) {
                return false;
            }
        }
        return true;
	}

	Tile HexTileMap::getTile(Hexi hex) const {
		auto it = hexes_.find(hex);
		if (it == hexes_.end()) {
			return Tile{hex, HEXSIDES_NONE};
		}
		return it->second;
	}

    void HexTileMap::clear() {
        for (auto& hexTile : hexes_) {
			hexTile.second = Tile{hexTile.first, HEXSIDES_NONE};
        }
	}

	bool HexTileMap::put(const Tile& tile) {
		if (!isAllowed(tile)) {
			return false;
		}
		hexes_[tile.getHexi()] = tile;
		return true;
	}

} // Namespace vin::hex.
