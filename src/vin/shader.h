#ifndef VINLANDVIKINGS_VIN_SHADER_H
#define VINLANDVIKINGS_VIN_SHADER_H

#include "types.h"

#include <sdl/shaderprogram.h>

namespace vin {
	
	class Shader {
	public:
		Shader() = default;

		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;

		Shader(Shader&& other) noexcept;
		Shader& operator=(Shader&& other) noexcept;		

		void useProgram();

		void setVertexAttribPointer() const;

		void setMatrix(const Mat44& matrix) const;

		void setTextureId(GLint textureId) const;

	private:
		void load();

		sdl::ShaderProgram shaderProgram_;

		// Vertex buffer attributes.
		GLuint aPos_ = -1;
		GLuint aTex_ = -1;
		GLuint aColor_ = -1;

		// Vertex buffer uniforms.
		GLuint uMat_ = -1;
		GLuint uTexture_ = -1;
		GLuint uUseTexture_ = -1;
		
		mutable bool initiated_ = false;
	};

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_SHADER_H