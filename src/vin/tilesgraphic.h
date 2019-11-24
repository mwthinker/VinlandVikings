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
		float getAngle() const;
		
		void setGrid(bool grid);
		bool isGrid() const;

		void setHexCoord(bool hexCoord);
		bool isHexCoord() const;

		void setXYCoord(bool xyCoord);
		bool isXYCoord() const;

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
		void drawColor(Shader& shader);
		void drawGrid(Shader& shader);
		void drawHexCoord(Shader& shader);
		void drawWorld(Shader& shader);
		void drawXYCoord(Shader& shader);

		hex::Hexi worldToHex(Vec2 pos) const;
		Vec2 hexToWorld(hex::Hexi pos) const;

		float angle_{PI / 3};
		bool dirty_{true};
		bool grid_{true};
		bool hexCoord_{true};
		bool xyCoord_{true};
		Color color_{};

		HexDimension hexDimension_;
		Mat2 hexToWorld_{1};
		Mat4 worldToScreen_{1};
		Graphic graphic_;
		std::unordered_map<hex::Hexi, HexImage> hexImages_;
	};
	
} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_TILESGRAPHIC_H