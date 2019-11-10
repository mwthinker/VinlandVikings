#include "graphic.h"
#include "hexagon.h"

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/component_wise.hpp>

namespace vin {

	namespace {

		constexpr GLenum VBO_USAGE = GL_DYNAMIC_DRAW;

	}

	Graphic::BatchData::BatchData(sdl::BatchView<Vertex>&& batchView, int matrixIndex)
		: batchView_(batchView), matrixIndex_(matrixIndex) {
	}

	Graphic::BatchData::BatchData(TextureView texture, sdl::BatchView<Vertex>&& batchView, int matrixIndex)
		: texture_(texture), batchView_(batchView), matrixIndex_(matrixIndex) {
	}

	Graphic::Graphic() : batch_(VBO_USAGE) {
		matrixes_.emplace_back(1.f);
	}

	void Graphic::draw() {
		glActiveTexture(GL_TEXTURE1);

		if (batch_.getSize() > 0) {
			shader_.useProgram();
			bind();
			batch_.uploadToGraphicCard();
			currentMatrix_ = 0;
			shader_.setMatrix(matrixes_.front());
			
			for (auto& batchData : batches_) {
				draw(batchData);
			}
		}
	}

	void Graphic::addRectangle(Vec2 pos, Vec2 size, Color color) {
		batch_.startBatchView();
		batch_.startAdding();
		
		batch_.add(
			Vertex{pos, {0.f, 0.f}, color},
			Vertex{pos + Vec2{size.x, 0.f}, {0.f, 0.f}, color},
			Vertex{pos + size, {0.f, 0.f}, color},
			Vertex{pos + Vec2{0.f, size.y}, {0.f, 0.f}, color}
		);
		batch_.addIndexes(0, 1, 2, 0, 2, 3);
		batches_.emplace_back(batch_.getBatchView(GL_TRIANGLES), currentMatrix_);
	}

	void Graphic::addFilledHexagon(Vec2 center, float radius, Color color) {
		addCircle(center, radius, color, 6);
	}

	void Graphic::addHexagonImage(Vec2 center, float radius, const SpriteView& sprite, float startAngle) {
		batch_.startBatchView();
		batch_.startAdding();

		if (sprite) {
			Vec2 texSize = Vec2{sprite.getWidth(), sprite.getHeight()} * 0.5f;
			Vec2 texPos = Vec2{sprite.getX(), sprite.getY()} + texSize;

			Vertex centerVertex{center, texPos, WHITE};
			batch_.pushBack(centerVertex);

			for (int i = 0; i < 6; ++i) {
				auto v = Vertex{getHexCorner(center, radius, i), texPos + texSize * getHexCorner(i, startAngle), WHITE};
				batch_.pushBack(v);
			}
			for (int i = 1; i <= 6; ++i) {
				batch_.addIndexes(0, i, (i % 6) + 1);
			}
		}

		auto& batchData = batches_.emplace_back(sprite, batch_.getBatchView(GL_TRIANGLES), currentMatrix_);
	}

	void Graphic::addHexagon(Vec2 center, float innerRadius, float outerRadius, Color color, float startAngle) {
		batch_.startBatchView();
		batch_.startAdding();

		auto innerCorners = getHexCorners(center, innerRadius, startAngle);
		auto outerCorners = getHexCorners(center, outerRadius, startAngle);

		for (const auto& corner : innerCorners) {
			batch_.pushBack({corner, {0.f, 0.f}, color});
		}
		for (const auto& corner : outerCorners) {
			batch_.pushBack({corner, {0.f, 0.f}, color});
		}

		for (int i = 0; i < 6; ++i) {
			batch_.addIndexes(i, 6 + i, 6 + (i + 1) % 6,
				i, (i + 1) % 6, 6 + (i + 1) % 6);
		}
		batches_.emplace_back(batch_.getBatchView(GL_TRIANGLES), currentMatrix_);
	}

	void Graphic::addCircle(Vec2 center, float radius, Color color, const int iterations, float startAngle) {
		batch_.startBatchView();
		batch_.startAdding();
		
		batch_.pushBack({center, {0.f, 0.f}, color});

		for (int i = 0; i < iterations; ++i) {
			auto rad = 2 * PI * i / iterations + startAngle;
			auto edge = center + glm::rotate(Vec2{radius, 0.f}, rad);

			batch_.pushBack({edge, {0.f, 0.f}, color});
		}
		for (int i = 1; i <= iterations; ++i) {
			batch_.addIndexes(0, i, (i % iterations) + 1);
		}

		batches_.emplace_back(batch_.getBatchView(GL_TRIANGLES), currentMatrix_);
	}

	void Graphic::bind() {
		if (!initiated_) {
			initiated_ = true;
			vao_.generate();
			vao_.bind();
			batch_.bind();
			shader_.setVertexAttribPointer();
		} else {
			vao_.bind();
		}
	}

	void Graphic::draw(const BatchData& batchData) {
		if (const auto& texture = batchData.texture_; texture) {
			shader_.setTextureId(1);
			texture.bind();
		} else {
			shader_.setTextureId(-1);
		}
		if (currentMatrix_ != batchData.matrixIndex_) {
			currentMatrix_ = batchData.matrixIndex_;
			shader_.setMatrix(matrixes_[currentMatrix_]);
		}
		batch_.draw(batchData.batchView_);
	}

	void Graphic::clearDraw() {
		batch_.clear();
		batches_.clear();
		matrixes_.clear();
	}

	void Graphic::pushMatrix(const Mat4& model) {
		currentMatrix_ = static_cast<int>(matrixes_.size());
		matrixes_.push_back(model);
	}

} // vin.
