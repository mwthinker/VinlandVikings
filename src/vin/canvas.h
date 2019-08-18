#ifndef VINLANDVIKINGS_VIN_CANVAS_H
#define VINLANDVIKINGS_VIN_CANVAS_H

#include "types.h"
#include "hexagonbatch.h"
#include "hexagon.h"
#include "hextilemap.h"
#include "heximage.h"
#include "shader.h"

#include <sdl/sprite.h>
#include <sdl/vertexarrayobject.h>

#include "logger.h"

#include <imgui.h>

#include <unordered_set>

namespace vin {

	void addHexagon(ImDrawList* drawList, ImVec2 center, float innerSize, float outerSize, ImU32 color);

	void HexagonImage(const sdl::Sprite& image, ImVec2 pos, ImVec2 size, float angle);

	class ICamera {

	};

	class Camera {
	public:
		Mat44 getView() const {
			glm::vec3 eye = lookAtPos_ / 100.f;
			//eye.z *= zoom_;
			
			glm::vec3 center = {lookAtPos_.x / 100.f, lookAtPos_.y / 100.f, 0.f};
			eye = glm::rotateX(eye, angle_);

			//logger()->info("eye: ({},{},{})", eye.x, eye.y, eye.z);

			return glm::lookAt(eye, center, glm::vec3(0.0f, 1.0f, 0.0f));
			//auto center = cameraPosition;
			//center.z = 0;
			//return glm::lookAt(cameraPosition, center, cameraUp);
		}

		void setZoom(float zoom) {
			zoom_ = zoom;
		}

		void setAngle(float angle) {
			angle_ = angle;
		}

		void setPosition(Vec2 pos) {
			lookAtPos_ = {pos.x, pos.y, 1.f};
		}

		void move(Vec2 delta) {
		}

		Vec3 lookAtPos_ = {0, 0, 0};
	private:
		float zoom_ = 1.f;
		float angle_ = 0.f;
	};

    class Canvas {
    public:
		Canvas();

		void update(float width, float height, const sdl::ImGuiShader& imGuiShader, double deltaTime);

		void draw();

		void eventUpdate(const SDL_Event& windowEvent);

		void activateHexagon(const HexImage& hexImage) {
			hexImage_ = hexImage;
			activateHexagon_ = true;
			//rotations_ = rotations;
		}

		void inactivateHexagon() {
			activateHexagon_ = false;
			rotations_ = 0;
		}

		const sdl::Sprite& currentHexSprite() const {
			return hexImage_.getImage();
		}

		void init(const sdl::ImGuiShader& imGuiShader);

		void drawHexImage(const sdl::ImGuiShader& imGuiShader, Hexi hex, const HexImage& image);

    private:
		Hexi getHexFromMouse() const;

		sdl::Texture whiteSquare_;
		float zoom_;
		float x_, y_;
		bool hasFocus_;
		bool activateHexagon_;
		HexImage hexImage_;
		//float imageAngle_;
		HexTileMap hexTileMap_;
		HexagonBatch hexagonBatch_;
		Shader shader_;

		HexTile lastHexTile_;
		bool lastAllowed_;
		std::unordered_map<Hexi, HexImage> hexImages_;
		int rotations_;
		Camera camera_;
		float angle_ =  0.f;
    };

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_HEXAGONBATCH_H
