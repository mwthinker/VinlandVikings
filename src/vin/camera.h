#ifndef VINLANDVIKINGS_VIN_CAMERA_H
#define VINLANDVIKINGS_VIN_CAMERA_H

#include "types.h"

namespace vin {
	
	class Camera {
	public:
		Mat4 getView() const;

		void setAngle(float angle);

		void setPosition(Vec2 pos);

		Vec2 getLookAtPosition() const;

		void move(Vec2 delta);

		void angleDelta(float delta);

		Vec3 getEye() const;

	private:
		Vec3 getCenter() const;

		Vec2 lookAtPos_ = {0, 0};
		float angle_ = 0.f;
	};

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_CAMERA_H
