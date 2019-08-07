#ifndef VINLANDVIKINGS_VIN_HEXTILEMAP_H
#define VINLANDVIKINGS_VIN_HEXTILEMAP_H

#include "hexagon.h"
#include "types.h"
#include "hexsides.h"
#include "hexsideskey.h"

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

	std::vector<Hexi> createRectangleShape(int columns, int rows);

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
			hex_ = tile.hex_;
			sides_ = tile.sides_;
			return *this;
		}

		HexTile& operator=(HexTile&&) = default;

        bool operator==(const HexTile& tile) const {
            return hex_ == tile.hex_ && sides_ == tile.sides_;
        }

        bool operator!=(const HexTile& tile) const {
            return hex_ != tile.hex_ || sides_ != tile.sides_;
        }

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
				hexes_[hex] = HexTile(hex, {HexSide::NONE});
			}
		}

		void clear();

		bool put(const HexTile& tile);

		bool isInside(const Hexi& hex) const;

		bool isEmpty(const Hexi& hex) const;

		const_iterator begin() const {
			return hexes_.begin();
		}

		const_iterator end() const {
			return hexes_.end();
		}

		bool isNeighborsMatching(HexTile hexTile) const;

		bool isAllowed(HexTile hexTile) const;

		HexTile getHexTile(Hexi hex) const;

	private:
		std::unordered_map<Hexi, HexTile> hexes_;
	};

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_HEXTILEMAP_H
