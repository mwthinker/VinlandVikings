#include "vinshader.h"
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
				"#version 330 core                                                     \n"
				"                                                                        "
				"uniform mat4 uProj;                                                   \n"
				"uniform mat4 uView;                                                   \n"
				"uniform mat4 uModel;                                                  \n"
				"                                                                        "
				"in vec2 aPos;                                                         \n"
				"in vec2 aTex;                                                         \n"
				"in vec4 aColor;                                                       \n"
				"                                                                        "
				"out vec2 fragTex;                                                     \n"
				"out vec4 fragColor;                                                   \n"
				"                                                                        "
				"void main()                                                           \n"
				"{                                                                     \n"
				"    fragTex = aTex;                                                   \n"
				"    fragColor = aColor;                                               \n"
				"    gl_Position = uProj * uView * uModel * vec4(aPos.xy,0,1);         \n"
				"}                                                                     \n";
		}

		constexpr const GLchar* getFragmentShaderGlsl_330() {
			return
				"#version 330 core                                           \n"
				"                                                              "
				"uniform sampler2D uTexture;                                 \n"
				"                                                              "
				"in vec2 fragTex;                                            \n"
				"in vec4 fragColor;                                          \n"
				"                                                              "
				"out vec4 Out_Color;                                         \n"
				"                                                              "
				"void main()                                                 \n"
				"{                                                           \n"
				"    Out_Color = fragColor * texture(texture, fragTex.st);   \n"
				"}                                                           \n";
		}

		GLuint initiateAttribute(sdl::ShaderProgram& program, const GLchar* attributeName) {
			GLuint loc = program.getAttributeLocation(attributeName);
			if (loc == -1) {
				logger()->warn("[VinShader] shader attribute {} failed to be extracted", attributeName);
			}
			return loc;
		}

		GLuint initiateUniform(sdl::ShaderProgram& program, const GLchar* uniformName) {
			GLuint loc = program.getUniformLocation(uniformName);
			if (loc == -1) {
				logger()->warn("[VinShader] shader uniform {} failed to be extracted", uniformName);
			}
			return loc;
		}

	}

	void VinShader::load() {
		shader_.bindAttribute(A_POS);
		shader_.bindAttribute(A_TEX);
		shader_.bindAttribute(A_COL);
		shader_.loadAndLink(getVertexShaderGlsl_330(), getFragmentShaderGlsl_330());

		shader_.useProgram();

		if (shader_.isLinked()) {
			// Collect the vertex buffer attributes indexes.
			aPos_ = initiateAttribute(shader_, A_POS);
			aTex_ = initiateAttribute(shader_, A_TEX);
			aColor_ = initiateAttribute(shader_, A_COL);

			// Collect the vertex buffer uniforms indexes.
			uProj = initiateUniform(shader_, U_PROJ);
			uView = initiateUniform(shader_, U_VIEW);
			uModel = initiateUniform(shader_, U_MODEL);
			uTexture_ = initiateUniform(shader_, U_TEXTURE);
		} else {
			logger()->warn("[VinShader] failed to create VinShader, shader not linked");
		}
	}

	VinShader::VinShader(VinShader&& other) noexcept :
		shader_(std::move(other.shader_)), aPos_(other.aPos_),
		aTex_(other.aTex_), aColor_(other.aColor_),
		uProj(other.uProj), 
		uView(other.uView),
		uModel(other.uModel),
		uTexture_(other.uTexture_) {
	}

	VinShader& VinShader::operator=(VinShader&& other) noexcept {
		shader_ = std::move(other.shader_);
		aPos_ = other.aPos_;
		aTex_ = other.aTex_;
		aColor_ = other.aColor_;
		
		uProj = other.uProj;
		uView = other.uView;
		uModel = other.uModel;
		uTexture_ = other.uTexture_;
		return *this;
	}

	void VinShader::useProgram() const {
		shader_.useProgram();
	}

	void VinShader::setVertexAttribPointer() const {
		if (shader_.isLinked()) {
			glEnableVertexAttribArray(aPos_);
			glVertexAttribPointer(aPos_, sizeof(Vertex::pos) , GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, pos));
			sdl::checkGlError();

			glEnableVertexAttribArray(aTex_);
			glVertexAttribPointer(aTex_, sizeof(Vertex::tex), GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, tex));
			sdl::checkGlError();

			glEnableVertexAttribArray(aColor_);
			glVertexAttribPointer(aColor_, sizeof(Vertex::color), GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, color));
			sdl::checkGlError();

			/*
			glEnableVertexAttribArray(aTex_);
			glVertexAttribPointer(aTexIndex_, 2, GL_FLOAT, GL_FALSE, sizeof(VinVertex), (GLvoid*)IM_OFFSETOF(ImDrawVert, uv));
			sdl::checkGlError();

			glEnableVertexAttribArray(aColor_);
			glVertexAttribPointer(aColorIndex_, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VinVertex), (GLvoid*)IM_OFFSETOF(ImDrawVert, col));
			sdl::checkGlError();
			*/
		} else {
			logger()->warn("[ImGuiShader] setVertexAttribPointer failed, shader not linked");
		}
	}

	void VinShader::setProj(const Mat44& matrix) const {
		glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void VinShader::setView(const Mat44& matrix) const {
		glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void VinShader::setModel(const Mat44& matrix) const {
		glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void VinShader::setTextureId(GLint textureId) const {
		glUniform1i(uTexture_, textureId);
	}

} // Namespace vin.
