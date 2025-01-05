#include "camera.h"

#include <glm/gtx/rotate_vector.hpp>
#include <algorithm>

namespace vin {
		
	constexpr Vec3 Up{0.0f, 1.0f, 0.0f};

	Mat4 Camera::getView() const {
		return glm::lookAt(getEye(), getCenter(), Up);
	}

	Vec3 Camera::getEye() const {
		return getCenter() + glm::rotateX(Vec3{0, 0, 10.f}, angle_);
	}

	Vec3 Camera::getCenter() const {
		return {lookAtPos_, 0.f};
	}

	void Camera::setAngle(float angle) {
		angle_ = angle;
		angle_ = std::clamp(angle_, 0.f, Pi / 2 - Pi / 9);
	}

	void Camera::setPosition(Vec2 pos) {
		lookAtPos_ = pos;
	}

	Vec2 Camera::getLookAtPosition() const {
		return lookAtPos_;
	}

	void Camera::move(Vec2 delta) {
		lookAtPos_ += delta;
	}

	void Camera::angleDelta(float delta) {
		angle_ += delta;
		angle_ = std::clamp(angle_, 0.f, Pi / 2 - Pi / 9);
	}

}
