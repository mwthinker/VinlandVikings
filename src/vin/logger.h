#ifndef VINLANDVIKINGS_VIN_LOGGER_H
#define VINLANDVIKINGS_VIN_LOGGER_H

#include <sdl/logger.h>

namespace vin {
	
	static std::shared_ptr<spdlog::logger> logger() {
		return sdl::logger("vin");
	}

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_LOGGER_H
