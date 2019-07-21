#include "hexsideskey.h"

namespace vin {

	// Assumes one side is not bigger than one byte.
	HexSidesKey::HexSidesKey(const HexSides& sides)
		: key_(((int_least64_t)sides[0] << 8)
		+ ((int_least64_t)sides[1] << 8 * 2)
		+ ((int_least64_t)sides[2] << 8 * 3)
		+ ((int_least64_t)sides[3] << 8 * 4)
		+ ((int_least64_t)sides[4] << 8 * 5)
		+ ((int_least64_t)sides[5] << 8 * 6)) {
	}

	bool HexSidesKey::operator==(const HexSidesKey& key) const {
		return key_ == key.key_;
	}

	bool HexSidesKey::operator<(const HexSidesKey& key) const {
		return key_ < key.key_;
	}

} // Namespace vin.
