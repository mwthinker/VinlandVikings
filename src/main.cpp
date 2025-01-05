#include <vin/vinlandwindow.h>
#include <vin/logger.h>
#include <vin/configuration.h>

#include <sdl/initsdl.h>
#include <fmt/printf.h>
#include <lyra/lyra.hpp>

#include <fmt/ostream.h>

#ifdef WIN32

#pragma warning(push)
#pragma warning( disable: 4005 )
#include <Windows.h>
#pragma warning(pop)

#endif

struct TerminalConfig {
	int width = 400;
	int height = 400;
	bool showHelp = false;
	spdlog::level::level_enum logLevel = spdlog::level::trace;
	std::string filename = "imageTiles/hexdata.json";
	std::string logPath;
};

namespace {

#ifdef WIN32

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
			spdlog::debug("My console, hide console");
			if (FreeConsole() != 0) {
				spdlog::error("FreeConsole(): {}", getLastError());
			}
		} else {
			spdlog::debug("Not my console, do not hide console!");
		}
	}

#else

	void hideMyConsole() {
		// Todo!
	}

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

template <> struct fmt::formatter<lyra::cli> : ostream_formatter {};

int main(int argc, char** argv) {
	TerminalConfig config;
	config.logPath = SDL_GetPrefPath("mwthinker", "VinlandVikings");

	try {
		auto cli
			= lyra::opt(config.filename, "filename")
			["-f"]["--filename"]
			("config filename") |
			lyra::opt(config.width, "width")
			["-W"]["--width"]
			("window width") |
			lyra::opt(config.width, "height")
			["-H"]["--height"]
			("window height") |
			lyra::opt(config.logLevel, "trace|debug|info|warning|err|critical|off")
			["-L"]["--log_level"]
			("log level") |
			lyra::opt(config.logPath, "log path")
			["-P"]["--log_path"]
			("path to log folder") |
			lyra::help(config.showHelp);

		auto result = cli.parse(lyra::args(argc, argv));
		if (!result) {
			fmt::print(std::cerr, "Error in command line: {}\n", result.message());
			return 1;
		}

		if (config.showHelp) {
			fmt::print("{}\n", cli);
			return 0;
		}
	} catch (const std::exception& ex) {
		fmt::print("Incorrect commmand line: {}\n", ex.what());
		return 1;
	}

	if (vin::logger::init(config.logPath)) {
		hideMyConsole();
	}
	spdlog::default_logger()->set_level(config.logLevel);
	spdlog::info("[Main] VinlandVikings Version:  {}", PROJECT_VERSION);
	spdlog::info("[Main] VinlandVikings Git Hash:  {}", GIT_VERSION);

	auto& hexData = vin::Configuration::getInstance();
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
	return 0;
}
