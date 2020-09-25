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

	bool ManButton(const char* idStr, int& nbr, int max, const sdl::TextureView& noMan,
		const sdl::TextureView& man, const vin::Vec2& size, const sdl::Color& color) {
		
		vin::Vec2 p = GetCursorScreenPos();

		ImGui::PushID(idStr);
		int buttons = (nbr > 0 ? nbr : 1);
		ImGui::InvisibleButton("", {size.x * buttons, size.y});
			
		vin::Vec2 delta{};
		vin::Vec2 scaledSize = size;
			
		bool pushed = false;
		if (ImGui::IsItemHovered()) {
			if (ImGui::IsAnyMouseDown()) {
				if (isMouseRightClicked()) {
					nbr = (nbr + 4) % (max + 1);
					pushed = true;
				} else if (isMouseLeftClicked()) {
					nbr = (nbr + 1) % (max + 1);
					pushed = true;
				}
			}

			scaledSize *= 1.2f;
			delta = 0.1f * scaledSize;
		}
		ImGui::PopID();

		auto drawList = ImGui::GetWindowDrawList();
		drawList->PushTextureID((ImTextureID)(intptr_t) noMan);
		buttons = (nbr > 0 ? nbr : 2);
		drawList->PrimReserve(6 * buttons, 4 * buttons);
		
		for (int i = 1; i <= nbr || nbr == 0 && i == 1; ++i) {
			float x = p.x;
			if (nbr != 0) {
				x = p.x + size.x * (i -1);
				Helper::AddImageQuad(man, {x - delta.x, p.y - delta.y}, scaledSize, color);
			} else {
				Helper::AddImageQuad(man, {x - delta.x, p.y - delta.y}, scaledSize, color);
				Helper::AddImageQuad(noMan, {x - delta.x, p.y - delta.y}, scaledSize, color);
			}
		}

		drawList->PopTextureID();
		return pushed;
	}

	void LoadingBar(const sdl::Color& color1, const sdl::Color& color2, const vin::Vec2& size) {
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
