#ifndef VINLANDVIKINGS_VIN_HEXSIDESKEY_H
#define VINLANDVIKINGS_VIN_HEXSIDESKEY_H

#include "hexagon.h"

#include <cstdint>

namespace vin {

	class HexSidesKey {
	public:
		HexSidesKey() = default;

		HexSidesKey(const HexSidesKey&) = default;

		HexSidesKey& operator=(const HexSidesKey&) = default;

		// Assumes one side is not bigger than one byte.
		HexSidesKey(const HexSides& sides);

		bool operator==(const HexSidesKey& key) const;

		bool operator<(const HexSidesKey& key) const;

	private:
		uint_fast64_t key_;
	};

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_HEXSIDESKEY_H
