#include "vinlandwindow.h"
#include "logger.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h> // or "../stdout_sinks.h" if no colors needed
#include <spdlog/sinks/basic_file_sink.h>

#include <sdl/initsdl.h>

int main(int, char**) {
	//auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	//console_sink->set_color(spdlog::level::level_enum::info, 0xffff0000);
	
	//auto console = spdlog::stdout_color_mt("MW");
	//spdlog::register_logger(console);
	//spdlog::get("MW");
		
	//spdlog::set_default_logger(console);
	//spdlog::set_automatic_registration(true);
	//spdlog::create (console_sink, "logger_name", "daylylog", 11, 59);

	try {
		const sdl::InitSdl SDL;
		vin::VinlandWindow vinland;
		vinland.startLoop();
	} catch (std::exception& ex) {
		vin::logger()->error("[Main] Fatal error has occured: {}", ex.what());
	}

	return 0;
}
