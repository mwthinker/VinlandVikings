#ifndef VINLANDVIKINGS_VIN_TILELEXICON_H
#define VINLANDVIKINGS_VIN_TILELEXICON_H

#include "tilelexicon.h"
#include "spritetile.h"
#include "hex/hash.h"

#include <vector>
#include <unordered_map>

namespace vin {

	using SpriteTiles = std::vector<SpriteTile>;

	class TileLexicon {
	public:
		TileLexicon() = default;

		void add(const std::vector<HexImage>& hexImages);

		void add(const HexImage& hexImage);

		SpriteTiles getInvariantTiles(hex::TileInvariantKey key) const;

		SpriteTiles getTiles(hex::TileKey key) const;

		void clear();

	private:
		using HexImageMap = std::unordered_map<hex::TileKey, SpriteTiles>;
		using HexImageInvariantMap = std::unordered_map<hex::TileInvariantKey, SpriteTiles>;

		HexImageMap map_;
		HexImageInvariantMap invariantMap_;
	};

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_TILELEXICON_H
