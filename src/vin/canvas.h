#ifndef VINLANDVIKINGS_VIN_CANVAS_H
#define VINLANDVIKINGS_VIN_CANVAS_H

#include "types.h"
#include "hexagonbatch.h"
#include "hex/hexsideskey.h"
#include "hex/hexagon.h"
#include "hex/tileboard.h"
#include "heximage.h"
#include "shader.h"
#include "graphic.h"
#include "camera.h"
#include "tilesgraphic.h"
#include "hex/mapgenerator.h"
#include "tilelexicon.h"

#include <sdl/sprite.h>
#include <sdl/vertexarrayobject.h>

#include "logger.h"

#include <imgui.h>

#include <unordered_set>

namespace vin {

	struct ViewPort {
		Vec2 pos;
		Vec2 size;
	};

	class Canvas {
	public:
		virtual ~Canvas() = default;

		virtual void drawImgui() = 0;

		virtual void drawCanvas(double deltaTime) = 0;

		void eventUpdate(const SDL_Event& windowEvent);

		void updateCanvasSize();

		Mat4 getProjection() const;

	private:
		bool hasFocus_ = false;;
		bool isHovering_ = false;;
		Mat4 projection_{1};
		ViewPort viewPort_{Vec2{0, 0}, Vec2{0 ,0}};
	};

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_CANVAS_H
