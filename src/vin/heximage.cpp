#include "heximage.h"

namespace vin {

	HexImage::HexImage(const std::string& filename, const sdl::Sprite& image, const HexSides& hexSides, bool flat, int rotations)
		: filename_(filename), image_(image), hexSides_(hexSides), flat_(flat), rotations_(rotations) {
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

	int HexImage::getRotations() const {
		return rotations_;
	}

} // Namespace vin.
