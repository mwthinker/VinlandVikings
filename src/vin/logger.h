#ifndef VINLANDVIKINGS_VIN_LOGGER_H
#define VINLANDVIKINGS_VIN_LOGGER_H

#include <spdlog/spdlog.h>

namespace vin::logger {
	
	bool init(const std::string& folderPath = "");

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_LOGGER_H
