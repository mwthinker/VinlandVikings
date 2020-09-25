#include "spritetile.h"

namespace vin {

	SpriteTile::SpriteTile(const HexSprite& hexSprite, const hex::Tile& hexTile)
		: sprite{hexSprite}
		, tile{hexTile} {

		assert(hexSprite.rotations >= 0);
	}

	SpriteTile::SpriteTile(const HexImage& hexImage, int rotation) :
		sprite{hexImage.getImage(), (hexImage.getRotations() + rotation) % 6}, tile{hexImage.getTile()} {
		assert(rotation >= 0);
	}

	//void Tile::rotateRight() {}

	void SpriteTile::rotateLeft(int nbr) {
		hex::rotate(tile, nbr);
		sprite.rotations = (sprite.rotations + nbr) % 6;
	}

	bool SpriteTile::rotateUntilEqual(const hex::Tile& otherTile) {
		for (int i = 0; i < 6; ++i) {
			rotateLeft();
			if (tile == otherTile) {
				return true;
			}
		}
		return false;
	}

}
