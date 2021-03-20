#include "imguiextra.h"

#include "types.h"

#include <sdl/vertex.h>
#include <sdl/sprite.h>
#include <utility>
#include <glm/gtx/rotate_vector.hpp>

namespace ImGui {

	namespace {

		bool isMouseLeftClicked() {
			return ImGui::GetIO().MouseClicked[ImGuiMouseButton_Left];
		}

		bool isMouseRightClicked() {
			return ImGui::GetIO().MouseClicked[ImGuiMouseButton_Right];
		}

	}

	bool Checkbox(vin::Action& action, bool active) {
		if (Checkbox(action.getCheckboxMenuName(), &active)) {
			action();
			return true;
		}
		return false;
	}

	bool MenuItem(vin::Action& action) {
		if (MenuItem(action.getMenuName(), action.getShortcut())) {
			action();
			return true;
		}
		return false;
	}

	void LoadingBar(vin::Color color1, vin::Color color2, const vin::Vec2& size) {
		ImGui::Dummy(size);
		auto pos = ImGui::GetCursorPos();
		ImGui::GetWindowDrawList()->AddRectFilled(pos, { pos.x + size.x, pos.y + size.y }, color1.toImU32());

		static float sign = 1; // Obs!!!! Only possible with one loading bar at a time.
		static float x = pos.x;
		
		const float speed = 100.f;
		x += sign * ImGui::GetIO().DeltaTime * speed;
		const float width = size.x * 0.3f;
		if (x > pos.x + size.x - width) {
			sign = -1.f;
			x = pos.x + size.x - width;
		}
		if (x < pos.x) {
			sign = 1.f;
			x = pos.x;
		}
		ImGui::GetWindowDrawList()->AddRectFilled({ x, pos.y }, { x + width, pos.y + size.y }, color2.toImU32());
	}

}
