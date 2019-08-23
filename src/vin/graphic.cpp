#include "graphic.h"

namespace vin {

	namespace {

		Mat44 pop(std::stack<Mat44>& stack) {
			if (stack.empty()) {
				return Mat44(1);
			}
			auto m = stack.top();
			stack.pop();
			return m;
		}

		Mat44 top(const std::stack<Mat44>& stack) {
			if (stack.empty()) {
				return Mat44(1);
			}
			return stack.top();
		}

	}

	Graphic::Graphic() {
	}

	void Graphic::draw() {
		shader_.useProgram();

		for (auto& batchData : batches_) {
			batchData.draw(shader_);
		}
	}

	void Graphic::addRectangle(Vec2 pos, Vec2 size, Color color) {
		if (true || batches_.empty()) {			

			auto it = std::find_if(batches_.begin(), batches_.end(), [&](const BatchData& batchData) {
				return matrixCounter_ == batchData.matrixNbr_ && batchData.texture_ == sdl::Texture();
			});

			BatchData* batchData = nullptr;
			if (it != batches_.end()) {
				batchData = &(*it);
			} else {
				batchData = &(batches_.emplace_back());
				//*batchData = BatchData();
				batchData->matrix_ = top(matrixStack_);
			}
			batchData->batch_.startIndex();
			batchData->batch_.add({
				{pos, {0.f, 0.f}, color},
				{pos + Vec2{size.x, 0.f}, {0.f, 0.f}, color},
				{pos + size, {0.f, 0.f}, color},
				{pos + Vec2{0.f, size.y}, {0.f, 0.f}, color},
			});
			batchData->batch_.addIndexes({
				0, 1, 2, 0, 2, 3
			});
		}
	}

	void Graphic::BatchData::bind(const Shader& shader) {
		if (!initiated_) {
			initiated_ = true;
			vao_.generate();
			vao_.bind();
			batch_.bind();
			shader.setVertexAttribPointer();
		} else {
			vao_.bind();
		}
		if (texture_.isValid()) {
			shader.setTextureId(0);
			texture_.bindTexture();
		} else {
			shader.setTextureId(-1);
		}
	}

	bool Graphic::BatchData::operator==(const BatchData& batchData) const {
		return batchData.texture_ == batchData.texture_
			&& matrixNbr_ == batchData.matrixNbr_;
	}

	void Graphic::BatchData::draw(const Shader& shader) {
		bind(shader);
		batch_.uploadToGraphicCard();
		shader.setMatrix(matrix_);
		batch_.draw();
	}

	void Graphic::clearDraw() {
		for (auto& batchData : batches_) {
			batchData.clearDraw();
		}
	}

	void Graphic::BatchData::clearDraw() {
		batch_.clear();
	}

	void Graphic::pushMatrix(const Mat44& model) {
		matrixStack_.push(model);
		++matrixCounter_;
	}
	
	void Graphic::popMatrix() {
		matrixStack_.pop();
		++matrixCounter_;
	}

} // vin.
