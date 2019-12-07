#ifndef VINLANDVIKINGS_VIN_TILEDECK_H
#define VINLANDVIKINGS_VIN_TILEDECK_H

#include "tileboard.h"
#include "../random.h"
#include "../logger.h"
#include "hexagon.h"

#include <vector>
#include <algorithm>

namespace vin::hex {
	
	struct HexMapConfig {

	};

	class MapGenerator {
	public:
		MapGenerator();

		void fill(hex::TileBoard& map, std::vector<hex::HexSides> tiles, hex::Hexi start);

	private:
		template <class T>
		void shuffle(std::vector<T>& vector) {
			std::shuffle(vector.begin(), vector.end(), engine_);
		}

		Random random_;
		std::mt19937 engine_{std::random_device{}()};
	};

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_TILEDECK_H
