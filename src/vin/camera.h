#ifndef VINLANDVIKINGS_VIN_CAMERA_H
#define VINLANDVIKINGS_VIN_CAMERA_H

#include "types.h"

#include <glm/gtx/rotate_vector.hpp>

namespace vin {
	
	class Camera {
	public:
		Mat4 getView() const {
			glm::vec3 eye = lookAtPos_ / 100.f;
			glm::vec3 center = {lookAtPos_.x / 100.f, lookAtPos_.y / 100.f, 0.f};
			eye = glm::rotateX(eye, angle_);
			return glm::lookAt(eye, center, glm::vec3(0.0f, 1.0f, 0.0f));
		}

		void setAngle(float angle) {
			angle_ = angle;
		}

		void setPosition(Vec2 pos) {
			lookAtPos_ = {pos.x, pos.y, 1.f};
		}

		Vec2 getLookAtPosition() const {
			return {lookAtPos_.x, lookAtPos_.y};
		}

		void move(Vec2 delta) {
			lookAtPos_.x = delta.x;
			lookAtPos_.y = delta.y;
		}

	private:
		Vec3 lookAtPos_ = {0, 0, 0};
		float angle_ = 0.f;
	};

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_CAMERA_H
