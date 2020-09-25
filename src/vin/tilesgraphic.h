#ifndef VINLANDVIKINGS_VIN_TILESGRAPHIC_H
#define VINLANDVIKINGS_VIN_TILESGRAPHIC_H

#include "graphic.h"
#include "heximage.h"
#include "hex/hex.h"
#include "types.h"
#include "hexdimension.h"
#include "hex/hash.h"
#include "tilelexicon.h"

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

		void setHexCoords(bool hexCoord);
		bool isHexCoords() const;

		void setXYCoords(bool xyCoord);
		bool isXYCoords() const;

		void fillTile(hex::Hexi hex, const SpriteTile& tile);
		void fillTile(hex::Hexi hex, const Color& color);

		void fill(const Color& color);
		void fill(const SpriteTile& tile);
		void fillGrid(hex::Hexi hex, const Color& color);

		void clearTile(hex::Hexi hex);

		void clear();

		void draw(const sdl::Shader& shader);

		void setMatrix(const Mat4& mat);

		const std::unordered_map<hex::Hexi, SpriteTile>& getMap() {
			return hexImages_;
		}

		SpriteTile getTile(const hex::Hexi& hex) const {
			auto it = hexImages_.find(hex);
			if (it != hexImages_.end()) {
				return it->second;
			}
			return SpriteTile{};
		}

		void fill(const std::unordered_map<hex::Hexi, SpriteTile>& map) {
			hexImages_ = map;
			dirty_ = true;
		}

	private:
		void drawColor(const sdl::Shader& shader);
		void drawGrid(const sdl::Shader& shader);
		void drawHexCoord(const sdl::Shader& shader);
		void drawWorld(const sdl::Shader& shader);
		void drawXYCoord(const sdl::Shader& shader);

		hex::Hexi worldToHex(Vec2 pos) const;
		Vec2 hexToWorld(hex::Hexi pos) const;

		float angle_{PI / 3};
		bool dirty_{true};
		bool grid_{true};
		bool hexCoord_{false};
		bool xyCoord_{false};
		Color color_;

		HexDimension hexDimension_;
		Mat2 hexToWorld_{1};
		Mat4 worldToScreen_{1};
		Graphic graphic_;
		std::unordered_map<hex::Hexi, SpriteTile> hexImages_;
	};
	
}

#endif
