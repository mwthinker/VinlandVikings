#include "shape.h"

#include <algorithm> 

namespace vin::hex::shape {

	namespace {

		Hexi oddToCube(int x, int y) {
			auto q = x - (y - (y & 1)) / 2;
			auto s = y;
			auto r = -q - s;
			return {q, r};
		}

	}

	std::vector<Hexi> createHex(int radiusNbr) {
		std::vector<Hexi> hexes;
		for (int q = -radiusNbr; q <= radiusNbr; ++q) {
			int r1 = std::max(-radiusNbr, -q - radiusNbr);
			int r2 = std::min(radiusNbr, -q + radiusNbr);
			for (int r = r1; r <= r2; ++r) {
				hexes.emplace_back(q, r);
			}
		}
		return hexes;
	}

	std::vector<Hexi> createParallelogram(int columns, int rows) {
		std::vector<Hexi> hexes;
		for (int i = -columns; i < columns; ++i) {
			for (int j = -rows; j < rows; ++j) {
				hexes.emplace_back(i, j);
			}
		}
		return hexes;
	}

	std::vector<Hexi> createRectangle(int columns, int rows) {
		std::vector<Hexi> hexes;
		for (int x = -columns; x < columns; ++x) {
			for (int y = -rows; y < rows; ++y) {
				auto hex = oddToCube(x, y);
				hexes.push_back(hex);
			}
		}
		return hexes;
	}

}
