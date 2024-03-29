#ifndef VINLANDVIKINGS_VIN_TILESGRAPHIC_H
#define VINLANDVIKINGS_VIN_TILESGRAPHIC_H


#include "heximage.h"
#include "hex/hex.h"
#include "types.h"
#include "hexdimension.h"
#include "hex/hash.h"
#include "tilelexicon.h"

#include <sdl/color.h>
#include <sdl/graphic.h>

#include <unordered_map>

namespace vin {

	class TilesGraphic {
	public:
		//using Map = std::map<hex::Hexi, SpriteTile, hex::HexComparator>;
		using Map = std::unordered_map<hex::Hexi, SpriteTile>;

		TilesGraphic(const HexDimension& dimension, const Mat2& hexToWorld);

		void setAngle(float angle);
		float getAngle() const;
		
		void setGrid(bool grid);
		bool isGrid() const;

		void setHexCoords(bool hexCoord);
		bool isHexCoords() const;

		void setXYCoords(bool xyCoord);
		bool isXYCoords() const;

		void fillTile(hex::Hexi hex, const SpriteTile& tile);
		void fillTile(hex::Hexi hex, Color color);

		void fill(Color color);
		void fill(const SpriteTile& tile);
		void fillGrid(hex::Hexi hex, Color color);

		void clearTile(hex::Hexi hex);

		void clear();

		void draw(sdl::Shader& shader);

		void setWorldToClip(const Mat4& mat);

		const Map& getMap() const;

		void setMap(const Map& map);

		SpriteTile getTile(const hex::Hexi& hex) const;

	private:
		void drawColor(const sdl::Shader& shader);
		void drawGrid(const sdl::Shader& shader);
		void drawHexCoord(const sdl::Shader& shader);
		void drawWorld(const sdl::Shader& shader);
		void drawXYCoord(const sdl::Shader& shader);

		hex::Hexi worldToHex(Vec2 pos) const;
		Vec2 hexToWorld(hex::Hexi pos) const;

		float angle_ = Pi / 3.f;
		bool grid_ = true;
		bool hexCoord_ = false;
		bool xyCoord_ = false;
		Color color_;

		HexDimension hexDimension_;
		Mat2 hexToWorld_{1};
		Mat4 worldToScreen_{1};
		sdl::Graphic graphic_;
		Map hexImages_;
	};
	
}

#endif
