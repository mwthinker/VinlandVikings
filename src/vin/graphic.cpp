#include "graphic.h"
#include "hex/hexagon.h"

#include <sdl/graphic.h>

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/component_wise.hpp>

namespace sdlg = sdl::graphic::indexed;

namespace vin {	

	Graphic::BatchData::BatchData(BatchView&& batchView, int matrixIndex)
		: batchView{batchView}, matrixIndex{matrixIndex} {
	}
	
	Graphic::BatchData::BatchData(BatchView&& batchView, const sdl::TextureView& texture, int matrixIndex)
		: texture{texture}, batchView{batchView}, matrixIndex{matrixIndex} {
	}

	Graphic::Graphic() {
		matrixes_.emplace_back(1.f);
	}

	void Graphic::draw(const sdl::Shader& shader) {
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
		batches_.emplace_back(sdlg::addRectangle(batch_, pos, size, color), currentMatrix_);
	}

	void Graphic::addFilledHexagon(Vec2 center, float radius, Color color, float startAngle) {
		addCircle(center, radius, color, 6, startAngle);
	}

	void Graphic::addHexagonImage(Vec2 center, float radius, const sdl::TextureView& sprite, float startAngle) {
		batches_.emplace_back(sdlg::addHexagonImage(batch_, center, radius, sprite, startAngle), sprite, currentMatrix_);
	}

	void Graphic::addHexagon(Vec2 center, float innerRadius, float outerRadius, Color color, float startAngle) {
		batches_.emplace_back(sdlg::addHexagon(batch_, center, innerRadius, outerRadius, color, startAngle), currentMatrix_);
	}

	void Graphic::addCircle(Vec2 center, float radius, Color color, const int iterations, float startAngle) {
		batches_.emplace_back(sdlg::addCircle(batch_, center, radius, color, iterations, startAngle), currentMatrix_);
	}

	void Graphic::bind(const sdl::Shader& shader) {
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

	void Graphic::draw(const sdl::Shader& shader, const BatchData& batchData) {
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
