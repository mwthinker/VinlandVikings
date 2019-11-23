#ifndef VINLANDVIKINGS_VIN_TILESGRAPHIC_H
#define VINLANDVIKINGS_VIN_TILESGRAPHIC_H

#include "graphic.h"
#include "heximage.h"
#include "hex/hex.h"
#include "types.h"
#include "hexdimension.h"
#include "hex/hash.h"

#include <sdl/color.h>

#include <unordered_map>

namespace vin {

	class TilesGraphic {
	public:
		TilesGraphic(const HexDimension& dimension, const Mat2& hexToWorld);

		void setAngle(float angle);
		
		void setGrid(bool grid);

		void setHexCoord(bool hexCoord);

		void setXYCoord(bool xyCoord);

		void fillTile(hex::Hexi hex, const HexImage& image);
		void fillTile(hex::Hexi hex, const Color& color);

		void fill(const Color& color);
		void fill(const HexImage& image);
		void fillGrid(hex::Hexi hex, const Color& color);

		void clearTile(hex::Hexi hex);

		void clear();

		void draw(Shader& shader);

		void setMatrix(const Mat4& mat);

	private:
		hex::Hexi worldToHex(Vec2 pos) const;
		Vec2 hexToWorld(hex::Hexi pos) const;

		float angle_{PI / 3};
		bool dirty_{true};
		bool grid_{true};
		bool hexCoord_{true};
		bool xyCoord_{true};

		HexDimension hexDimension_;
		Mat2 hexToWorld_;
		Mat4 worldToScreen_;
		Graphic graphic_;
		std::unordered_map<hex::Hexi, HexImage> hexImages_;
	};
	
} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_TILESGRAPHIC_H
