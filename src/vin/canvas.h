#ifndef VINLANDVIKINGS_VIN_CANVAS_H
#define VINLANDVIKINGS_VIN_CANVAS_H

#include "types.h"
#include "hexagonbatch.h"
#include "hexagon.h"
#include "hextilemap.h"
#include "heximage.h"
#include "shader.h"
#include "graphic.h"
#include "camera.h"

#include <sdl/sprite.h>
#include <sdl/vertexarrayobject.h>

#include "logger.h"

#include <imgui.h>

#include <unordered_set>

namespace vin {

	void addHexagon(ImDrawList* drawList, ImVec2 center, float innerSize, float outerSize, ImU32 color);

	void HexagonImage(const sdl::Sprite& image, ImVec2 pos, ImVec2 size, float angle);	

    class Canvas {
    public:
		Canvas();

		void drawCanvas(double deltaTime);

		void drawImgui();

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
		void addGrid();
		void addGridImages();
		void updateCanvasSize();

		Vec2 screenPosToWorld(Vec2 pos);

		Hexi getHexFromMouse() const;

		sdl::Texture whiteSquare_;
		float zoom_ = 1.f;
		bool hasFocus_ = false;;
		bool activateHexagon_;
		HexImage hexImage_;
		HexTileMap hexTileMap_;
		HexagonBatch hexagonBatch_;
		Shader shader_;
		Graphic graphic_;

		HexTile lastHexTile_;
		bool lastAllowed_ = false;
		std::unordered_map<Hexi, HexImage> hexImages_;
		int rotations_ = 0;
		
		Camera camera_;

		Mat2 hexModel_;
		Vec2 windowSize_ = {0.f ,0.f};
		Vec2 windowPos_ = {0.f, 0.f};
		Mat4 projection_;
    };

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_HEXAGONBATCH_H
