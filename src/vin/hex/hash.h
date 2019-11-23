#ifndef VINLANDVIKINGS_VIN_HEX_HASH_H
#define VINLANDVIKINGS_VIN_HEX_HASH_H

#include "hex.h"

#include <functional>

namespace std {

	template <>
	struct hash<vin::hex::Hexi> {
		inline size_t operator()(const vin::hex::Hexi& h) const {
			hash<int> int_hash;
			size_t hq = int_hash(h.q());
			size_t hr = int_hash(h.r());
			return hq ^ (hr + 0x9e3779b9 + (hq << 6) + (hq >> 2));
		}
	};

} // Namespace std.


#endif // VINLANDVIKINGS_VIN_HEX_HASH_H
