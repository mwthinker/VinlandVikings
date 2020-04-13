#include "tilelexicon.h"
#include "heximage.h"

namespace vin {
	
	void TileLexicon::add(const std::vector<HexImage>& hexImages) {
		for (const auto& hexImage : hexImages) {
			add(hexImage);
		}
	}

	void TileLexicon::add(const HexImage& hexImage) {
		invariantMap_[hexImage.getTile()].emplace_back(hexImage);
		for (int i = 0; i < 6; ++i) {
			HexSprite sprite{hexImage.getImage(), i};
			auto sides = rotateCopy(hexImage.getTile(), i);
			map_[sides].emplace_back(sprite, sides);
		}
	}

	SpriteTiles TileLexicon::getInvariantTiles(hex::TileInvariantKey key) const {
		auto it = invariantMap_.find(key);
		if (it == invariantMap_.end()) {
			return {};
		}
		return it->second;
	}

	SpriteTiles TileLexicon::getTiles(hex::TileKey key) const {
		auto it = map_.find(key);
		if (it == map_.end()) {
			return {};
		}
		return it->second;
	}

	void TileLexicon::clear() {
		map_.clear();
		invariantMap_.clear();
	}

} // Namespace vin.
