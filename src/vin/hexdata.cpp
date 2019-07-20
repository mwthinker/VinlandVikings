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

	HexData::HexData() {
		std::ifstream defaultStream("USE_APPLICATION_JSON");
		bool applicationJson;
		defaultStream >> applicationJson;
		const std::string APPLICATION_JSON = "images/hexdata.json";
		if (applicationJson) {
			jsonPath_ = APPLICATION_JSON;
		} else {
			// Find default path to save/load file from.
			jsonPath_ = SDL_GetPrefPath("mwthinker", "VinlandVikings") + APPLICATION_JSON;
		}
		std::ifstream stream(jsonPath_);
		if (!stream.is_open()) {
			// Assume that the file does not exist, load file from application folder.
			stream = std::ifstream(APPLICATION_JSON);
		}
		stream >> jsonObject_;
	}

	void HexData::save() {
		std::ofstream stream(jsonPath_);
		stream << jsonObject_.dump(1);
	}

	sdl::Font HexData::loadFont(const std::string& file, unsigned int fontSize) {
		size_t size = fonts_.size();
		std::string key = file;
		key += fontSize;
		sdl::Font& font = fonts_[key];
		
		if (fonts_.size() > size) {
			font = sdl::Font(file, fontSize);
		}

		return font;
	}

	sdl::Sprite HexData::loadSprite(const std::string& file) {
		size_t size = sprites_.size();
		auto& sprite = sprites_[file];
		
		if (sprites_.size() > size) {
			sprite = sdl::Sprite("images/" + file);
		}
		return sprite;
	}

	std::vector<HexImage> HexData::loadHexImages() {
		auto& hexes = jsonObject_["hexes"];

		std::vector<HexImage> hexImages;
		for (auto& hex : hexes) {
			std::string filename = "images/" + hex["image"].get<std::string>();

			sdl::Texture texture(filename);

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
			float x = (texture.getWidth() - width) / 2.f;
			float y = (texture.getHeight() - height) / 2.f;

			sdl::Sprite sprite(texture, x, y, width, height);
			hexImages.emplace_back(filename, sprite, hexSides, flat);
		}
		return hexImages;
	}

	/*
	sdl::Sprite TetrisData::getSprite(BlockType blockType) {
		switch (blockType) {
			case BlockType::I:
				return loadSprite(jsonObject_["window"]["tetrisBoard"]["sprites"]["squareI"].get<std::string>());
			case BlockType::J:
				return loadSprite(jsonObject_["window"]["tetrisBoard"]["sprites"]["squareJ"].get<std::string>());
			case BlockType::L:
				return loadSprite(jsonObject_["window"]["tetrisBoard"]["sprites"]["squareL"].get<std::string>());
			case BlockType::O:
				return loadSprite(jsonObject_["window"]["tetrisBoard"]["sprites"]["squareO"].get<std::string>());
			case BlockType::S:
				return loadSprite(jsonObject_["window"]["tetrisBoard"]["sprites"]["squareS"].get<std::string>());
			case BlockType::T:
				return loadSprite(jsonObject_["window"]["tetrisBoard"]["sprites"]["squareT"].get<std::string>());
			case BlockType::Z:
				return loadSprite(jsonObject_["window"]["tetrisBoard"]["sprites"]["squareZ"].get<std::string>());
		}
		return sdl::Sprite();
	}
	*/

	sdl::Font HexData::getDefaultFont(int size) {
		return loadFont(jsonObject_["window"]["font"].get<std::string>(), size);
	}

	//void HexData::bindTextureFromAtlas() const {
	//	textureAtlas_.getTexture().bindTexture();
	//}

} // Namespace vin.
