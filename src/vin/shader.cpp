#include "shader.h"
#include "logger.h"
#include "vertex.h"

#include <glm/gtc/type_ptr.hpp>

namespace vin {

	namespace {

		constexpr const GLchar* A_POS = "aPos";
		constexpr const GLchar* A_TEX = "aTex";
		constexpr const GLchar* A_COL = "aColor";
		
		constexpr const GLchar* U_MAT = "uMat";
		constexpr const GLchar* U_TEXTURE = "uTexture";
		constexpr const GLchar* U_USE_TEXTURE = "uUseTexture";

		constexpr const GLchar* getVertexShaderGlsl_330() {
			return R"(#version 330 core

uniform mat4 uMat;

in vec2 aPos;
in vec2 aTex;
in vec4 aColor;

out vec2 fragTex;
out vec4 fragColor;

void main() {
    fragTex = aTex;
    fragColor = aColor;
    gl_Position = uMat * vec4(aPos.xy, 0, 1);
}
)";
		}

		constexpr const GLchar* getFragmentShaderGlsl_330() {
			return R"(#version 330 core

uniform sampler2D uTexture;
uniform float uUseTexture;

in vec2 fragTex;
in vec4 fragColor;

out vec4 Out_Color;

void main() {
    Out_Color = fragColor * (texture(uTexture, fragTex.st) * uUseTexture + (1 - uUseTexture));
}
)";
		}

	}

	void Shader::load() {
		shaderProgram_.bindAttribute(A_POS);
		shaderProgram_.bindAttribute(A_TEX);
		shaderProgram_.bindAttribute(A_COL);
		shaderProgram_.loadAndLink(getVertexShaderGlsl_330(), getFragmentShaderGlsl_330());

		shaderProgram_.useProgram();

		if (shaderProgram_.isLinked()) {
			// Collect the vertex buffer attributes indexes.
			aPos_ = shaderProgram_.getAttributeLocation(A_POS);
			aTex_ = shaderProgram_.getAttributeLocation(A_TEX);
			aColor_ = shaderProgram_.getAttributeLocation(A_COL);

			// Collect the vertex buffer uniforms indexes.
			uMat_ = shaderProgram_.getUniformLocation(U_MAT);
			uTexture_ = shaderProgram_.getUniformLocation(U_TEXTURE);
			uUseTexture_ = shaderProgram_.getUniformLocation(U_USE_TEXTURE);
		} else {
			logger()->warn("[VinShader] failed to create VinShader, shader not linked");
		}
	}

	Shader::Shader(Shader&& other) noexcept :
		shaderProgram_(std::move(other.shaderProgram_)), aPos_(other.aPos_),
		aTex_(other.aTex_), aColor_(other.aColor_),
		uMat_(other.uMat_),
		uTexture_(other.uTexture_),
		uUseTexture_(other.uUseTexture_) {
	}

	Shader& Shader::operator=(Shader&& other) noexcept {
		shaderProgram_ = std::move(other.shaderProgram_);
		aPos_ = other.aPos_;
		aTex_ = other.aTex_;
		aColor_ = other.aColor_;
		
		uMat_ = other.uMat_;
		uTexture_ = other.uTexture_;
		uUseTexture_ = other.uUseTexture_;
		return *this;
	}

	void Shader::useProgram() {
		if (!initiated_) {
			initiated_ = true;
			load();
		}
		shaderProgram_.useProgram();
	}

	void Shader::setVertexAttribPointer() const {
		if (shaderProgram_.isLinked()) {
			glEnableVertexAttribArray(aPos_);
			glVertexAttribPointer(aPos_, sizeof(Vertex::pos) / sizeof(GLfloat) , GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, pos));
			sdl::assertGlError();

			glEnableVertexAttribArray(aTex_);
			glVertexAttribPointer(aTex_, sizeof(Vertex::tex) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, tex));
			sdl::assertGlError();

			glEnableVertexAttribArray(aColor_);
			glVertexAttribPointer(aColor_, sizeof(Vertex::color) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, color));
			sdl::assertGlError();
		} else {
			logger()->warn("[VinShader] setVertexAttribPointer failed, shader not linked");
		}
	}

	void Shader::setMatrix(const Mat4& matrix) const {
		glUniformMatrix4fv(uMat_, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void Shader::setTextureId(GLint textureId) const {
		if (textureId < 0) {
			glUniform1f(uUseTexture_, 0.f);
		} else {
			glUniform1i(uTexture_, textureId);
			glUniform1f(uUseTexture_, 1.f);
		}
	}

} // Namespace vin.
