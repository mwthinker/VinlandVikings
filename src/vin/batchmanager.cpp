#include "batchmanager.h"

#include "graphic.h"
#include "hex/hexagon.h"

namespace vin {	

	namespace {



	}
	
	BatchManager::BatchManager() {
	}

	void BatchManager::add(const BatchConfig& batchConfig) {
		batches_[batchConfig.texture] = batchConfig.subBatch;
	}

	void BatchManager::draw(sdl::Shader& shader) {
	}

	BatchConfig createHexagonImage(Vec2 center, float radius, const sdl::TextureView& sprite, float startAngle) {
		BatchConfig batchConfig;
		if (sprite) {
			Vec2 texSize = Vec2{sprite.getWidth(), sprite.getHeight()} *0.5f;
			Vec2 texPos = Vec2{sprite.getX(), sprite.getY()} +texSize;

			sdl::Vertex centerVertex{center, texPos, WHITE};
			batchConfig.subBatch.pushBack(centerVertex);

			for (int i = 0; i < 6; ++i) {
				auto v = sdl::Vertex{hex::getHexCorner(center, radius, i), texPos + texSize * hex::getHexCorner(i, startAngle), WHITE};
				batchConfig.subBatch.pushBack(v);
			}
			for (int i = 1; i <= 6; ++i) {
				batchConfig.subBatch.pushBackIndex(0);
				batchConfig.subBatch.pushBackIndex(i);
				batchConfig.subBatch.pushBackIndex((i % 6) + 1);
			}
			batchConfig.texture = sprite;
		}
		return batchConfig;
	}

	BatchConfig createHexagon(Vec2 center, float innerRadius, float outerRadius, Color color, float startAngle) {
		BatchConfig batchConfig;

		auto innerCorners = hex::getHexCorners(center, innerRadius, startAngle);
		auto outerCorners = hex::getHexCorners(center, outerRadius, startAngle);

		for (const auto& corner : innerCorners) {
			batchConfig.subBatch.pushBack({corner, {0.f, 0.f}, color});
		}
		for (const auto& corner : outerCorners) {
			batchConfig.subBatch.pushBack({corner, {0.f, 0.f}, color});
		}

		for (int i = 0; i < 6; ++i) {
			batchConfig.subBatch.pushBackIndex(i);
			batchConfig.subBatch.pushBackIndex(6 + i);
			batchConfig.subBatch.pushBackIndex(6 + (i + 1) % 6);
			batchConfig.subBatch.pushBackIndex(i);			
			batchConfig.subBatch.pushBackIndex((i + 1) % 6);
			batchConfig.subBatch.pushBackIndex(6 + (i + 1) % 6);
		}
		return batchConfig;
	}

	BatchConfig createCircle(Vec2 center, float radius, Color color, const int iterations, float startAngle) {
		BatchConfig batchConfig;
		batchConfig.subBatch.pushBack({center, {0.f, 0.f}, color});

		for (int i = 0; i < iterations; ++i) {
			auto rad = 2 * PI * i / iterations + startAngle;
			auto edge = center + glm::rotate(Vec2{radius, 0.f}, rad);

			batchConfig.subBatch.pushBack({edge, {0.f, 0.f}, color});
		}
		for (int i = 1; i <= iterations; ++i) {
			batchConfig.subBatch.pushBackIndex(0);
			batchConfig.subBatch.pushBackIndex(i);
			batchConfig.subBatch.pushBackIndex((i % iterations) + 1);
		}
		return batchConfig;
	}

	BatchConfig createFilledHexagon(Vec2 center, float radius, Color color, float startAngle) {
		return createCircle(center, radius, color, 6, startAngle);
	}

	BatchConfig createRectangle(Vec2 pos, Vec2 size, Color color) {
		BatchConfig batchConfig;
		batchConfig.subBatch.pushBack(sdl::Vertex{pos, {0.f, 0.f}, color});
		batchConfig.subBatch.pushBack(sdl::Vertex{pos + Vec2{size.x, 0.f}, {0.f, 0.f}, color});
		batchConfig.subBatch.pushBack(sdl::Vertex{pos + size, {0.f, 0.f}, color});
		batchConfig.subBatch.pushBack(sdl::Vertex{pos + Vec2{0.f, size.y}, {0.f, 0.f}, color});
		
		batchConfig.subBatch.pushBackIndex(0); 
		batchConfig.subBatch.pushBackIndex(1);
		batchConfig.subBatch.pushBackIndex(2);
		batchConfig.subBatch.pushBackIndex(0);
		batchConfig.subBatch.pushBackIndex(2);
		batchConfig.subBatch.pushBackIndex(3);
		return batchConfig;
	}

} // Namespace vin.
