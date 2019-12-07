#ifndef VINLANDVIKINGS_VIN_TILELEXICON_H
#define VINLANDVIKINGS_VIN_TILELEXICON_H

#include "tilelexicon.h"
#include "tile.h"
#include "hex/hash.h"

#include <vector>
#include <unordered_map>

namespace vin {

	using Tiles = std::vector<Tile>;

	class TileLexicon {
	public:
		TileLexicon() = default;

		void add(const std::vector<HexImage>& hexImages);

		void add(const HexImage& hexImage);

		Tiles getInvariantTiles(hex::HexSidesInvariantKey key) const;

		Tiles getTiles(hex::HexSidesKey key) const;

	private:
		using HexImageMap = std::unordered_map<hex::HexSidesKey, Tiles>;
		using HexImageInvariantMap = std::unordered_map<hex::HexSidesInvariantKey, Tiles>;

		HexImageMap map_;
		HexImageInvariantMap invariantMap_;
	};

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_TILELEXICON_H
