#ifndef VINLANDVIKINGS_VIN_HEX_TILEBOARD_H
#define VINLANDVIKINGS_VIN_HEX_TILEBOARD_H

#include "hexagon.h"
#include "../types.h"
#include "tile.h"
#include "tilekey.h"
#include "hash.h"

#include <vector>
#include <concepts>

//#include <unordered_map>
#include <map>

namespace vin::hex {

	class TileBoard;
	std::vector<hex::Hexi> findAllEmptyNeighbors(const hex::TileBoard& board, const std::vector<hex::Hexi>& tiles);

	class TileBoard {
	public:
		//using Map = std::unordered_map<Hexi, Tile>;
		using Map = std::map<Hexi, Tile, HexComparator>;
		using const_iterator = Map::const_iterator;

		TileBoard();

		TileBoard(const std::vector<Hexi>& hexes);
		
		TileBoard(std::input_iterator auto begin, std::input_iterator auto end) {
			for (auto it = begin; it != end; ++it) {
				Hexi hex = *it;
				hexes_[hex] = HexSidesNone;
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
