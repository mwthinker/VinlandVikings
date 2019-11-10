#ifndef VINLANDVIKINGS_VIN_TEXTUREVIEW_H
#define VINLANDVIKINGS_VIN_TEXTUREVIEW_H

#include "logger.h"

#include <sdl/texture.h>

namespace vin {

	class TextureView {
	public:
		TextureView() noexcept = default;
		TextureView(const sdl::Texture& texture) noexcept {
			texture_ = texture.getGlTexture();
		}

		operator GLuint() const noexcept {
			return texture_;
		}		

		void bind() const noexcept {
			glBindTexture(GL_TEXTURE_2D, texture_);
		}

		bool operator==(GLuint texture) const noexcept {
			return texture_ == texture;
		}

		bool operator==(TextureView texture) const noexcept {
			return texture_ == texture.texture_;
		}

		bool operator!=(GLuint texture) const noexcept {
			return texture_ != texture;
		}

		bool operator!=(TextureView texture) const noexcept {
			return texture_ != texture.texture_;
		}

	private:
		GLuint texture_ = 0;
	};

	class SpriteView {
	public:
		SpriteView() noexcept = default;

		SpriteView(TextureView texture, float x, float y, float dx, float dy) noexcept :
			texture_{texture}, x_{x}, y_{y}, dx_{dx}, dy_{dy} {
		}

		SpriteView(SpriteView spriteView, float x, float y, float dx, float dy) noexcept :
			texture_{spriteView.texture_}, x_{x}, y_{y}, dx_{dx}, dy_{dy} {
		}

		SpriteView(const SpriteView& texture) noexcept = default;
		
		SpriteView& operator=(const SpriteView& texture) noexcept = default;

		float getX() const noexcept {
			return x_;
		}

		float getY() const noexcept {
			return y_;
		}

		float getWidth() const noexcept {
			return dx_;
		}

		float getHeight() const noexcept {
			return dy_;
		}

		void bind() const noexcept {
			texture_.bind();
		}

		operator GLuint() const noexcept {
			return texture_;
		}

		operator TextureView() const noexcept {
			return texture_;
		}

	private:
		float x_ = 0;
		float y_ = 0;
		float dx_ = 1;
		float dy_ = 1;
		TextureView texture_;
	};

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_TEXTUREVIEW_H
