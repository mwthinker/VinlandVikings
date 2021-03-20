#include "configuration.h"
#include "hex/hexagon.h"

#include <fstream>
#include <sstream>
#include <array>
#include <map>
#include <string>
#include <filesystem>

#include <spdlog/spdlog.h>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace vin::hex {

	NLOHMANN_JSON_SERIALIZE_ENUM(hex::HexSide, {
		{hex::HexSide::None, "NONE"},
		{hex::HexSide::Mountain, "MOUNTAIN"},
		{hex::HexSide::Grass, "GRASS"},
		{hex::HexSide::Water, "WATER"},
		{hex::HexSide::Forest, "FOREST"}
	})

	void to_json(json& j, const Hexi& hex) {
		j = json{{"q", hex.q()}, {"r", hex.r()}};
	}

	void from_json(const json& json, Hexi& hex) {
		hex = Hexi{json["q"].get<Hexi::value_type>(), json["r"].get<Hexi::value_type>()};
	}

	void to_json(json& j, const Tile& tile) {
		for (const auto& side : tile) {
			j.push_back(side);
		}
	}

	void from_json(const json& json, Tile& tile) {
		for (int i = 0; i < 6; ++i) {
			tile[i] = json[i].get<HexSide>();
		}
	}

	void to_json(json& json, const TileBoard& tileBoard) {
		for (const auto& [hex, tile] : tileBoard) {
			json.push_back({{"hex", hex}, {"tile", tile}});
		}
	}

	void from_json(const json& json, TileBoard& tileBoard) {
		for (const auto& tileHex : json) {
			tileBoard.put(tileHex["hex"].get<hex::Hexi>(), tileHex["tile"].get<hex::Tile>());
		}
	}

}

namespace vin {

	namespace {

		struct Image {
			sdl::Texture texture;
			float x;
			float y;
			float dx;
			float dy;
		};

	}

	void save(const std::string& file, const HexCanvas& hexCanvas) {
		spdlog::info("[HexData] Current working directory {}", std::filesystem::current_path().string());


		/*if (std::filesystem::exists("USE_APPLICATION_JSON")) {
			jsonPath_ = jsonFile;
		} else {
			// Find default path to save/load file from.
			jsonPath_ = SDL_GetPrefPath("mwthinker", "VinlandVikings") + jsonFile;
		}*/

		const auto& snapshot = hexCanvas.getSnapshot();

		json json;
		json["tileboard"] = snapshot.tileBoard;

		std::ofstream output{file};
		output << json.dump(4);
	}

	hex::TileBoard load(const std::string& file) {
		json json;
		std::ifstream input{file};
		input >> json;
		return json.get<hex::TileBoard>();
	}

	struct Configuration::Impl {
		std::string jsonPath;
		std::map<std::string, sdl::Sound> sounds;
		std::map<std::string, sdl::Font> fonts;
		std::map<std::string, Image> images;
		nlohmann::ordered_json config;
	};

	Configuration& Configuration::getInstance() {
		static Configuration configuration;
		return configuration;
	}

	Configuration::Configuration()
		: impl_{std::make_unique<Impl>()} {
	}

	void Configuration::load(const std::string& jsonFile) {
		clear();

		spdlog::info("[HexData] Current working directory {}", std::filesystem::current_path().string());
		impl_->jsonPath = jsonFile;
		/*if (std::filesystem::exists("USE_APPLICATION_JSON")) {
			jsonPath_ = jsonFile;
		} else {
			// Find default path to save/load file from.
			jsonPath_ = SDL_GetPrefPath("mwthinker", "VinlandVikings") + jsonFile;
		}*/

		std::ifstream input{impl_->jsonPath, std::ios::in | std::ios::binary};
		if (!input.is_open()) {
			// Assume that the file does not exist, load file from application folder.
			input = std::ifstream{impl_->jsonPath, std::ios::in | std::ios::binary};
		}

		input >> impl_->config;
	}

	void Configuration::save() {
	}

	const sdl::Font& Configuration::loadFont(const std::string& file, int fontSize) {
		auto size = impl_->fonts.size();
		std::string key = file;
		key += fontSize;
		sdl::Font& font = impl_->fonts[key];
		if (impl_->fonts.size() > size) {
			font = sdl::Font{file, fontSize};
		}
		return font;
	}

	sdl::TextureView Configuration::loadSprite(const std::string& file) {
		size_t size = impl_->images.size();
		auto& image = impl_->images[file];
		
		if (impl_->images.size() > size) {
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
		auto hexTiles = impl_->config["hex_image_tiles"];

		for (const auto& tile : hexTiles) {
			auto filename = fmt::format("imageTiles/{}", tile["image"].get<std::string>());
			loadSprite(filename);

			const auto& image = impl_->images[filename];
			float size = tile["size"].get<float>();
			bool flat = tile["flat"].get<bool>();

			if (auto size = tile["sides"].size(); size != 6) {
				spdlog::warn("[HexData] Missing sides in {}, nr of sides is {}", filename, size);
				continue;
			}
			
			hex::Tile hexTile;
			for (int i = 0; i < 6; ++i) {
				hexTile[i] = static_cast<hex::HexSide>(tile["sides"][i].get<int>());
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
		static sdl::Font font;
		return font;
	}

	const std::string& Configuration::getLoadedFilename() const {
		return impl_->jsonPath;
	}

	void Configuration::clear() {
		impl_->jsonPath = "";
		impl_->sounds.clear();
		impl_->fonts.clear();
		impl_->images.clear();
	}

}
