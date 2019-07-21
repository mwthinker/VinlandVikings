#include "heximage.h"

namespace vin {

	HexImage::HexImage() :flat_(true), hexSides_{HexSide::NONE} {
	}

	HexImage::HexImage(const std::string& filename, sdl::Sprite& image, const HexSides& hexSides, bool flat)
		: filename_(filename), image_(image), hexSides_(hexSides), flat_(flat) {
	}

	const std::string& HexImage::getFilename() const {
		return filename_;
	}

	const sdl::Sprite& HexImage::getImage() const {
		return image_;
	}

	const HexSides& HexImage::getHexSides() const {
		return hexSides_;
	}

	bool HexImage::isFlat() const {
		return flat_;
	}

} // Namespace vin.
