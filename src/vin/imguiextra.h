#ifndef VINLANDVIKINGS_VIN_IMGUIEXTRA_H
#define VINLANDVIKINGS_VIN_IMGUIEXTRA_H

#include "action.h"
#include "hex/hexagon.h"

#include <sdl/textureview.h>
#include <sdl/sprite.h>
#include <sdl/imguiauxiliary.h>

#include <imgui.h>
#include "types.h"

namespace ImGui {

	bool Checkbox(vin::Action& action, bool active);

	bool MenuItem(vin::Action& action);

	void LoadingBar(vin::Color color1 = vin::Color{0.8f, 0.8f, 0.8f, 1.f},
		vin::Color color2 = vin::Color{0.5f, 0.5f, 0.5f, 1.f},
		const vin::Vec2& size = {150.f, 25.f});

}

#endif
