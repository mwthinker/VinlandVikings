#ifndef VINLANDVIKINGS_VIN_DECK_H
#define VINLANDVIKINGS_VIN_DECK_H

#include "hex/tilekey.h"
#include "tilesgraphic.h"
#include "hex/tileboard.h"
#include "spritetile.h"

#include <vector>
#include <spdlog/spdlog.h>

namespace vin {

	class Deck {
	public:
		using TileInvariantKeys = std::vector<hex::TileInvariantKey>;
		using const_iterator = TileInvariantKeys::const_iterator;

		void push(const hex::TileInvariantKey& tile, int nbr = 1) {
			for (int i = 0; i < nbr; ++i) {
				tiles_.push_back(tile);
			}
		}

		const_iterator begin() const {
			return tiles_.begin();
		}

		const_iterator end() const {
			return tiles_.end();
		}

	private:
		TileInvariantKeys tiles_;
	};

}

#endif
