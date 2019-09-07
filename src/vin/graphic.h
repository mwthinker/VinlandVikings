#ifndef VINLANDVIKINGS_VIN_GRAPHIC_H
#define VINLANDVIKINGS_VIN_GRAPHIC_H

#include "types.h"
#include "shader.h"
#include "logger.h"
#include "vertex.h"

#include <sdl/sprite.h>
#include <sdl/batch.h>
#include <sdl/vertexarrayobject.h>
#include <stack>

namespace vin {

	class BatchData {
	public:
		BatchData() = default;
		BatchData(sdl::BatchView<Vertex>&& batchView) : batchView_(batchView) {
		}

		sdl::Texture texture_;
		sdl::BatchView<Vertex> batchView_;
	};

    class Graphic {
    public:
		Graphic();

		void setMatrix(const Mat4& matrix);

		void addFlatHexagon(Vec2 center, float radius, Color color);

		void addPointyHexagon(Vec2 center, float radius, Color color);

		void addFlatHexagonImage(Vec2 center, float radius, const sdl::Sprite& sprite);
		void addPointyHexagonImage(Vec2 center, float radius, const sdl::Sprite& sprite);
		
		void addFlatHexagon(Vec2 center, float innerRadius, float outerRadius, Color color);
		void addPointyHexagon(Vec2 center, float innerRadius, float outerRadius, Color color);

		void addRectangle(Vec2 pos, Vec2 size, Color color);

		void addCircle(Vec2 center, float radius, Color color, const int iterations = 40, float startAngle = 0);

		void draw();

		void clearDraw();

    private:
		using Batch = sdl::Batch<Vertex>;
		using BatchView = sdl::BatchView<Vertex>;

		void bind();

		void draw(const BatchData& batchData);

		void addHexagonImage(Vec2 center, float radius, const sdl::Sprite& sprite, float startAngle);
		void addHexagon(Vec2 center, float innerRadius, float outerRadius, Color color, float startAngle);

		Shader shader_;
		Batch batch_;
		int lastIndexCounter_ = 0;
		BatchView lastView_;
		std::vector<BatchData> batches_;
		Mat4 matrix_;
		bool initiated_ = false;
		sdl::VertexArrayObject vao_;
    };

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_GRAPHIC_H
