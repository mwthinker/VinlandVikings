#ifndef VINLANDVIKINGS_VIN_TILEDECK_H
#define VINLANDVIKINGS_VIN_TILEDECK_H

#include "tile.h"
#include "tileboard.h"
#include "../random.h"
#include "../logger.h"
#include "hexagon.h"

#include <vector>
#include <algorithm>

namespace vin::hex {
	
	struct HexMapConfig {

	};

	std::vector<hex::Hexi> findAllEmptyNeighbors(const hex::TileBoard& tiles);

	class MapGenerator {
	public:
		MapGenerator();

		void fill(hex::TileBoard& map, std::vector<hex::HexSides> tiles, hex::Hexi start);

	private:
		void fill(hex::TileBoard& map, const hex::Tile& tile, hex::Hexi start);

		bool put(hex::TileBoard& map, const hex::Tile& tile) {

		}

		Random random_;
		std::mt19937 engine_{std::random_device{}()};
	};

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_TILEDECK_H
