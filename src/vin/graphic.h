#ifndef VINLANDVIKINGS_VIN_GRAPHIC_H
#define VINLANDVIKINGS_VIN_GRAPHIC_H

#include "types.h"
#include "shader.h"
#include "logger.h"
#include "vertex.h"
#include "textureview.h"

#include <sdl/sprite.h>
#include <sdl/batch.h>
#include <sdl/vertexarrayobject.h>
#include <array>
#include <functional>

namespace vin {

    class Graphic {
    public:
		Graphic();

		void setMatrix(const Mat4& matrix);

		void pushMatrix(const Mat4& matrix);

		void addFilledHexagon(Vec2 center, float radius, Color color, float startAngle = 0);

		void addRectangle(Vec2 pos, Vec2 size, Color color);

		void addCircle(Vec2 center, float radius, Color color, const int iterations = 40, float startAngle = 0);

		void draw(Shader& shader);

		void clearDraw();

		void addHexagonImage(Vec2 center, float radius, const SpriteView& sprite, float startAngle = 0);
		void addHexagon(Vec2 center, float innerRadius, float outerRadius, Color color, float startAngle = 0);

    private:
		class BatchData {
		public:
			BatchData() = default;
			BatchData(sdl::BatchView<Vertex>&& batchView, int matrixIndex);
			BatchData(TextureView texture, sdl::BatchView<Vertex>&& batchView, int matrixIndex);

			TextureView texture_;
			sdl::BatchView<Vertex> batchView_;
			int matrixIndex_ = 0;
		};

		using Batch = sdl::Batch<Vertex>;
		using BatchView = sdl::BatchView<Vertex>;

		void bind(Shader& shader);

		void draw(Shader& shader, const BatchData& batchData);
		
		Batch batch_{GL_DYNAMIC_DRAW};
		BatchView lastView_;
		std::vector<BatchData> batches_;
		std::vector<Mat4> matrixes_;
		sdl::VertexArrayObject vao_;
		int currentMatrix_{};
		bool initiated_ = false;
    };	

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_GRAPHIC_H
