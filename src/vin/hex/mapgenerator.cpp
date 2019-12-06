#include "mapgenerator.h"

namespace vin::hex {
	
	std::vector<hex::Hexi> findAllEmptyNeighbors(const hex::TileBoard& board, const std::vector<hex::Hexi>& tiles) {
		std::vector<hex::Hexi> emptyNeighbors;
		for (const auto& pos : tiles) {
			for (const auto& dir : vin::hex::CUBE_DIRECTIONS) {
				auto neighbor = dir + pos;
				if (board.isEmpty(neighbor)) {
					emptyNeighbors.push_back(neighbor);
				}
			}
		}
		return emptyNeighbors;
	}

	MapGenerator::MapGenerator() {
	}

	namespace {

		bool put(hex::TileBoard& board, hex::Tile tile, hex::Hexi pos) {
			return true;
		}

	}

	void MapGenerator::fill(hex::TileBoard& board, std::vector<hex::HexSides> deck, hex::Hexi start) {
		std::shuffle(deck.begin(), deck.end(), engine_);

		if (deck.empty()) {
			logger()->warn("[HexMapGenerator] Failed to generate, no tiles provided");
			return;
		}

		if (!board.put(start, deck.back())) {
			logger()->warn("[HexMapGenerator] Failed to generate, start position not valid");
			return;
		}
		deck.pop_back();

		std::vector<hex::Hexi> tiles{start};

		auto nbrTiles = deck.size() + 1;
		while (nbrTiles > 0 && nbrTiles != deck.size()) {
			nbrTiles = deck.size();
			auto emptyNeighbors = findAllEmptyNeighbors(board, tiles);
			std::shuffle(emptyNeighbors.begin(), emptyNeighbors.end(), engine_);
			for (const auto& pos : emptyNeighbors) {
				for (auto& tile : deck) {
					for (int i = 0; i < 6; ++i) {
						rotate(tile, 1);
						if (board.put(pos, tile)) {
							deck.pop_back();
							tiles.push_back(pos);
							break;
						}
					}
				}
			}
		}
	}

	void MapGenerator::fill(hex::TileBoard& board, const hex::Tile& tile, hex::Hexi start) {
		
	}

} // Namespace vin.
