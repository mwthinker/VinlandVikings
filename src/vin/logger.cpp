#include "logger.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>

#include <fmt/printf.h>
#include <fstream>
#include <filesystem>

namespace vin::logger {
	
	namespace {

		const std::string LOG_FILE = "logs/log.txt";

		void createLogFolder(const std::string& folderPath) {
			std::string logPath = "logs";
			if (!folderPath.empty()) {
				logPath = folderPath + "/logs";
			}
			if (std::filesystem::create_directory(logPath)) {
				fmt::print("Log folder created: {}\n", logPath);
			}
		}

	}

	bool init(const std::string& folderPath) {
		try {
			createLogFolder(folderPath);
			auto rotatingSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(folderPath + LOG_FILE, 1024 * 1024, 10);
			auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
			
			auto logger = std::make_shared<spdlog::logger>("vin", spdlog::sinks_init_list({consoleSink, rotatingSink}));
			logger->set_level(spdlog::level::trace);
			spdlog::set_default_logger(logger);
			return true;
		} catch (const spdlog::spdlog_ex& ex) {
			fmt::print("Log initialization failed: {}\n", ex.what());
			return false;
		}
	}

}
