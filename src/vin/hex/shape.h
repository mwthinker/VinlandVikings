#ifndef VINLANDVIKINGS_VIN_HEX_SHAPE_H
#define VINLANDVIKINGS_VIN_HEX_SHAPE_H

#include "hex.h"

#include <vector>

namespace vin::hex::shape {

	std::vector<Hexi> createHex(int radiusNbr);

	std::vector<Hexi> createParallelogram(int columns, int rows);

	std::vector<Hexi> createRectangle(int columns, int rows);

} // Namespace vin::hex.

#endif // VINLANDVIKINGS_VIN_HEX_SHAPE_H
