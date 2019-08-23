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

    class Graphic {
    public:
		Graphic();

		void pushMatrix(const Mat44& matrix);
		void popMatrix();

		void addRectangle(Vec2 pos, Vec2 size, Color color);

		void draw();

		void clearDraw();

    private:
		using Batch = sdl::Batch<Vertex>;

		class BatchData {
		public:
			BatchData() : matrix_(1), batch_(GL_TRIANGLES, GL_DYNAMIC_DRAW) {
			}

			sdl::Texture texture_;
			Mat44 matrix_;

			int matrixNbr_ = 1;
			
			Batch batch_;
			sdl::VertexArrayObject vao_;
			bool initiated_ = false;

			void bind(const Shader& shader);
			void draw(const Shader& shader);

			void clearDraw();

			bool operator==(const BatchData& batchData) const;
		};

		Shader shader_;
		std::vector<BatchData> batches_;
		std::stack<Mat44> matrixStack_;
		int matrixCounter_ = 1;
    };

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_GRAPHIC_H
