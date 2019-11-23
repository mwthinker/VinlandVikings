#ifndef VINLANDVIKINGS_VIN_HEXDIMENSION_H
#define VINLANDVIKINGS_VIN_HEXDIMENSION_H

#include "types.h"

namespace vin {

	struct HexDimension {
		float innerSize = 0.95f;
		float outerSize = 1.f;
		float angle = PI / 3;
	};

} // Namespace vin.


#endif // VINLANDVIKINGS_VIN_HEXDIMENSION_H
