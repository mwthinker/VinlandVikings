#ifndef VINLANDVIKINGS_VIN_TYPES_H
#define VINLANDVIKINGS_VIN_TYPES_H

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>

namespace vin {

	using Mat4 = glm::mat4;
	using Mat3 = glm::mat3;
	using Mat2 = glm::mat2;
	using Vec2 = glm::vec2;
	using Vec3 = glm::vec3;
	using Vec4 = glm::vec4;	

	class Color {
	public:
		Color(glm::vec4 vec) : value(vec) {
		}

		Color(ImColor vec) : value(vec.Value.x, vec.Value.y, vec.Value.z, vec.Value.w) {
		}

		Color(float red, float green, float blue, float alpha = 1.f)
			: value(red, green, blue, alpha) {
		}

		operator glm::vec4() const {
			return value;
		}

		operator ImU32() const {
			return ImGui::ColorConvertFloat4ToU32({value.r, value.g, value.b, value.a});
		}

		operator ImColor() const {
			return {value.x, value.y, value.z, value.w};
		}

	private:
		glm::vec4 value;
	};

	static const Color WHITE = {1.f, 1.f, 1.f, 1.f};
	static const Color BLACK = {0.f, 0.f, 0.f, 1.f};
	static const Color RED = {1.f, 0.f, 0.f, 1.f};
	static const Color GREEN = {0.f, 1.f, 0.f, 1.f};
	static const Color BLUE = {0.f, 0.f, 1.f, 1.f};

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_TYPES_H
