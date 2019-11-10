#include "hexdata.h"
#include "hexagon.h"
#include "logger.h"

#include <nlohmann/json.hpp>

#include <fstream>
#include <sstream>

using nlohmann::json;

namespace vin {
	
	//void from_json(const json& j, HexImage& hex) {
	//	hex = HexImage(sdl::Sprite(j.at("blockType").get<std::string>()));
	//}

	/*
	void to_json(json& j, const HexImage& block) {
		j = json({
			{"bottomRow", block.getLowestStartRow()},
			{"blockType", block.getBlockType()},
			{"leftColumn", block.getStartColumn()},
			{"currentRotation", block.getCurrentRotation()}
		});
	}
	*/

	void from_json(const json& j, HexSide& hex) {
		hex = static_cast<HexSide>(j.get<int>());
	}

	HexData::HexData(const std::string& filename) {
		std::ifstream defaultStream("USE_APPLICATION_JSON");
		bool applicationJson;
		defaultStream >> applicationJson;
		if (applicationJson) {
			jsonPath_ = filename;
		} else {
			// Find default path to save/load file from.
			jsonPath_ = SDL_GetPrefPath("mwthinker", "VinlandVikings") + filename;
		}
		std::ifstream stream(jsonPath_);
		if (!stream.is_open()) {
			// Assume that the file does not exist, load file from application folder.
			stream = std::ifstream(filename);
		}
		stream >> jsonObject_;
	}

	void HexData::save() {
		std::ofstream stream(jsonPath_);
		stream << jsonObject_.dump(1);
	}

	const sdl::Font& HexData::loadFont(const std::string& file, unsigned int fontSize) {
		size_t size = fonts_.size();
		std::string key = file;
		key += fontSize;
		sdl::Font& font = fonts_[key];
		
		if (fonts_.size() > size) {
			font = sdl::Font(file, fontSize);
		}

		return font;
	}

	SpriteView HexData::loadSprite(const std::string& file) {
		size_t size = images_.size();
		auto& image = images_[file];
		
		if (images_.size() > size) {
			sdl::Surface surface{file};
			image.texture.generate();
			image.texture.texImage(surface);
			image.dx = static_cast<float>(surface.getWidth());
			image.dy = static_cast<float>(surface.getHeight());
		}
		return SpriteView{image.texture, 0, 0, 1, 1};
	}

	std::vector<HexImage> HexData::loadHexImages() {
		auto& hexes = jsonObject_["hexes"];

		std::vector<HexImage> hexImages;
		for (auto& hex : hexes) {
			std::string filename = "images/" + hex["image"].get<std::string>();

			loadSprite(filename);
			const auto& image = images_[filename];

			float size = hex["size"].get<float>();
			bool flat = hex["flat"].get<bool>();
			auto& hexSidesJson = hex["sides"];
			if (hexSidesJson.size() != 6) {
				logger()->warn("Missing sides in {}, nr of sides is {}", filename, hexSidesJson.size());
				continue;
			}

			std::array<HexSide, 6> hexSides;
			for (int i = 0; i < 6; ++i) {
				hexSides[i] = hexSidesJson[i].get<HexSide>();
			}
			float width = size;
			float height = std::sqrt(3.f) / 2.f * size;
			float x = (image.dx - width) / 2.f / image.dx;
			float y = (image.dy - height) / 2.f / image.dy;

			SpriteView sprite{image.texture, x, y, width/image.dx, height/image.dy};
			hexImages.emplace_back(filename, sprite, hexSides, flat);
		}
		return hexImages;
	}

	const sdl::Font& HexData::getDefaultFont(int size) {
		return loadFont(jsonObject_["window"]["font"].get<std::string>(), size);
	}

} // Namespace vin.
