#ifndef VINLANDVIKINGS_VIN_HEXDATA_H
#define VINLANDVIKINGS_VIN_HEXDATA_H

#include "hexagon.h"

#include <sdl/sound.h>
#include <sdl/sprite.h>
#include <sdl/font.h>
#include <sdl/textureatlas.h>

#include <nlohmann/json.hpp>
#include <imgui.h>

#include <map>
#include <vector>
#include <array>

namespace vin {

	class HexImage {
	public:
		HexImage() : flat_(true), hexSides_{HexSide::NONE} {
		}

		HexImage(const HexImage&) = default;

		HexImage& operator=(const HexImage&) = default;

		HexImage(const std::string& filename, sdl::Sprite& image, const HexSides& hexSides, bool flat)
			: filename_(filename), image_(image), hexSides_(hexSides), flat_(flat) {
		}

		const std::string& getFilename() const {
			return filename_;
		}

		const sdl::Sprite& getImage() const {
			return image_;
		}

		const HexSides& getHexSides() const {
			return hexSides_;
		}

		bool isFlat() const {
			return flat_;
		}		

	private:
		std::string filename_;
		sdl::Sprite image_;
		HexSides hexSides_;
		bool flat_;
	};

	class HexData {
	public:
		static HexData& getInstance() {
			static HexData instance;
			return instance;
		}

		HexData(HexData const&) = delete;
		HexData& operator=(const HexData&) = delete;

		void save();

		sdl::Font loadFont(const std::string& file, unsigned int fontSize);
		sdl::Sprite loadSprite(const std::string& file);

		//sdl::Sprite getSprite(BlockType blockType);

		sdl::Font getDefaultFont(int size);

		//void bindTextureFromAtlas() const;

		std::vector<HexImage> loadHexImages();

	private:
		HexData();

		std::string jsonPath_;
		std::map<std::string, sdl::Sound> sounds_;
		std::map<std::string, sdl::Font> fonts_;
		std::map<std::string, sdl::Sprite> sprites_;
		nlohmann::json jsonObject_;
	};

}

#endif // VINLANDVIKINGS_VIN_HEXDATA_H
