#include "hexdata.h"

#include <nlohmann/json.hpp>

#include <fstream>
#include <sstream>

namespace vin {		

	/*
	void from_json(const json& j, Block& block) {
		block = Block(j.at("blockType").get<BlockType>(),
			j.at("leftColumn").get<int>(),
			j.at("bottomRow").get<int>(),
			j.at("currentRotation").get<int>());
	}

	void to_json(json& j, const Block& block) {
		j = json({
			{"bottomRow", block.getLowestStartRow()},
			{"blockType", block.getBlockType()},
			{"leftColumn", block.getStartColumn()},
			{"currentRotation", block.getCurrentRotation()}
		});
	}
	*/

	HexData::HexData() : textureAtlas_(2048, 2048, []() {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}) {
		std::ifstream defaultStream("USE_APPLICATION_JSON");
		bool applicationJson;
		defaultStream >> applicationJson;
		const std::string APPLICATION_JSON = "hexdata.json";
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

		// Font not found?
		if (fonts_.size() > size) {
			font = sdl::Font(file, fontSize);
		}

		return font;
	}

	sdl::Sprite HexData::loadSprite(const std::string& file) {
		return textureAtlas_.add(file, 1);;
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

	void HexData::bindTextureFromAtlas() const {
		textureAtlas_.getTexture().bindTexture();
	}

} // Namespace vin.
