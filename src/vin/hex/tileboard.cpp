#include "tileboard.h"
#include "../types.h"
#include "tilekey.h"

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

	bool TileBoard::isAllowed(const Hexi& pos, const Tile& tile) const {
		if (!isInside(pos)) {
			return false;
		}

		if (!isEmpty(pos)) {
			return false;
		}

		return isNeighborsMatching(pos, tile);
	}

    bool TileBoard::isNeighborsMatching(const Hexi& pos, const Tile& tile) const {
        for (int i = 0; i < 6; ++i) {
            Hexi neighborPos = CUBE_DIRECTIONS[i] + pos;
			Tile neighbor = getTile(neighborPos);

            int oppositeSide = (i + 3) % 6;
            HexSide neighborSide = neighbor[oppositeSide];
            if (neighborSide == HexSide::NONE) {
                continue;
            }

            if (tile[i] != neighborSide) {
                return false;
            }
        }
        return true;
	}

	Tile TileBoard::getTile(Hexi hex) const {
		auto it = hexes_.find(hex);
		if (it == hexes_.end()) {
			return HEXSIDES_NONE;
		}
		return it->second;
	}

    bool TileBoard::clear() {
		bool dirty = false;
        for (auto& hexTile : hexes_) {
			if (!dirty && hexTile.second != HEXSIDES_NONE) {
				dirty = true;
			}
			hexTile.second = HEXSIDES_NONE;
        }
		return dirty;
	}

	bool TileBoard::put(const Hexi& pos, const Tile& tile) {
		if (!isAllowed(pos, tile)) {
			return false;
		}
		bool dirty = hexes_[pos] != tile;
		hexes_[pos] = tile;
		return dirty;
	}

	bool TileBoard::remove(const Hexi& pos) {
		if (auto it = hexes_.find(pos); it != hexes_.end()) {
			if (it->second != HEXSIDES_NONE) {
				it->second = HEXSIDES_NONE;
				return true;
			}
			return false;
		}
		return false;
	}

} // Namespace vin::hex.
