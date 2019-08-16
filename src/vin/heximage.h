#ifndef VINLANDVIKINGS_VIN_HEXIMAGE_H
#define VINLANDVIKINGS_VIN_HEXIMAGE_H

#include "heximage.h"
#include "hexsides.h"

#include <sdl/sprite.h>

#include <string>

namespace vin {

	class HexImage {
	public:
		HexImage() = default;

		HexImage(const HexImage&) = default;

		HexImage& operator=(const HexImage&) = default;

		HexImage(const std::string& filename, const sdl::Sprite& image, const HexSides& hexSides, bool flat, int rotations = 0);

		const std::string& getFilename() const;

		const sdl::Sprite& getImage() const;

		const HexSides& getHexSides() const;

		bool isFlat() const;

		int getRotations() const;

	private:
		std::string filename_;
		sdl::Sprite image_;
		HexSides hexSides_ = { HexSide::NONE };
		bool flat_ = true;
		int rotations_ = 0;
	};

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_HEXIMAGE_H
