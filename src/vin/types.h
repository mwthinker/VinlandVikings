#ifndef VINLANDVIKINGS_VIN_TYPES_H
#define VINLANDVIKINGS_VIN_TYPES_H

#include <glm/glm.hpp>
#include <glm/mat4x2.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>

namespace vin {

	using Mat44 = glm::mat4x4;
	using Vec2 = glm::vec2;
	using Vec3 = glm::vec3;
	using Vec4 = glm::vec4;	

	class Color {
	public:
		Color(glm::vec4 vec) : value(vec) {
		}

		Color(ImColor vec) : value(vec.Value.x, vec.Value.y, vec.Value.z, vec.Value.w) {
		}

		Color(float red = 0, float green = 0, float blue = 0, float alpha = 0)
			: value(red, green, blue, alpha) {
		}

		operator glm::vec4() const {
			return value;
		}

		operator ImColor() const {
			return {value.x, value.y, value.z, value.w};
		}

	private:
		glm::vec4 value;
	};

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_TYPES_H
