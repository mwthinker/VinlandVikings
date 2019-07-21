#ifndef VINLANDVIKINGS_VIN__HEXTILEMAP_H
#define VINLANDVIKINGS_VIN__HEXTILEMAP_H

#include "hexagon.h"
#include "types.h"

#include <vector>

#include <unordered_map>

namespace std {
	template <> struct hash<vin::Hexi> {
		size_t operator()(const vin::Hexi& h) const {
			hash<int> int_hash;
			size_t hq = int_hash(h.q());
			size_t hr = int_hash(h.r());
			return hq ^ (hr + 0x9e3779b9 + (hq << 6) + (hq >> 2));
		}
	};
}

namespace vin {

	std::vector<Hexi> createFlatHexShape(int radiusNbr);

	std::vector<Hexi> createParallelogramShape(int columns, int rows);

	class HexTile {
	public:
		HexTile() {
		}

		HexTile(const Hexi& hex, const HexSides& sides)
			: hex_(hex), sides_(sides) {
		}

		HexTile(const HexTile&) = default;
		HexTile(HexTile&&) = default;

		HexTile& operator=(const HexTile& tile) {
			//hex_ = tile.hex_;
			sides_ = tile.sides_;
		}

		HexTile& operator=(HexTile&&) = default;

		const Hexi& getHexi() const {
			return hex_;
		}

		const HexSides& getHexSides() const {
			return sides_;
		}

	private:
		Hexi hex_;
		HexSides sides_;
	};

	class HexTileMap {
	public:
		using const_iterator = std::unordered_map<Hexi, HexTile>::const_iterator;

		HexTileMap() {
		}

		HexTileMap(HexTileMap&& map) noexcept : hexes_(std::move(map.hexes_)) {
		}

		HexTileMap& operator=(HexTileMap&& map) noexcept {
			hexes_ = std::move(map.hexes_);
			return *this;
		}

		HexTileMap(const std::vector<Hexi>& hexes) {
			for (const auto& hex : hexes) {
				
				hexes_.insert({hex, HexTile(hex, {HexSide::NONE})});
			}
		}

		template <class iterator>
		HexTileMap(const iterator& begin, const iterator& end) {
			for (auto it = begin; it != end; ++it) {
				Hexi hex = *it;
				hexes_[hex] = HexTile(hex, {HexSide::NONE});
			}
		}

		bool isInside(Hexi hex) const;

		bool isEmpty(const Hexi& hex);

		const_iterator begin() const {
			return hexes_.begin();
		}

		const_iterator end() const {
			return hexes_.end();
		}

	private:
		std::unordered_map<Hexi, HexTile> hexes_;
	};

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN__HEXTILEMAP_H
