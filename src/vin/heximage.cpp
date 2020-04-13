#include "heximage.h"

namespace vin {

	HexImage::HexImage(const std::string& filename, const sdl::TextureView& sprite,
		const hex::Tile& tile, bool flat, int rotations)
		: filename_{filename}
		, sprite_{sprite}
		, tile_{tile}
		, flat_{flat}
		, rotations_{rotations} {
	}

	const std::string& HexImage::getFilename() const {
		return filename_;
	}

	const sdl::TextureView& HexImage::getImage() const {
		return sprite_;
	}

	const hex::Tile& HexImage::getTile() const {
		return tile_;
	}

	bool HexImage::isFlat() const {
		return flat_;
	}

	int HexImage::getRotations() const {
		return rotations_;
	}

} // Namespace vin.
