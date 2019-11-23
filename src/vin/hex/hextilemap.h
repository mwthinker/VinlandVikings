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

	std::vector<Hexi> createFlatHexShape(int radiusNbr);

	std::vector<Hexi> createParallelogramShape(int columns, int rows);

	std::vector<Hexi> createRectangleShape(int columns, int rows);

	class HexTileMap {
	public:
		using const_iterator = std::unordered_map<Hexi, Tile>::const_iterator;

		HexTileMap();

        HexTileMap(const HexTileMap& map) = delete;
        HexTileMap& operator=(const HexTileMap& map) = delete;

		HexTileMap(HexTileMap&& map) noexcept;

		HexTileMap& operator=(HexTileMap&& map) noexcept;

		HexTileMap(const std::vector<Hexi>& hexes);

		template <class iterator>
		HexTileMap(const iterator& begin, const iterator& end) {
			for (auto it = begin; it != end; ++it) {
				Hexi hex = *it;
				hexes_[hex] = Tile{hex, HEXSIDES_NONE};
			}
		}

		void clear();

		bool put(const Tile& tile);

		bool isInside(const Hexi& hex) const;

		bool isEmpty(const Hexi& hex) const;

		const_iterator begin() const {
			return hexes_.begin();
		}

		const_iterator end() const {
			return hexes_.end();
		}

		bool isNeighborsMatching(Tile hexTile) const;

		bool isAllowed(Tile hexTile) const;

		Tile getTile(Hexi hex) const;

	private:
		std::unordered_map<Hexi, Tile> hexes_;
	};

} // Namespace vin::hex.

#endif // VINLANDVIKINGS_VIN_HEXTILEMAP_H
