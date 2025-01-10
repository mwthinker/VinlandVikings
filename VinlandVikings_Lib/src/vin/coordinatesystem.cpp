#include "coordinatesystem.h"
#include "camera.h"
#include "hex/hexagon.h"

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/intersect.hpp>

#include <algorithm>
#include <optional>

namespace vin {

	namespace {

		Vec4 getScreenPositionOnPlane(float x, float y, const Mat4& worldToScreen) {
			Vec3 orig = Vec3{x, y, 4000.f};
			Vec3 dir = Vec3{0.f, 0.f, -1.f};
			Vec3 planeOrig = worldToScreen * Vec4{0.f, 0.f, 0.f, 1};
			Vec3 planeNormal = glm::normalize(glm::mat3(glm::transpose(glm::inverse(worldToScreen))) * Vec3{0.f, 0.f, 1.f});
			float intersectionDistance;

			if (glm::intersectRayPlane(orig, dir, planeOrig, planeNormal, intersectionDistance)) {
				return Vec4{orig + intersectionDistance * dir, 1};
			}

			return Vec4{0, 0, 0, 1};
		}

	}
		
	CoordinateSystem::CoordinateSystem(HexDimension dimension)
		: hexToWorld_{hex::createHexToCoordModel(dimension.angle, dimension.outerSize)} {

		worldToCamera_ = glm::mat4{1};
		cameraToClip_ = glm::mat4{1};

		setViewport(100, 100, {0, 0, 100, 100});
	}

	void CoordinateSystem::setViewport(int width, int height, const Viewport& viewport) {
		viewport_ = viewport;
		const float x = static_cast<float>(viewport.x);
		const float y = static_cast<float>(viewport.y);
		const float w = static_cast<float>(viewport.w);
		const float h = static_cast<float>(viewport.h);
		const float H = static_cast<float>(height);
		const float aspect = w / h;

		screenToClip_ = glm::ortho(x, x + w, H - y, H - y - h);

		const float delta = 30.f;
		cameraToClip_ = glm::ortho(-delta * aspect, delta * aspect, -delta, delta, -100.f, 100.f);
	}

	hex::Hexi CoordinateSystem::worldToHex(Vec2 pos) const {
		auto hex = glm::inverse(hexToWorld_) * pos;
		return hex::hexRound({hex.x, hex.y});
	}

	hex::Hexi CoordinateSystem::getHexFromScreen(float x, float y) const {
		auto screenPos = getScreenPositionOnPlane(x, y, getMatrix(Space::World, Space::Screen));
		auto pos = getMatrix(Space::Screen, Space::World) * screenPos;// glm::vec4{x, y, 0.f, 1.f};
		return worldToHex({pos.x, pos.y});
	}

	glm::mat4 CoordinateSystem::getMatrix(Space from, Space to) const {
		switch (from) {
			case Space::World:
				switch (to) {
					case Space::Camera:
						return worldToCamera_;
					case Space::Clip:
						return cameraToClip_ * worldToCamera_;
					case Space::World:
						return glm::mat4{1};
					case Space::Screen:
						return glm::inverse(screenToClip_) * cameraToClip_ * worldToCamera_;
				}
				break;
			case Space::Camera:
				switch (to) {
					case Space::Camera:
						return glm::mat4{1};
					case Space::Clip:
						return cameraToClip_;
					case Space::World:
						return glm::inverse(worldToCamera_);
					case Space::Screen:
						return glm::inverse(screenToClip_) * cameraToClip_;
				}
				break;
			case Space::Clip:
				switch (to) {
					case Space::Camera:
						return glm::inverse(cameraToClip_);
					case Space::Clip:
						return glm::mat4{1};
					case Space::World:
						return glm::inverse(worldToCamera_) * glm::inverse(cameraToClip_);
					case Space::Screen:
						return glm::inverse(screenToClip_);
				}
				break;
			case Space::Screen:
				switch (to) {
					case Space::Camera:
						return glm::inverse(cameraToClip_) * screenToClip_;
					case Space::Clip:
						return screenToClip_;
					case Space::World:
						return glm::inverse(worldToCamera_) * glm::inverse(cameraToClip_) * screenToClip_;
					case Space::Screen:
						return glm::mat4{1};
				}
				break;
		}
		return glm::mat4{1};
	}

	void CoordinateSystem::zoom(float scale) {
		worldToCamera_ = glm::scale(worldToCamera_, Vec3{scale, scale, 1});
	}

	void CoordinateSystem::move(float x, float y) {
		worldToCamera_ = glm::translate(worldToCamera_, Vec3{x, y, 0.f});
	}

	void CoordinateSystem::tilt(float angle) {
		worldToCamera_ *= glm::rotate(angle, Vec3{1, 0, 0});
	}

	Vec2 CoordinateSystem::hexToWorld(hex::Hexi pos) const {
		return hexToWorld_ * Vec2{pos.q(), pos.r()};
	}

}
