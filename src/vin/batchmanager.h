#ifndef VINLANDVIKINGS_VIN_BATCHMANAGER_H
#define VINLANDVIKINGS_VIN_BATCHMANAGER_H

#include "vertex.h"
#include "textureview.h"
#include "shader.h"

#include <sdl/logger.h>
#include <sdl/batch.h>

#include <functional>
#include <unordered_map>

namespace vin {

	using SubBatch = sdl::SubBatch<Vertex>;
	using Batch = sdl::Batch<Vertex>;
	using BatchView = sdl::BatchView<Vertex>;	

	struct BatchConfig;

	class BatchManager {
	public:
		BatchManager();

		void add(const BatchConfig& batchConfig);

		void draw(Shader& shader);

	private:
		Batch batch_{GL_DYNAMIC_DRAW};
		std::unordered_map<GLuint, SubBatch> batches_;
	};

	struct BatchConfig {
		SubBatch subBatch;
		TextureView texture;
	};

	BatchConfig createHexagonImage(Vec2 center, float radius, const SpriteView& sprite, float startAngle = 0);

	BatchConfig createHexagon(Vec2 center, float innerRadius, float outerRadius, Color color, float startAngle = 0);

	BatchConfig createCircle(Vec2 center, float radius, Color color, const int iterations, float startAngle = 0);

	BatchConfig createFilledHexagon(Vec2 center, float radius, Color color, float startAngle = 0);

	BatchConfig createRectangle(Vec2 pos, Vec2 size, Color color);

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_BATCHMANAGER_H
