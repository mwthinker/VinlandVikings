#ifndef VINLANDVIKINGS_VIN_HEX_HASH_H
#define VINLANDVIKINGS_VIN_HEX_HASH_H

#include "hex.h"
#include "hexsideskey.h"

#include <functional>

namespace std {

	template <>
	struct hash<vin::hex::Hexi> {
		inline size_t operator()(const vin::hex::Hexi& h) const {
			hash<int> intHash;
			size_t hq = intHash(h.q());
			size_t hr = intHash(h.r());
			return hq ^ (hr + 0x9e3779b9 + (hq << 6) + (hq >> 2));
		}
	};	

	template <>
	struct hash<vin::hex::HexSidesKey> {
		inline size_t operator()(vin::hex::HexSidesKey key) const {
			hash<uint64_t> intHash;
			return intHash(key);
		}
	};

	template <>
	struct hash<vin::hex::HexSidesInvariantKey> {
		inline size_t operator()(vin::hex::HexSidesInvariantKey key) const {
			hash<uint64_t> intHash;
			return intHash(key);
		}
	};

} // Namespace std.

#endif // VINLANDVIKINGS_VIN_HEX_HASH_H
