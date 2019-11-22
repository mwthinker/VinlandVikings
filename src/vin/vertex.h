#ifndef VINLANDVIKINGS_VIN_VERTEX_H
#define VINLANDVIKINGS_VIN_VERTEX_H

#include "types.h"

namespace vin {
	
	struct Vertex {
		Vec2 pos{0, 0};
		Vec2 tex{0, 0};
		Color color{WHITE};
	};

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_VERTEX_H
