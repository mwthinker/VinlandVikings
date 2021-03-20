#include "tilesgraphic.h"
#include "hex/hexagon.h"
#include "hex/hash.h"
#include "logger.h"

namespace vin {

	namespace {

		template <class T>
		bool updateValue(T& value, T newValue) {
			if (value != newValue) {
				value = newValue;
				return true;
			}
			return false;
		}

	}

	TilesGraphic::TilesGraphic(const HexDimension& dimension, const Mat2& hexToWorld)
		: hexDimension_{dimension}
		, hexToWorld_{hexToWorld} {
	}

	hex::Hexi TilesGraphic::worldToHex(Vec2 pos) const {
		auto hex = glm::inverse<>(hexToWorld_) * pos;
		return hex::hexRound({hex.x, hex.y});
	}

	Vec2 TilesGraphic::hexToWorld(hex::Hexi pos) const {
		return hexToWorld_ * Vec2{pos.q(), pos.r()};
	}

	void TilesGraphic::setAngle(float angle) {
		dirty_ |= updateValue(angle_, angle);
	}

	float TilesGraphic::getAngle() const {
		return angle_;
	}

	void TilesGraphic::setGrid(bool grid) {
		dirty_ |= updateValue(grid_, grid);
	}

	bool TilesGraphic::isGrid() const {
		return grid_;
	}

	void TilesGraphic::setHexCoords(bool hexCoord) {
		dirty_ |= updateValue(hexCoord_, hexCoord);
	}

	bool TilesGraphic::isHexCoords() const {
		return hexCoord_;
	}

	void TilesGraphic::setXYCoords(bool xyCoord) {
		dirty_ |= updateValue(xyCoord_, xyCoord);
	}

	bool TilesGraphic::isXYCoords() const {
		return xyCoord_;
	}

	void TilesGraphic::fillTile(hex::Hexi hex, const SpriteTile& tile) {
		hexImages_[hex] = tile;
		dirty_ = true;
	}

	void TilesGraphic::fillTile(hex::Hexi hex, Color color) {
		hexImages_[hex] = SpriteTile{HexImage{"", {}, {}, true}};
		dirty_ = true;
	}

	void TilesGraphic::fill(Color color) {
		color_ = color;
		for (auto& [hex, hexImage] : hexImages_) {
			hexImage = SpriteTile{HexImage{"", {}, {}, true}};
		}
		dirty_ = true;
	}

	void TilesGraphic::fill(const SpriteTile& tile) {
		for (auto& [hex, mapTile] : hexImages_) {
			mapTile = tile;
		}
		dirty_ = true;
	}

	void TilesGraphic::fillGrid(hex::Hexi hex, Color color) {
		hexImages_[hex] = SpriteTile{HexImage{"", {}, {}, true}};
		dirty_ = true;
	}

	void TilesGraphic::clearTile(hex::Hexi hex) {
		auto size = hexImages_.size();
		if (size != hexImages_.erase(hex)) {
			dirty_ = true;
		}
	}

	void TilesGraphic::clear() {
		hexImages_.clear();
		graphic_.clearDraw();
		dirty_ = true;
	}

	void TilesGraphic::draw(const sdl::Shader& shader) {
		if (dirty_) {
			spdlog::debug("[TilesGraphic] State dirty");
			graphic_.clearDraw();
			graphic_.setMatrix(worldToScreen_);
			
			drawColor(shader);
			drawGrid(shader);
			drawWorld(shader);
			drawHexCoord(shader);
			drawXYCoord(shader);

			dirty_ = false;
		} else {
			graphic_.setMatrix(worldToScreen_);
		}
		
		graphic_.draw(shader);
	}

	void TilesGraphic::drawColor(const sdl::Shader& shader) {
		if (color_.toImU32() != 0) {
			for (const auto& [hex, hexTile] : hexImages_) {
				auto pos = hexToWorld(hex);
				graphic_.addHexagon(pos, 0.f, hexDimension_.outerSize, color_, hexDimension_.angle);
			}
		}
	}

	void TilesGraphic::drawGrid(const sdl::Shader& shader) {
		if (grid_) {
			for (const auto& [hex, hexTile] : hexImages_) {
				auto pos = hexToWorld(hex);
				graphic_.addHexagon(pos, hexDimension_.innerSize, hexDimension_.outerSize, White, hexDimension_.angle);
			}
		}
	}

	void TilesGraphic::drawHexCoord(const sdl::Shader& shader) {
		if (hexCoord_) {
			graphic_.addHexagon(hexToWorld(hex::HexiZero), hexDimension_.innerSize * 0.8f, hexDimension_.outerSize, White, hexDimension_.angle);
			graphic_.addHexagon(hexToWorld(hex::HexiQ), hexDimension_.innerSize, hexDimension_.outerSize, Red, hexDimension_.angle);
			graphic_.addHexagon(hexToWorld(hex::HexiR), hexDimension_.innerSize, hexDimension_.outerSize, Green, hexDimension_.angle);
			graphic_.addHexagon(hexToWorld(hex::HexiS), hexDimension_.innerSize, hexDimension_.outerSize, Blue, hexDimension_.angle);
		}
	}

	void TilesGraphic::drawWorld(const sdl::Shader& shader) {
		for (const auto& [hex, tile] : hexImages_) {
			auto pos = hexToWorld(hex);
			graphic_.addHexagonImage(pos, hexDimension_.outerSize, tile.sprite.sprite, tile.sprite.rotations * Pi / 3 + hexDimension_.angle);
		}
	}

	void TilesGraphic::drawXYCoord(const sdl::Shader& shader) {
		if (xyCoord_) {
			graphic_.addRectangle({0.f, 0.f}, {2.0f, 0.05f}, Red);
			graphic_.addRectangle({0.f, 0.f}, {0.05f, 2.0f}, Green);
		}
	}

	void TilesGraphic::setMatrix(const Mat4& mat) {
		worldToScreen_ = mat;
	}

	const TilesGraphic::Map& TilesGraphic::getMap() const {
		return hexImages_;
	}

	void TilesGraphic::setMap(const Map& map) {
		hexImages_ = map;
		dirty_ = true;
	}

	SpriteTile TilesGraphic::getTile(const hex::Hexi& hex) const {
		if (auto it = hexImages_.find(hex); it != hexImages_.end()) {
			return it->second;
		}
		return SpriteTile{};
	}

}
