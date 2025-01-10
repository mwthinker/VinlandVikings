#ifndef VINLANDVIKINGS_VIN_COORDINATESYSTEM_H
#define VINLANDVIKINGS_VIN_COORDINATESYSTEM_H

#include "types.h"
#include "hexdimension.h"

namespace vin {

	// In OpenGL coordinates, i.e. lower left screen is Origo and y is positive upwards on the screen.
	struct Viewport {
		int x;
		int y;
		int w;
		int h;
	};
	
	class CoordinateSystem {
	public:
		enum class Space {
			World,
			Camera,
			Clip,
			Screen
		};

		explicit CoordinateSystem(HexDimension dimension);

		void setViewport(int width, int height, const Viewport& viewport);

		hex::Hexi worldToHex(Vec2 pos) const;

		hex::Hexi getHexFromScreen(float x, float y) const;
		glm::mat4 getMatrix(Space from, Space to) const;

		void setCameraToClip(const glm::mat4& cameraToClip) {
			cameraToClip_ = cameraToClip;
		}

		void zoom(float scale);

		void move(float x, float y);

		void tilt(float angle);

		Vec2 hexToWorld(hex::Hexi pos) const;

		glm::mat2 getHexToWorld() const {
			return hexToWorld_;
		}

		const Viewport& getViewport() const {
			return viewport_;
		}

	private:
		Viewport viewport_{};
		glm::mat2 hexToWorld_;
		glm::mat4 screenToClip_;
		glm::mat4 worldToCamera_;
		glm::mat4 cameraToClip_;
	};
}

#endif
