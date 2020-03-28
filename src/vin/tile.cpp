#include "tile.h"

namespace vin {

	Tile::Tile(const HexSprite& hexSprite, const hex::HexSides& hexSides)
		: sprite{hexSprite}
		, sides{hexSides} {

		assert(hexSprite.rotations >= 0);
	}

	Tile::Tile(const HexImage& hexImage, int rotation) :
		sprite{hexImage.getImage(), (hexImage.getRotations() + rotation) % 6}, sides{hexImage.getHexSides()} {
		assert(rotation >= 0);
	}

	//void Tile::rotateRight() {}

	void Tile::rotateLeft(int nbr) {
		hex::rotate(sides, nbr);
		sprite.rotations = (sprite.rotations + nbr) % 6;
	}

	bool Tile::rotateUntilEqual(const hex::HexSides& otherSides) {
		for (int i = 0; i < 6; ++i) {
			rotateLeft();
			if (sides == otherSides) {
				return true;
			}
		}
		return false;
	}

} // Namespace vin.
