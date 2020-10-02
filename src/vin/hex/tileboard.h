#ifndef VINLANDVIKINGS_VIN_HEX_HEXTILEMAP_H
#define VINLANDVIKINGS_VIN_HEX_HEXTILEMAP_H

#include "hexagon.h"
#include "../types.h"
#include "tile.h"
#include "tilekey.h"
#include "hash.h"

#include <vector>

#include <unordered_map>

namespace vin::hex {

	class TileBoard;
	std::vector<hex::Hexi> findAllEmptyNeighbors(const hex::TileBoard& board, const std::vector<hex::Hexi>& tiles);

	class TileBoard {
	public:
		using Map = std::unordered_map<Hexi, Tile>;
		using const_iterator = Map::const_iterator;

		TileBoard();

		TileBoard(const std::vector<Hexi>& hexes);

		template <class iterator>
		TileBoard(const iterator& begin, const iterator& end) {
			for (auto it = begin; it != end; ++it) {
				Hexi hex = *it;
				hexes_[hex] = HEXSIDES_NONE;
			}
		}
		
		bool clear();

		bool remove(const Hexi& pos);

		bool put(const Hexi& pos, const Tile& tile);

		bool isInside(const Hexi& hex) const;

		bool isEmpty(const Hexi& hex) const;

		const_iterator begin() const {
			return hexes_.begin();
		}

		const_iterator end() const {
			return hexes_.end();
		}

		bool isNeighborsMatching(const Hexi& pos, const Tile& tile) const;

		bool isAllowed(const Hexi& pos, const Tile& tile) const;

		Tile getTile(Hexi hex) const;

	private:
		Map hexes_;
	};

}

#endif
