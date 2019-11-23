#ifndef VINLANDVIKINGS_VIN_HEXDATA_H
#define VINLANDVIKINGS_VIN_HEXDATA_H

#include "hex/hexagon.h"
#include "heximage.h"
#include "textureview.h"

#include <sdl/sound.h>
#include <sdl/sprite.h>
#include <sdl/font.h>

#include <imgui.h>

#include <vector>
#include <memory>

namespace vin {

	class HexData {
	public:
		static HexData& getInstance(const std::string& filename = "");

		HexData(HexData const&) = delete;
		HexData& operator=(const HexData&) = delete;

		void save();

		const sdl::Font& loadFont(const std::string& file, int fontSize);
		SpriteView loadSprite(const std::string& file);

		const sdl::Font& getDefaultFont(int size);

		std::vector<HexImage> loadHexImages();

	private:
		class Impl;
		std::unique_ptr<Impl> impl_;

		HexData(const std::string& filename);
	};

}

#endif // VINLANDVIKINGS_VIN_HEXDATA_H
