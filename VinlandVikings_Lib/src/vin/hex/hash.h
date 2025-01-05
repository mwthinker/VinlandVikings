#ifndef VINLANDVIKINGS_VIN_HEX_HASH_H
#define VINLANDVIKINGS_VIN_HEX_HASH_H

#include "hex.h"
#include "tilekey.h"

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
	struct hash<vin::hex::TileKey> {
		inline size_t operator()(vin::hex::TileKey key) const {
			return hash<uint64_t>()(static_cast<uint64_t>(key));
		}
	};

	template <>
	struct hash<vin::hex::TileInvariantKey> {
		inline size_t operator()(vin::hex::TileInvariantKey key) const {
			return hash<uint64_t>()(static_cast<uint64_t>(key));
		}
	};

}

#endif
