#ifndef VINLANDVIKINGS_VIN_HEXIMAGE_H
#define VINLANDVIKINGS_VIN_HEXIMAGE_H

#include "hexagon.h"

#include <sdl/sprite.h>

#include <string>

namespace vin {

	class HexImage {
	public:
		HexImage();

		HexImage(const HexImage&) = default;

		HexImage& operator=(const HexImage&) = default;

		HexImage(const std::string& filename, sdl::Sprite& image, const HexSides& hexSides, bool flat);

		const std::string& getFilename() const;

		const sdl::Sprite& getImage() const;

		const HexSides& getHexSides() const;

		bool isFlat() const;

	private:
		std::string filename_;
		sdl::Sprite image_;
		HexSides hexSides_;
		bool flat_;
	};

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_HEXIMAGE_H
