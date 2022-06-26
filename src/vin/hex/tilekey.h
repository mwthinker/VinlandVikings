#ifndef VINLANDVIKINGS_VIN_HEX_TILEKEY_H
#define VINLANDVIKINGS_VIN_HEX_TILEKEY_H

#include "tile.h"

#include <cstdint>
#include <algorithm>

namespace vin::hex {

	class TileKey {
	public:
		constexpr TileKey() = default;

		explicit constexpr TileKey(const Tile& tile);

		constexpr bool operator==(TileKey key) const;

		constexpr bool operator!=(TileKey key) const;

		constexpr bool operator<(TileKey key) const;

		explicit constexpr operator uint64_t() const;

	private:
		uint64_t key_{0};
	};

	class TileInvariantKey {
	public:
		constexpr TileInvariantKey() = default;

		explicit constexpr operator uint64_t() const;

		constexpr bool operator==(TileInvariantKey key) const;

		constexpr bool operator!=(TileInvariantKey key) const;

		explicit TileInvariantKey(Tile sides);

		constexpr bool operator<(TileInvariantKey key) const;

	private:
		TileKey key_;
	};

	// Assumes one side is not bigger than one byte.
	inline constexpr TileKey::TileKey(const Tile& sides) :
		key_{(static_cast<uint64_t>(sides[0]) << 8)
		+ (static_cast<uint64_t>(sides[1]) << 8 * 2)
		+ (static_cast<uint64_t>(sides[2]) << 8 * 3)
		+ (static_cast<uint64_t>(sides[3]) << 8 * 4)
		+ (static_cast<uint64_t>(sides[4]) << 8 * 5)
		+ (static_cast<uint64_t>(sides[5]) << 8 * 6)} {
	}

	inline constexpr bool TileKey::operator==(TileKey key) const {
		return key_ == key.key_;
	}

	inline constexpr bool TileKey::operator!=(TileKey key) const {
		return key_ != key.key_;
	}

	inline constexpr bool TileKey::operator<(TileKey key) const {
		return key_ < key.key_;
	}

	inline constexpr TileKey::operator uint64_t() const {
		return key_;
	}

	inline TileInvariantKey::TileInvariantKey(Tile sides) {
		std::array<TileKey, 6> keys{
			TileKey{sides},
			TileKey{rotateCopy(sides, 1)},
			TileKey{rotateCopy(sides, 2)},
			TileKey{rotateCopy(sides, 3)},
			TileKey{rotateCopy(sides, 4)},
			TileKey{rotateCopy(sides, 5)},
		};
		key_ = *std::min_element(keys.begin(), keys.end());
	}

	inline constexpr bool TileInvariantKey::operator<(TileInvariantKey invariantKey) const {
		return key_ < invariantKey.key_;
	}

	inline constexpr TileInvariantKey::operator uint64_t() const {
		return static_cast<uint64_t>(key_);
	}

	constexpr bool TileInvariantKey::operator==(TileInvariantKey key) const {
		return key_ == key.key_;
	}

	constexpr bool TileInvariantKey::operator!=(TileInvariantKey key) const {
		return key_ != key.key_;
	}

}

#endif
