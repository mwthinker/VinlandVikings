#include "heximage.h"

namespace vin {

	HexImage::HexImage(const std::string& filename, SpriteView sprite, const hex::HexSides& hexSides, bool flat, int rotations)
		: filename_(filename), sprite_(sprite), hexSides_(hexSides), flat_(flat), rotations_(rotations) {
	}

	const std::string& HexImage::getFilename() const {
		return filename_;
	}

	SpriteView HexImage::getImage() const {
		return sprite_;
	}

	const hex::HexSides& HexImage::getHexSides() const {
		return hexSides_;
	}

	bool HexImage::isFlat() const {
		return flat_;
	}

	int HexImage::getRotations() const {
		return rotations_;
	}

} // Namespace vin.
