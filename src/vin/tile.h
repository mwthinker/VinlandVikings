#ifndef VINLANDVIKINGS_VIN_TILE_H
#define VINLANDVIKINGS_VIN_TILE_H

#include "heximage.h"
#include "hex/hex.h"
#include "hex/hexsides.h"

namespace vin {

	struct HexSprite {
		sdl::TextureView sprite{};
		int rotations{};
	};	

	struct Tile {
		Tile() = default;

		Tile(const HexSprite& hexSprite, const hex::HexSides& hexSides);

		Tile(const HexImage& hexImage, int rotation = 0);

		void rotateLeft();
		
		bool rotateUntilEqual(const hex::HexSides& otherSides);

		//void rotateRight();

		HexSprite sprite;
		hex::HexSides sides{};
	};

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_TILE_H
