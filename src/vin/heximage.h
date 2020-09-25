#ifndef VINLANDVIKINGS_VIN_HEXIMAGE_H
#define VINLANDVIKINGS_VIN_HEXIMAGE_H

#include "heximage.h"
#include "hex/tile.h"

#include <sdl/textureview.h>

#include <string>

namespace vin {

	class HexImage {
	public:
		HexImage() = default;

		HexImage(const HexImage&) = default;

		HexImage& operator=(const HexImage&) = default;

		HexImage(const std::string& filename, const sdl::TextureView& sprite, const hex::Tile& hexSides, bool flat, int rotations = 0);

		const std::string& getFilename() const;

		const sdl::TextureView& getImage() const;

		const hex::Tile& getTile() const;

		bool isFlat() const;

		int getRotations() const;

	private:
		std::string filename_;
		sdl::TextureView sprite_;
		hex::Tile tile_;
		bool flat_ = true;
		int rotations_ = 0;
	};

}

#endif
