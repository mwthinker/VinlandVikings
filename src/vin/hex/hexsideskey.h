#ifndef VINLANDVIKINGS_VIN_HEX_HEXSIDESKEY_H
#define VINLANDVIKINGS_VIN_HEX_HEXSIDESKEY_H

#include "hexsides.h"

#include <cstdint>
#include <algorithm>

namespace vin::hex {

	class HexSidesKey {
	public:
		constexpr HexSidesKey() noexcept = default;

		constexpr HexSidesKey(const HexSidesKey&) noexcept = default;

		constexpr HexSidesKey& operator=(const HexSidesKey&) noexcept = default;

		constexpr HexSidesKey(HexSidesKey&&) noexcept = default;

		constexpr HexSidesKey& operator=(HexSidesKey&&) noexcept = default;

		constexpr HexSidesKey(const HexSides& sides) noexcept;

		constexpr bool operator<(const HexSidesKey& key) const noexcept;

		constexpr operator uint64_t() const;

	private:
		uint64_t key_{0};
	};

	class HexSidesInvariantKey {
	public:
		constexpr HexSidesInvariantKey() noexcept = default;

		constexpr HexSidesInvariantKey(const HexSidesInvariantKey&) noexcept = default;

		constexpr HexSidesInvariantKey& operator=(const HexSidesInvariantKey&) noexcept = default;

		constexpr HexSidesInvariantKey(HexSidesInvariantKey&&) noexcept = default;

		constexpr HexSidesInvariantKey& operator=(HexSidesInvariantKey&&) noexcept = default;

		constexpr operator uint64_t() const;

		HexSidesInvariantKey(HexSides sides) noexcept;

		bool operator<(const HexSidesInvariantKey& key) const noexcept;
	private:
		HexSidesKey key_;
	};

	// Assumes one side is not bigger than one byte.
	inline constexpr HexSidesKey::HexSidesKey(const HexSides& sides) noexcept :
		key_{((uint64_t) sides[0] << 8)
		+ ((uint64_t) sides[1] << 8 * 2)
		+ ((uint64_t) sides[2] << 8 * 3)
		+ ((uint64_t) sides[3] << 8 * 4)
		+ ((uint64_t) sides[4] << 8 * 5)
		+ ((uint64_t) sides[5] << 8 * 6)} {
	}

	inline constexpr bool HexSidesKey::operator<(const HexSidesKey& key) const noexcept {
		return key_ < key.key_;
	}

	inline constexpr HexSidesKey::operator uint64_t() const {
		return key_;
	}

	inline HexSidesInvariantKey::HexSidesInvariantKey(HexSides sides) noexcept {
		std::array<HexSidesKey, 6> keys{
			sides,
			rotateCopy(sides, 1),
			rotateCopy(sides, 2),
			rotateCopy(sides, 3),
			rotateCopy(sides, 4),
			rotateCopy(sides, 5),
		};
		key_ = *std::min_element(keys.begin(), keys.end());
	}

	inline bool HexSidesInvariantKey::operator<(const HexSidesInvariantKey& invariantKey) const noexcept {
		return key_ < invariantKey.key_;
	}

	inline constexpr HexSidesInvariantKey::operator uint64_t() const {
		return key_;
	}

} // Namespace vin::hex.

#endif // VINLANDVIKINGS_VIN_HEX_HEXSIDESKEY_H
