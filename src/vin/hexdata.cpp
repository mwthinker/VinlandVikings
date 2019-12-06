#include "hexdata.h"
#include "hex/hexagon.h"
#include "logger.h"

#include <fstream>
#include <sstream>
#include <array>
#include <map>
#include <string>
#include <filesystem>

#include <config.pb.h>
#include <google/protobuf/util/json_util.h>
#include <google/protobuf/util/type_resolver_util.h>

namespace pb = google::protobuf;
namespace pbu = google::protobuf::util;

namespace vin {

	namespace {

		constexpr hex::HexSide toVinHexSide(vin_config::HexSide hexSide) {
			switch (hexSide) {
				case vin_config::HexSide::NONE:
					return hex::HexSide::NONE;
				case vin_config::HexSide::MOUNTAIN:
					return hex::HexSide::MOUNTAIN;
				case vin_config::HexSide::GRASS:
					return hex::HexSide::GRASS;
				case vin_config::HexSide::WATER:
					return hex::HexSide::WATER;
				case vin_config::HexSide::FOREST:
					return hex::HexSide::FOREST;
			}
			assert(false); // Should not be here.
		}

		constexpr hex::HexSides convertConfigHexSides(const pb::RepeatedField<pb::int32>& configSides) {
			hex::HexSides hexSides{};
			for (int i = 0; i < 6; ++i) {
				//hexSides[i] = toVinHexSide(configSides.sides(i));
			}
			return hexSides;
		}

	}

	class HexData::Impl {
	public:
		Impl(const std::string& filename);

		Impl(HexData const&) = delete;
		Impl& operator=(const HexData&) = delete;

		void save();

		const sdl::Font& loadFont(const std::string& file, int fontSize);
		SpriteView loadSprite(const std::string& file);

		const sdl::Font& getDefaultFont(int size);

		std::vector<HexImage> loadHexImages();

	private:
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

	HexData::HexData(const std::string& filename)
		: impl_{std::make_unique<HexData::Impl>(filename)} {
	}

	HexData& HexData::getInstance(const std::string& filename) {
		static HexData instance{filename};
		return instance;
	}

	void HexData::save() {
		impl_->save();
	}

	const sdl::Font& HexData::loadFont(const std::string& file, int fontSize) {
		return impl_->loadFont(file, fontSize);
	}
	
	SpriteView HexData::loadSprite(const std::string& file) {
		return impl_->loadSprite(file);
	}

	const sdl::Font& HexData::getDefaultFont(int size) {
		return impl_->getDefaultFont(size);
	}

	std::vector<HexImage> HexData::loadHexImages() {
		return impl_->loadHexImages();
	}

	HexData::Impl::Impl(const std::string& filename) {
		if (std::filesystem::exists("USE_APPLICATION_JSON")) {
			jsonPath_ = filename;
		} else {
			// Find default path to save/load file from.
			jsonPath_ = SDL_GetPrefPath("mwthinker", "VinlandVikings") + filename;
		}

		std::ifstream input{jsonPath_, std::ios::in | std::ios::binary};
		if (!input.is_open()) {
			// Assume that the file does not exist, load file from application folder.
			input = std::ifstream{jsonPath_, std::ios::in | std::ios::binary};
		}

		pbu::JsonParseOptions parseOptions;
		parseOptions.case_insensitive_enum_parsing = false;
		parseOptions.ignore_unknown_fields = false;
		
		std::string jsonContent{std::istreambuf_iterator<char>{input}, std::istreambuf_iterator<char>{}};
		auto status = pbu::JsonStringToMessage(jsonContent, &hexTiles_, parseOptions);

		if (!status.ok()) {
			logger()->warn("Jsonfile {}, protobuf parsing error: {}", filename, status.message().ToString());
			logger()->warn("Error message: {}", status.error_message().ToString());
		}
	}

	void HexData::Impl::save() {
	}

	const sdl::Font& HexData::Impl::loadFont(const std::string& file, int fontSize) {
		auto size = fonts_.size();
		std::string key = file;
		key += fontSize;
		sdl::Font& font = fonts_[key];
		if (fonts_.size() > size) {
			font = sdl::Font{file, fontSize};
		}
		return font;
	}

	SpriteView HexData::Impl::loadSprite(const std::string& file) {
		size_t size = images_.size();
		auto& image = images_[file];
		
		if (images_.size() > size) {
			sdl::Surface surface{file};
			sdl::flipVertical(surface);
			image.texture.generate();
			image.texture.texImage(surface);
			image.dx = static_cast<float>(surface.getWidth());
			image.dy = static_cast<float>(surface.getHeight());
		}
		return SpriteView{image.texture, 0, 0, 1, 1};
	}	

	std::vector<HexImage> HexData::Impl::loadHexImages() {
		std::vector<HexImage> hexImages;
		for (const auto& tile : hexTiles_.hex_tiles()) {
			auto filename = "images/" + tile.image();
			loadSprite(filename);

			const auto& image = images_[filename];
			float size = static_cast<float>(tile.size());
			bool flat = tile.flat();

			if (auto size = tile.sides().size(); size != 6) {
				logger()->warn("Missing sides in {}, nr of sides is {}", filename, size);
				continue;
			}
			hex::HexSides hexSides{};
			for (int i = 0; i < 6; ++i) {
				hexSides[i] = toVinHexSide(tile.sides(i));
			}

			float width = size;
			float height = std::sqrt(3.f) / 2.f * size;
			float x = (image.dx - width) / 2.f / image.dx;
			float y = (image.dy - height) / 2.f / image.dy;

			SpriteView sprite{image.texture, x, y, width / image.dx, height / image.dy};
			hexImages.emplace_back(filename, sprite, hexSides, flat);
		}
		return hexImages;
	}

	const sdl::Font& HexData::Impl::getDefaultFont(int size) {
		return {};
	}

} // Namespace vin.
