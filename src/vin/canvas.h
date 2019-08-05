#ifndef VINLANDVIKINGS_VIN_CANVAS_H
#define VINLANDVIKINGS_VIN_CANVAS_H

#include "types.h"
#include "hexagonbatch.h"
#include "hexagon.h"
#include "hextilemap.h"
#include "heximage.h"

#include <sdl/sprite.h>
#include <sdl/vertexarrayobject.h>

#include <imgui.h>

#include <unordered_set>

namespace vin {

	void addHexagon(ImDrawList* drawList, ImVec2 center, float innerSize, float outerSize, ImU32 color);

	void HexagonImage(const sdl::Sprite& image, ImVec2 pos, ImVec2 size, float angle);

	class Camera {
	public:
		Mat44 getModel() const {
			Mat44 proj = glm::ortho(-10.f, 10.f, -10.f, 10.f);
			return glm::ortho(-10.f, 10.f, -10.f, 10.f) * glm::translate(proj, Vec3{0, 0, 0});

			//auto center = cameraPosition;
			//center.z = 0;
			//return glm::lookAt(cameraPosition, center, cameraUp);
		}

		void setPosition(Vec2 pos) {

		}

		void move(Vec2 delta) {
		}

		void zoom(float value) {

		}


		Vec3 cameraPosition = {0, 0, 3};
		Vec3 cameraFront = {0, 0, -1};
		Vec3 cameraUp = {0, 1, 0};
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

		HexTile lastHexTile_;
		bool lastAllowed_;
		std::unordered_map<Hexi, HexImage> hexImages_;
		int rotations_;
		Camera camera_;
    };

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_HEXAGONBATCH_H
