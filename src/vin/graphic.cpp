#include "graphic.h"
#include "hex/hexagon.h"

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/component_wise.hpp>

namespace vin {

	namespace {
		
		sdl::BatchView<Vertex> addIndexedRectangle(sdl::Batch<Vertex>& batch, Vec2 pos, Vec2 size, Color color) {
			batch.startBatchView();
			batch.startAdding();

			batch.pushBack(Vertex{pos, {0.f, 0.f}, color});
			batch.pushBack(Vertex{pos + Vec2{size.x, 0.f}, {0.f, 0.f}, color});
			batch.pushBack(Vertex{pos + size, {0.f, 0.f}, color});
			batch.pushBack(Vertex{pos + Vec2{0.f, size.y}, {0.f, 0.f}, color});

			batch.pushBackIndex(0);
			batch.pushBackIndex(1);
			batch.pushBackIndex(2);
			batch.pushBackIndex(0);
			batch.pushBackIndex(2);
			batch.pushBackIndex(3);
			return batch.getBatchView(GL_TRIANGLES);
		}

		sdl::BatchView<Vertex> addIndexedHexagonImage(sdl::Batch<Vertex>& batch, Vec2 center, float radius, const sdl::TextureView& sprite, float startAngle) {
			batch.startBatchView();
			batch.startAdding();

			if (sprite) {
				Vec2 texSize = Vec2{sprite.getWidth(), sprite.getHeight()} * 0.5f;
				Vec2 texPos = Vec2{sprite.getX(), sprite.getY()} + texSize;

				Vertex centerVertex{center, texPos, WHITE};
				batch.pushBack(centerVertex);

				for (int i = 0; i < 6; ++i) {
					auto v = Vertex{hex::getHexCorner(center, radius, i, startAngle), texPos + texSize * hex::getHexCorner(i, 0), WHITE};
					batch.pushBack(v);
				}
				for (int i = 1; i <= 6; ++i) {
					batch.pushBackIndex(0);
					batch.pushBackIndex(i);
					batch.pushBackIndex((i % 6) + 1);
				}
			}

			return batch.getBatchView(GL_TRIANGLES);
		}

		sdl::BatchView<Vertex> addIndexedHexagon(sdl::Batch<Vertex>& batch, Vec2 center, float innerRadius, float outerRadius, Color color, float startAngle) {
			batch.startBatchView();
			batch.startAdding();

			auto innerCorners = hex::getHexCorners(center, innerRadius, startAngle);
			auto outerCorners = hex::getHexCorners(center, outerRadius, startAngle);

			for (const auto& corner : innerCorners) {
				batch.pushBack({corner, {0.f, 0.f}, color});
			}
			for (const auto& corner : outerCorners) {
				batch.pushBack({corner, {0.f, 0.f}, color});
			}

			for (int i = 0; i < 6; ++i) {
				batch.pushBackIndex(i);
				batch.pushBackIndex(6 + i);
				batch.pushBackIndex(6 + (i + 1) % 6);
				batch.pushBackIndex(i);
				batch.pushBackIndex((i + 1) % 6);
				batch.pushBackIndex(6 + (i + 1) % 6);
			}
			return batch.getBatchView(GL_TRIANGLES);
		}

		sdl::BatchView<Vertex> addIndexedCircle(sdl::Batch<Vertex>& batch, Vec2 center, float radius, Color color, const int iterations, float startAngle) {
			batch.startBatchView();
			batch.startAdding();

			batch.pushBack({center, {0.f, 0.f}, color});

			for (int i = 0; i < iterations; ++i) {
				auto rad = 2 * PI * i / iterations + startAngle;
				auto edge = center + glm::rotate(Vec2{radius, 0.f}, rad);

				batch.pushBack({edge, {0.f, 0.f}, color});
			}
			for (int i = 1; i <= iterations; ++i) {
				batch.pushBackIndex(0);
				batch.pushBackIndex(i);
				batch.pushBackIndex((i % iterations) + 1);
			}

			return batch.getBatchView(GL_TRIANGLES);
		}

	}

	Graphic::BatchData::BatchData(BatchView&& batchView, int matrixIndex)
		: batchView{batchView}, matrixIndex{matrixIndex} {
	}
	
	Graphic::BatchData::BatchData(BatchView&& batchView, const sdl::TextureView& texture, int matrixIndex)
		: texture{texture}, batchView{batchView}, matrixIndex{matrixIndex} {
	}

	Graphic::Graphic() {
		matrixes_.emplace_back(1.f);
	}

	void Graphic::draw(Shader& shader) {
		glActiveTexture(GL_TEXTURE1);

		int currentMatrix = currentMatrix_;
		currentMatrix_ = -1;
		if (batch_.getSize() > 0) {
			shader.useProgram();
			bind(shader);
			batch_.uploadToGraphicCard();
			shader.setMatrix(matrixes_.front());
			
			for (auto& batchData : batches_) {
				draw(shader, batchData);
			}
		}
		currentMatrix_ = currentMatrix;
	}

	void Graphic::addRectangle(Vec2 pos, Vec2 size, Color color) {
		batches_.emplace_back(addIndexedRectangle(batch_, pos, size, color), currentMatrix_);
	}

	void Graphic::addFilledHexagon(Vec2 center, float radius, Color color, float startAngle) {
		addCircle(center, radius, color, 6, startAngle);
	}

	void Graphic::addHexagonImage(Vec2 center, float radius, const sdl::TextureView& sprite, float startAngle) {
		batches_.emplace_back(addIndexedHexagonImage(batch_, center, radius, sprite, startAngle), sprite, currentMatrix_);
	}

	void Graphic::addHexagon(Vec2 center, float innerRadius, float outerRadius, Color color, float startAngle) {
		batches_.emplace_back(addIndexedHexagon(batch_, center, innerRadius, outerRadius, color, startAngle), currentMatrix_);
	}

	void Graphic::addCircle(Vec2 center, float radius, Color color, const int iterations, float startAngle) {
		batches_.emplace_back(addIndexedCircle(batch_, center, radius, color, iterations, startAngle), currentMatrix_);
	}

	void Graphic::bind(Shader& shader) {
		if (!initiated_) {
			initiated_ = true;
			vao_.generate();
			vao_.bind();
			batch_.bind();
			shader.setVertexAttribPointer();
		} else {
			vao_.bind();
		}
	}

	void Graphic::draw(Shader& shader, const BatchData& batchData) {
		if (const auto& texture = batchData.texture; texture) {
			shader.setTextureId(1);
			glBindTexture(GL_TEXTURE_2D, texture);
		} else {
			shader.setTextureId(-1);
		}
		if (currentMatrix_ != batchData.matrixIndex) {
			currentMatrix_ = batchData.matrixIndex;
			shader.setMatrix(matrixes_[currentMatrix_]);
		}
		batch_.draw(batchData.batchView);
	}

	void Graphic::clearDraw() {
		batch_.clear();
		batches_.clear();
		matrixes_.clear();
		matrixes_.emplace_back(1.f);
		currentMatrix_ = 0;
	}

	void Graphic::pushMatrix(const Mat4& model) {
		currentMatrix_ = static_cast<int>(matrixes_.size());
		matrixes_.push_back(model);
	}

	void Graphic::setMatrix(const Mat4& model) {
		matrixes_[0] = model;
	}

} // vin.
