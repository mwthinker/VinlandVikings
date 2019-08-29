#include "vinlandwindow.h"
#include "logger.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h> // or "../stdout_sinks.h" if no colors needed
#include <spdlog/sinks/basic_file_sink.h>

#include <sdl/initsdl.h>

#include <clara.hpp>

#include <iostream>

struct TerminalConfig {
	int width = 400;
	int height = 400;
	bool showHelp = false;
	bool showWarnings = false;
	bool showErrors = false;
	bool showDebugInfo = false;
};

int main(int argc, char** argv) {
	//auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	//console_sink->set_color(spdlog::level::level_enum::info, 0xffff0000);
	
	//auto console = spdlog::stdout_color_mt("MW");
	//spdlog::register_logger(console);
	//spdlog::get("MW");
		
	//spdlog::set_default_logger(console);
	//spdlog::set_automatic_registration(true);
	//spdlog::create (console_sink, "logger_name", "daylylog", 11, 59);
	
	TerminalConfig config;

	auto cli
		= clara::Opt(config.width, "width")
		["-W"]["--width"]
		("window width") |
		clara::Opt(config.width, "height")
		["-H"]["--height"]
		("window height") |
		clara::Opt(config.showWarnings)
		["-w"]["--warnings"]
		( "show warnings in terminal") |
		clara::Opt(config.showWarnings)
		["-e"]["--errors"]
		("show errors in terminal") |
		clara::Opt(config.showWarnings)
		["-d"]["--debugs"]
		("show debug info in terminal") |
		clara::Help(config.showHelp);

	auto result = cli.parse(clara::Args(argc, argv));
	if (!result) {
		std::cerr << "Error in command line: " << result.errorMessage() << std::endl;
		exit(1);
	}

	if (config.showHelp) {
		std::cerr << cli << std::endl;
		return 0;
	}

	try {
		const sdl::InitSdl SDL;
		vin::VinlandWindow vinland;
		vinland.startLoop();
		return 0;
	} catch (std::exception& ex) {
		vin::logger()->error("[Main] Fatal error has occured: {}", ex.what());
		return 1;
	}
}
