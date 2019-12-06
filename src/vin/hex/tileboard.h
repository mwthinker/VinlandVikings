#ifndef VINLANDVIKINGS_VIN_HEXTILEMAP_H
#define VINLANDVIKINGS_VIN_HEXTILEMAP_H

#include "hexagon.h"
#include "../types.h"
#include "hexsides.h"
#include "hexsideskey.h"
#include "hash.h"
#include "tile.h"

#include <vector>

#include <unordered_map>

namespace vin::hex {

	std::vector<Hexi> createHexShape(int radiusNbr);

	std::vector<Hexi> createParallelogramShape(int columns, int rows);

	std::vector<Hexi> createRectangleShape(int columns, int rows);

	class TileBoard {
	public:
		using Map = std::unordered_map<Hexi, HexSides>;
		using const_iterator = Map::const_iterator;

		TileBoard();

		TileBoard(const TileBoard& map) = delete;
		TileBoard& operator=(const TileBoard& map) = delete;

		TileBoard(TileBoard&& map) noexcept;

		TileBoard& operator=(TileBoard&& map) noexcept;

		TileBoard(const std::vector<Hexi>& hexes);

		template <class iterator>
		TileBoard(const iterator& begin, const iterator& end) {
			for (auto it = begin; it != end; ++it) {
				Hexi hex = *it;
				hexes_[hex] = HEXSIDES_NONE;
			}
		}

		void clear();

		bool put(const Hexi& pos, const HexSides& sides);

		bool isInside(const Hexi& hex) const;

		bool isEmpty(const Hexi& hex) const;

		const_iterator begin() const {
			return hexes_.begin();
		}

		const_iterator end() const {
			return hexes_.end();
		}

		bool isNeighborsMatching(const Hexi& pos, const HexSides& sides) const;

		bool isAllowed(const Hexi& pos, const HexSides& sides) const;

		HexSides getTile(Hexi hex) const;

	private:
		Map hexes_;
	};

} // Namespace vin::hex.

#endif // VINLANDVIKINGS_VIN_HEXTILEMAP_H
