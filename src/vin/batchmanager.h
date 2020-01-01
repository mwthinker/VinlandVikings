#ifndef VINLANDVIKINGS_VIN_BATCHMANAGER_H
#define VINLANDVIKINGS_VIN_BATCHMANAGER_H

#include "types.h"

#include <sdl/vertex.h>
#include <sdl/shader.h>
#include <sdl/logger.h>
#include <sdl/batch.h>
#include <sdl/textureview.h>

#include <functional>
#include <unordered_map>

namespace vin {

	using SubBatch = sdl::SubBatch<sdl::Vertex>;
	using Batch = sdl::Batch<sdl::Vertex>;
	using BatchView = sdl::BatchView<sdl::Vertex>;

	struct BatchConfig;

	class BatchManager {
	public:
		BatchManager();

		void add(const BatchConfig& batchConfig);

		void draw(sdl::Shader& shader);

	private:
		Batch batch_{GL_DYNAMIC_DRAW};
		std::unordered_map<GLuint, SubBatch> batches_;
	};

	struct BatchConfig {
		SubBatch subBatch;
		sdl::TextureView texture;
	};

	BatchConfig createHexagonImage(Vec2 center, float radius, const sdl::TextureView& sprite, float startAngle = 0);

	BatchConfig createHexagon(Vec2 center, float innerRadius, float outerRadius, Color color, float startAngle = 0);

	BatchConfig createCircle(Vec2 center, float radius, Color color, const int iterations, float startAngle = 0);

	BatchConfig createFilledHexagon(Vec2 center, float radius, Color color, float startAngle = 0);

	BatchConfig createRectangle(Vec2 pos, Vec2 size, Color color);

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_BATCHMANAGER_H
