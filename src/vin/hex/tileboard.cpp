#include "tileboard.h"
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

	TileBoard::TileBoard() {
    }

	TileBoard::TileBoard(TileBoard&& map) noexcept : hexes_{std::move(map.hexes_)} {
    }

	TileBoard& TileBoard::operator=(TileBoard&& map) noexcept {
        hexes_ = std::move(map.hexes_);
        return *this;
    }

	TileBoard::TileBoard(const std::vector<Hexi>& hexes) {
        for (const auto& hex : hexes) {
			hexes_.insert({hex, HEXSIDES_NONE});
        }
    }

	bool TileBoard::isInside(const Hexi& hex) const {
		return hexes_.find(hex) != hexes_.end();
	}

	bool TileBoard::isEmpty(const Hexi& hex) const {
		auto it = hexes_.find(hex);
		if (it == hexes_.end()) {
			return false;
		}

		return it->second == HEXSIDES_NONE;
	}

	bool TileBoard::isAllowed(const Hexi& pos, const HexSides& sides) const {
		if (!isInside(pos)) {
			return false;
		}

		if (!isEmpty(pos)) {
			return false;
		}

		return isNeighborsMatching(pos, sides);
	}

    bool TileBoard::isNeighborsMatching(const Hexi& pos, const HexSides& sides) const {
        for (int i = 0; i < 6; ++i) {
            Hexi neighborPos = CUBE_DIRECTIONS[i] + pos;
			HexSides neighbor = getTile(neighborPos);

            int oppositeSide = (i + 3) % 6;
            HexSide neighborSide = neighbor[oppositeSide];
            if (neighborSide == HexSide::NONE) {
                continue;
            }

            if (sides[i] != neighborSide) {
                return false;
            }
        }
        return true;
	}

	HexSides TileBoard::getTile(Hexi hex) const {
		auto it = hexes_.find(hex);
		if (it == hexes_.end()) {
			return HEXSIDES_NONE;
		}
		return it->second;
	}

    void TileBoard::clear() {
        for (auto& hexTile : hexes_) {
			hexTile.second = HEXSIDES_NONE;
        }
	}

	bool TileBoard::put(const Hexi& pos, const HexSides& sides) {
		if (!isAllowed(pos, sides)) {
			return false;
		}
		hexes_[pos] = sides;
		return true;
	}

} // Namespace vin::hex.
