#ifndef VINLANDVIKINGS_VIN_HEXDATA_H
#define VINLANDVIKINGS_VIN_HEXDATA_H

#include "hexagon.h"
#include "heximage.h"
#include "textureview.h"

#include <sdl/sound.h>
#include <sdl/sprite.h>
#include <sdl/font.h>
#include <sdl/textureatlas.h>

#include <imgui.h>

#include <map>
#include <vector>
#include <array>

#include <config.pb.h>

namespace vin {

	class HexData {
	public:
		static HexData& getInstance(const std::string& filename = "") {
			static HexData instance{filename};
			return instance;
		}

		HexData(HexData const&) = delete;
		HexData& operator=(const HexData&) = delete;

		void save();

		const sdl::Font& loadFont(const std::string& file, int fontSize);
		SpriteView loadSprite(const std::string& file);

		const sdl::Font& getDefaultFont(int size);

		std::vector<HexImage> loadHexImages();

	private:
		HexData(const std::string& filename);

		struct Image {
			sdl::Texture texture;
			float x{};
			float y{};
			float dx{};
			float dy{};
		};

		std::string jsonPath_;
		std::map<std::string, sdl::Sound> sounds_;
		std::map<std::string, sdl::Font> fonts_;
		std::map<std::string, Image> images_;
		vin_config::HexTiles hexTiles_;
	};

}

#endif // VINLANDVIKINGS_VIN_HEXDATA_H
