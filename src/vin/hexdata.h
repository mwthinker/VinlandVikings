#ifndef VINLANDVIKINGS_VIN_HEXDATA_H
#define VINLANDVIKINGS_VIN_HEXDATA_H

#include "hex/hexagon.h"
#include "heximage.h"

#include <sdl/textureview.h>
#include <sdl/sound.h>
#include <sdl/sprite.h>
#include <sdl/font.h>

#include <imgui.h>

#include <vector>
#include <memory>

namespace vin {

	class HexData {
	public:
		static HexData& getInstance();

		HexData(HexData const&) = delete;
		HexData& operator=(const HexData&) = delete;

		void load(const std::string& jsonFile);
		
		void save();

		const sdl::Font& loadFont(const std::string& file, int fontSize);
		sdl::TextureView loadSprite(const std::string& file);

		const std::string& getLoadedFilename() const;

		const sdl::Font& getDefaultFont(int size);

		std::vector<HexImage> loadHexImages();

	private:
		class Impl;
		std::unique_ptr<Impl> impl_;

		HexData();
	};

}

#endif
