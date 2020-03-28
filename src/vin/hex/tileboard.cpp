#include "tileboard.h"
#include "../types.h"
#include "hexsideskey.h"

namespace vin::hex {	

	std::vector<Hexi> findAllEmptyNeighbors(const TileBoard& board, const std::vector<Hexi>& tiles) {
		std::vector<Hexi> emptyNeighbors;
		for (const auto& pos : tiles) {
			for (const auto& dir : CUBE_DIRECTIONS) {
				auto neighbor = dir + pos;
				if (board.isEmpty(neighbor)) {
					emptyNeighbors.push_back(neighbor);
				}
			}
		}
		return emptyNeighbors;
	}

	TileBoard::TileBoard() {
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

	void TileBoard::remove(const Hexi& pos) {
		if (auto it = hexes_.find(pos); it != hexes_.end()) {
			it->second = HEXSIDES_NONE;
		}
	}

} // Namespace vin::hex.
