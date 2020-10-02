#ifndef VINLANDVIKINGS_VIN_TILE_H
#define VINLANDVIKINGS_VIN_TILE_H

#include "heximage.h"
#include "hex/hex.h"
#include "hex/tile.h"

namespace vin {

	struct HexSprite {
		sdl::TextureView sprite;
		int rotations{};
	};

	struct SpriteTile {
		SpriteTile() = default;

		SpriteTile(const HexSprite& hexSprite, const hex::Tile& tile);

		explicit SpriteTile(const HexImage& hexImage, int rotation = 0);

		void rotateLeft(int nbr = 1);
		
		bool rotateUntilEqual(const hex::Tile& otherSides);

		//void rotateRight();

		HexSprite sprite;
		hex::Tile tile;
	};

}

#endif
