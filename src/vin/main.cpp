#include "vinlandwindow.h"
#include "logger.h"
#include "hexdata.h"

#include <sdl/initsdl.h>
#include <fmt/printf.h>
#include <clara.hpp>

#include <iostream>

struct TerminalConfig {
	int width = 400;
	int height = 400;
	bool showHelp = false;
	spdlog::level::level_enum logLevel = spdlog::level::trace;
	std::string filename = "imageTiles/hexdata.json";
	std::string logPath;
};

namespace {

#if WIN32

	std::string getLastError() {
		LPSTR lpMsgBuf;
		DWORD dw = GetLastError();

		auto size = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr,
			dw,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &lpMsgBuf,
			0, nullptr);

		std::string message(lpMsgBuf, size);
		LocalFree(lpMsgBuf);
		return message;
	}

	bool isMyConsole() {
		auto consoleWnd = GetConsoleWindow();
		DWORD dwProcessId;
		GetWindowThreadProcessId(consoleWnd, &dwProcessId);
		return GetCurrentProcessId() == dwProcessId;
	}

	void hideMyConsole() {
		if (isMyConsole()) {
			spdlog::info("My console, hide console");
			if (FreeConsole() != 0) {
				spdlog::error("FreeConsole(): {}", getLastError());
			}
		} else {
			spdlog::info("Not my console, do not hide console!");
		}
	}

#else

void hideMyConsole();

#endif

	std::istream& operator>>(std::istream& stream, spdlog::level::level_enum& level) {
		std::string token;
		stream >> token;
		if (token == "trace") level = spdlog::level::level_enum::trace;
		else if (token == "debug") level = spdlog::level::level_enum::debug;
		else if (token == "info") level = spdlog::level::level_enum::info;
		else if (token == "warn") level = spdlog::level::level_enum::warn;
		else if (token == "err") level = spdlog::level::level_enum::err;
		else if (token == "critical") level = spdlog::level::level_enum::critical;
		else if (token == "off") level = spdlog::level::level_enum::off;
		else {
			throw std::runtime_error("Invalid verbosity level value");
		}
		return stream;
	}

}

int main(int argc, char** argv) {
	spdlog::info("Application started");
	
	TerminalConfig config;
	config.logPath = SDL_GetPrefPath("mwthinker", "VinlandVikings");

	try {
		auto cli
			= clara::Opt(config.filename, "filename")
			["-f"]["--filename"]
			("config filename") |
			clara::Opt(config.width, "width")
			["-W"]["--width"]
			("window width") |
			clara::Opt(config.width, "height")
			["-H"]["--height"]
			("window height") |
			clara::Opt(config.logLevel, "trace|debug|info|warning|err|critical|off")
			["-L"]["--log_level"]
			("log level") |
			clara::Opt(config.logPath, "log path")
			["-P"]["--log_path"]
			("path to log folder") |
			clara::Help(config.showHelp);

		auto result = cli.parse(clara::Args(argc, argv));
		if (!result) {
			fmt::print(std::cerr, "Error in command line: {}\n", result.errorMessage());
			return 1;
		}

		if (config.showHelp) {
			fmt::print("{}\n", cli);
			return 0;
		}
	} catch (const std::runtime_error& ex) {
		fmt::print("Incorrect commmand line: {}\n", ex.what());
		return 1;
	}

	if (vin::logger::init(config.logPath)) {
		hideMyConsole();
	}

	spdlog::default_logger()->set_level(config.logLevel);

	auto& hexData = vin::HexData::getInstance();
	hexData.load(config.filename);

	try {
		const sdl::InitSdl SDL;
		vin::VinlandWindow vinland;
		vinland.startLoop();
		spdlog::info("[Main] Program shutdown successfully.");
		return 0;
	} catch (const std::exception& ex) {
		spdlog::error("[Main] Fatal error has occured: {}", ex.what());
		return 1;
	}
}
