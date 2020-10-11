#ifndef VINLANDVIKINGS_VIN_CONFIGURATION_H
#define VINLANDVIKINGS_VIN_CONFIGURATION_H

#include "hex/hexagon.h"
#include "heximage.h"

#include <sdl/textureview.h>
#include <sdl/sound.h>
#include <sdl/sprite.h>
#include <sdl/font.h>

#include <imgui.h>

#pragma warning( push )
#pragma warning( disable : 4005 )
#define JSON_USE_IMPLICIT_CONVERSIONS 0
#include <nlohmann/json.hpp>
#pragma warning( pop )

#include <vector>
#include <memory>

namespace vin {

	class Configuration {
	public:
		static Configuration& getInstance();

		Configuration(const Configuration&) = delete;
		Configuration& operator=(const Configuration&) = delete;

		void load(const std::string& jsonFile);
		
		void save();

		const sdl::Font& loadFont(const std::string& file, int fontSize);
		sdl::TextureView loadSprite(const std::string& file);

		const std::string& getLoadedFilename() const;

		const sdl::Font& getDefaultFont(int size);

		std::vector<HexImage> loadHexImages();

	private:
		Configuration();

		void clear();

		struct Image {
			sdl::Texture texture;
			float x;
			float y;
			float dx;
			float dy;
		};

		std::string jsonPath_;
		std::map<std::string, sdl::Sound> sounds_;
		std::map<std::string, sdl::Font> fonts_;
		std::map<std::string, Image> images_;
		nlohmann::ordered_json config_;
	};

}

#endif
