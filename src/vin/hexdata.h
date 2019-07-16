#ifndef VINLANDVIKINGS_VIN_HEXDATA_H
#define VINLANDVIKINGS_VIN_HEXDATA_H

#include <sdl/sound.h>
#include <sdl/sprite.h>
#include <sdl/font.h>
#include <sdl/textureatlas.h>

#include <nlohmann/json.hpp>
#include <imgui.h>

#include <map>
#include <vector>

namespace vin {

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

		void bindTextureFromAtlas() const;

	private:
		HexData();

		std::string jsonPath_;
		sdl::TextureAtlas textureAtlas_;
		std::map<std::string, sdl::Sound> sounds_;
		std::map<std::string, sdl::Font> fonts_;
		nlohmann::json jsonObject_;
	};

}

#endif // VINLANDVIKINGS_VIN_HEXDATA_H
