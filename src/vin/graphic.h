#ifndef VINLANDVIKINGS_VIN_GRAPHIC_H
#define VINLANDVIKINGS_VIN_GRAPHIC_H

#include "types.h"
#include "logger.h"

#include <sdl/shader.h>
#include <sdl/vertex.h>
#include <sdl/sprite.h>
#include <sdl/batch.h>
#include <sdl/vertexarrayobject.h>
#include <sdl/textureview.h>

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

		void addHexagonImage(Vec2 center, float radius, const sdl::TextureView& sprite, float startAngle = 0);
		
		void addHexagon(Vec2 center, float innerRadius, float outerRadius, Color color, float startAngle = 0);

		void draw(const sdl::Shader& shader);

		void clearDraw();

    private:
		using Batch = sdl::Batch<sdl::Vertex>;
		using BatchView = sdl::BatchView<sdl::Vertex>;

		struct BatchData {
			BatchData() = default;
			BatchData(BatchView&& batchView, int matrixIndex);
			BatchData(BatchView&& batchView, const sdl::TextureView& texture, int matrixIndex);

			BatchView batchView;
			GLuint texture{};
			int matrixIndex{};
		};

		void bind(const sdl::Shader& shader);

		void draw(const sdl::Shader& shader, const BatchData& batchData);
		
		Batch batch_{GL_DYNAMIC_DRAW};
		BatchView lastView_;
		std::vector<BatchData> batches_;
		std::vector<Mat4> matrixes_;
		sdl::VertexArrayObject vao_;
		int currentMatrix_{};
		bool initiated_{};
    };

}

#endif
