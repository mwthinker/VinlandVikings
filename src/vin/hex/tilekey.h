#ifndef VINLANDVIKINGS_VIN_HEX_HEXSIDESKEY_H
#define VINLANDVIKINGS_VIN_HEX_HEXSIDESKEY_H

#include "tile.h"

#include <cstdint>
#include <algorithm>

namespace vin::hex {

	class TileKey {
	public:
		constexpr TileKey() noexcept = default;

		constexpr TileKey(const TileKey&) noexcept = default;

		constexpr TileKey& operator=(const TileKey&) noexcept = default;

		constexpr TileKey(TileKey&&) noexcept = default;

		constexpr TileKey& operator=(TileKey&&) noexcept = default;

		constexpr TileKey(const Tile& tile) noexcept;

		constexpr bool operator<(const TileKey& key) const noexcept;

		constexpr operator uint64_t() const;

	private:
		uint64_t key_{0};
	};

	class TileInvariantKey {
	public:
		constexpr TileInvariantKey() noexcept = default;

		constexpr TileInvariantKey(const TileInvariantKey&) noexcept = default;

		constexpr TileInvariantKey& operator=(const TileInvariantKey&) noexcept = default;

		constexpr TileInvariantKey(TileInvariantKey&&) noexcept = default;

		constexpr TileInvariantKey& operator=(TileInvariantKey&&) noexcept = default;

		constexpr operator uint64_t() const;

		TileInvariantKey(Tile sides) noexcept;

		bool operator<(const TileInvariantKey& key) const noexcept;
	private:
		TileKey key_;
	};

	// Assumes one side is not bigger than one byte.
	inline constexpr TileKey::TileKey(const Tile& sides) noexcept :
		key_{((uint64_t) sides[0] << 8)
		+ ((uint64_t) sides[1] << 8 * 2)
		+ ((uint64_t) sides[2] << 8 * 3)
		+ ((uint64_t) sides[3] << 8 * 4)
		+ ((uint64_t) sides[4] << 8 * 5)
		+ ((uint64_t) sides[5] << 8 * 6)} {
	}

	inline constexpr bool TileKey::operator<(const TileKey& key) const noexcept {
		return key_ < key.key_;
	}

	inline constexpr TileKey::operator uint64_t() const {
		return key_;
	}

	inline TileInvariantKey::TileInvariantKey(Tile sides) noexcept {
		std::array<TileKey, 6> keys{
			sides,
			rotateCopy(sides, 1),
			rotateCopy(sides, 2),
			rotateCopy(sides, 3),
			rotateCopy(sides, 4),
			rotateCopy(sides, 5),
		};
		key_ = *std::min_element(keys.begin(), keys.end());
	}

	inline bool TileInvariantKey::operator<(const TileInvariantKey& invariantKey) const noexcept {
		return key_ < invariantKey.key_;
	}

	inline constexpr TileInvariantKey::operator uint64_t() const {
		return key_;
	}

} // Namespace vin::hex.

#endif // VINLANDVIKINGS_VIN_HEX_HEXSIDESKEY_H
