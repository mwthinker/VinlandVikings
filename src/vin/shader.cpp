#include "shader.h"
#include "logger.h"
#include "vertex.h"

#include <glm/gtc/type_ptr.hpp>

namespace vin {

	namespace {

		constexpr const GLchar* A_POS = "aPos";
		constexpr const GLchar* A_TEX = "aTex";
		constexpr const GLchar* A_COL = "aColor";

		constexpr const GLchar* U_PROJ = "uProj";
		constexpr const GLchar* U_VIEW = "uView";
		constexpr const GLchar* U_MODEL = "uModel";
		constexpr const GLchar* U_TEXTURE = "uTexture";

		constexpr const GLchar* getVertexShaderGlsl_330() {
			return
				"#version 330 core                                                  \n"
				"                                                                   \n"
				"uniform mat4 uProj;                                                \n"
				"uniform mat4 uView;                                                \n"
				"uniform mat4 uModel;                                               \n"
				"                                                                   \n"
				"in vec2 aPos;                                                      \n"
				"in vec2 aTex;                                                      \n"
				"in vec4 aColor;                                                    \n"
				"                                                                   \n"
				"out vec2 fragTex;                                                  \n"
				"out vec4 fragColor;                                                \n"
				"                                                                   \n"
				"void main()                                                        \n"
				"{                                                                  \n"
				"    fragTex = aTex;                                                \n"
				"    fragColor = aColor;                                            \n"
				"    gl_Position = uProj * uView * uModel * vec4(aPos.xy, 0, 1);    \n"
				"}                                                                  \n";
		}

		constexpr const GLchar* getFragmentShaderGlsl_330() {
			return
				"#version 330 core                                            \n"
				"                                                             \n"
				"uniform sampler2D uTexture;                                  \n"
				"                                                             \n"
				"in vec2 fragTex;                                             \n"
				"in vec4 fragColor;                                           \n"
				"                                                             \n"
				"out vec4 Out_Color;                                          \n"
				"                                                             \n"
				"void main()                                                  \n"
				"{                                                            \n"
				"    Out_Color = fragColor * texture(uTexture, fragTex.st);   \n"
				"}                                                            \n";
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
			uProj = shaderProgram_.getUniformLocation(U_PROJ);
			uView = shaderProgram_.getUniformLocation(U_VIEW);
			uModel = shaderProgram_.getUniformLocation(U_MODEL);
			uTexture_ = shaderProgram_.getUniformLocation(U_TEXTURE);
		} else {
			logger()->warn("[VinShader] failed to create VinShader, shader not linked");
		}
	}

	Shader::Shader(Shader&& other) noexcept :
		shaderProgram_(std::move(other.shaderProgram_)), aPos_(other.aPos_),
		aTex_(other.aTex_), aColor_(other.aColor_),
		uProj(other.uProj), 
		uView(other.uView),
		uModel(other.uModel),
		uTexture_(other.uTexture_) {
	}

	Shader& Shader::operator=(Shader&& other) noexcept {
		shaderProgram_ = std::move(other.shaderProgram_);
		aPos_ = other.aPos_;
		aTex_ = other.aTex_;
		aColor_ = other.aColor_;
		
		uProj = other.uProj;
		uView = other.uView;
		uModel = other.uModel;
		uTexture_ = other.uTexture_;
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
			glVertexAttribPointer(aPos_, sizeof(Vertex::pos) , GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, pos));
			sdl::assertGlError();

			glEnableVertexAttribArray(aTex_);
			glVertexAttribPointer(aTex_, sizeof(Vertex::tex), GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, tex));
			sdl::assertGlError();

			glEnableVertexAttribArray(aColor_);
			glVertexAttribPointer(aColor_, sizeof(Vertex::color), GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, color));
			sdl::assertGlError();
		} else {
			logger()->warn("[VinShader] setVertexAttribPointer failed, shader not linked");
		}
	}

	void Shader::setProj(const Mat44& matrix) const {
		glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void Shader::setView(const Mat44& matrix) const {
		glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void Shader::setModel(const Mat44& matrix) const {
		glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void Shader::setTextureId(GLint textureId) const {
		glUniform1i(uTexture_, textureId);
	}

} // Namespace vin.
