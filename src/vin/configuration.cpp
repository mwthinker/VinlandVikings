#include "configuration.h"
#include "hex/hexagon.h"

#include <fstream>
#include <sstream>
#include <array>
#include <map>
#include <string>
#include <filesystem>

#include <spdlog/spdlog.h>

using json = nlohmann::json;

namespace vin {

	namespace {

		NLOHMANN_JSON_SERIALIZE_ENUM(hex::HexSide, {
			{hex::HexSide::Forest, "WATER"},
			{hex::HexSide::Grass, "GRASS"},
			{hex::HexSide::Mountain, "MOUNTAIN"},
			{hex::HexSide::Water, "WATER"},
			{hex::HexSide::None, "NONE"}
		})

	}

	Configuration& Configuration::getInstance() {
		static Configuration configuration;
		return configuration;
	}

	Configuration::Configuration() {
	}

	void Configuration::load(const std::string& jsonFile) {
		clear();

		spdlog::info("[HexData] Current working directory {}", std::filesystem::current_path().string());
		jsonPath_ = jsonFile;
		/*if (std::filesystem::exists("USE_APPLICATION_JSON")) {
			jsonPath_ = jsonFile;
		} else {
			// Find default path to save/load file from.
			jsonPath_ = SDL_GetPrefPath("mwthinker", "VinlandVikings") + jsonFile;
		}*/

		std::ifstream input{jsonPath_, std::ios::in | std::ios::binary};
		if (!input.is_open()) {
			// Assume that the file does not exist, load file from application folder.
			input = std::ifstream{jsonPath_, std::ios::in | std::ios::binary};
		}

		input >> config_;
	}

	void Configuration::save() {
	}

	const sdl::Font& Configuration::loadFont(const std::string& file, int fontSize) {
		auto size = fonts_.size();
		std::string key = file;
		key += fontSize;
		sdl::Font& font = fonts_[key];
		if (fonts_.size() > size) {
			font = sdl::Font{file, fontSize};
		}
		return font;
	}

	sdl::TextureView Configuration::loadSprite(const std::string& file) {
		size_t size = images_.size();
		auto& image = images_[file];
		
		if (images_.size() > size) {
			sdl::Surface surface{file};
			image.texture.generate();
			image.texture.texImage(sdl::Surface{file});
			image.dx = static_cast<float>(surface.getWidth());
			image.dy = static_cast<float>(surface.getHeight());
		}
		return sdl::TextureView{image.texture, 0, 0, 1, 1};
	}

	std::vector<HexImage> Configuration::loadHexImages() {
		std::vector<HexImage> hexImages;
		auto hexTiles = config_["hex_image_tiles"];

		for (const auto& tile : hexTiles) {
			auto filename = fmt::format("imageTiles/{}", tile["image"].get<std::string>());
			loadSprite(filename);

			const auto& image = images_[filename];
			float size = tile["size"].get<float>();
			bool flat = tile["flat"].get<bool>();

			if (auto size = tile["sides"].size(); size != 6) {
				spdlog::warn("[HexData] Missing sides in {}, nr of sides is {}", filename, size);
				continue;
			}
			
			hex::Tile hexTile;
			for (int i = 0; i < 6; ++i) {
				hexTile[i] = tile["sides"][i].get<hex::HexSide>();
			}

			float width = size;
			float height = std::sqrt(3.f) / 2.f * size;
			float x = (image.dx - width) / 2.f / image.dx;
			float y = (image.dy - height) / 2.f / image.dy;

			sdl::TextureView sprite{image.texture, x, y, width / image.dx, height / image.dy};
			hexImages.emplace_back(filename, sprite, hexTile, flat);
		}
		return hexImages;
	}

	const sdl::Font& Configuration::getDefaultFont(int size) {
		return {};
	}

	const std::string& Configuration::getLoadedFilename() const {
		return jsonPath_;
	}

	void Configuration::clear() {
		jsonPath_ = "";
		sounds_.clear();
		fonts_.clear();
		images_.clear();
	}

}
