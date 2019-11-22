#ifndef VINLANDVIKINGS_VIN_HEXSIDESKEY_H
#define VINLANDVIKINGS_VIN_HEXSIDESKEY_H

#include "hexsides.h"

#include <cstdint>

namespace vin::hex {

	// Unique key for HexSides.
	class HexSidesKey {
	public:
		constexpr HexSidesKey() noexcept = default;

		constexpr HexSidesKey(const HexSidesKey&) noexcept = default;

		constexpr HexSidesKey& operator=(const HexSidesKey&) noexcept = default;

		constexpr HexSidesKey(HexSidesKey&&) noexcept = default;

		constexpr HexSidesKey& operator=(HexSidesKey&&) noexcept = default;

		constexpr HexSidesKey(const HexSides& sides) noexcept;

		constexpr bool HexSidesKey::operator<(const HexSidesKey& key) const noexcept;

	private:
		uint_fast64_t key_{0};
	};

	// Assumes one side is not bigger than one byte.
	inline constexpr HexSidesKey::HexSidesKey(const HexSides& sides) noexcept :
		key_(((int_least64_t) sides[0] << 8)
		+ ((int_least64_t) sides[1] << 8 * 2)
		+ ((int_least64_t) sides[2] << 8 * 3)
		+ ((int_least64_t) sides[3] << 8 * 4)
		+ ((int_least64_t) sides[4] << 8 * 5)
		+ ((int_least64_t) sides[5] << 8 * 6)) {
	}

	inline constexpr bool HexSidesKey::operator<(const HexSidesKey& key) const noexcept {
		return key_ < key.key_;
	}

} // Namespace vin::hex.

#endif // VINLANDVIKINGS_VIN_HEXSIDESKEY_H
