#ifndef VINLANDVIKINGS_VIN_CONFIGURATION_H
#define VINLANDVIKINGS_VIN_CONFIGURATION_H

#include "hex/hexagon.h"
#include "hexcanvas.h"
#include "heximage.h"
#include "hex/tileboard.h"

#include <sdl/textureview.h>
#include <sdl/sound.h>
#include <sdl/sprite.h>
#include <sdl/font.h>

#include <imgui.h>

#include <vector>
#include <memory>

namespace vin {

	void save(const std::string& file, const HexCanvas& hexCanvas);

	hex::TileBoard load(const std::string& file);

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

		class Impl;
		std::unique_ptr<Impl> impl_;
	};

}

#endif
