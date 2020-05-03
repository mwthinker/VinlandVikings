#include "mapgenerator.h"

namespace vin::hex {

	MapGenerator::MapGenerator() {
	}

	void MapGenerator::fill(hex::TileBoard& board, std::vector<hex::Tile> deck, hex::Hexi start) {
		shuffle(deck);

		if (deck.empty()) {
			spdlog::warn("[HexMapGenerator] Failed to generate, no tiles provided");
			return;
		}

		if (!board.put(start, deck.back())) {
			spdlog::warn("[HexMapGenerator] Failed to generate, start position not valid");
			return;
		}
		deck.pop_back();

		std::vector<hex::Hexi> tiles{start};

		auto nbrTiles = deck.size() + 1;
		while (nbrTiles > 0 && nbrTiles != deck.size()) {
			nbrTiles = deck.size();
			auto emptyNeighbors = findAllEmptyNeighbors(board, tiles);
			shuffle(emptyNeighbors);
			for (const auto& pos : emptyNeighbors) {
				auto it = std::remove_if(deck.begin(), deck.end(), [&](hex::Tile tile) {
					for (int i = 0; i < 6; ++i) {
						rotate(tile, 1);
						if (board.put(pos, tile)) {
							tiles.push_back(pos);
							return true;
						}
					}
					return false;
				});
				deck.erase(it, deck.end());
			}
		}
	}

} // Namespace vin.
