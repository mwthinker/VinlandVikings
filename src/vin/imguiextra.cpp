#include "imguiextra.h"

#include "types.h"

#include <sdl/sprite.h>

#include <utility>
#include <glm/gtx/rotate_vector.hpp>

namespace ImGui {

	//void ItemSize(const ImVec2& size, float text_offset_y);

	//struct IMGUI_API ImRect;

	//bool ImGui::ItemAdd(const ImRect& bb, ImGuiID id, const ImRect* nav_bb_arg);

	//void ImGui::PushID(const char* str_id);
	//void ImGui::PopId();

	/*
	ImVec2 operator=(const std::pair<float, float>& pair) {
		return { pair.first, pair.second };
	}
	*/

	namespace {

		bool isMouseLeftClicked() {
			return ImGui::GetIO().MouseClicked[0];
		}

		bool isMouseRightClicked() {
			return ImGui::GetIO().MouseClicked[1];
		}

		void addImageQuad(ImDrawList* drawList, const vin::SpriteView& sprite,
			const ImVec2& pos, ImVec2& size, float angle, const ImColor& color) {

			ImVec2 delta = {size.x * std::cos(angle), size.y * std::sin(angle)};

			vin::Vec2 deltaA(-size.x * 0.5f, -size.y * 0.5f);
			vin::Vec2 deltaB(size.x * 0.5f, -size.y * 0.5f);
			vin::Vec2 deltaC(size.x * 0.5f, size.y * 0.5f);
			vin::Vec2 deltaD(-size.x * 0.5f, size.y * 0.5f);

			deltaA = glm::rotate(deltaA, angle);
			deltaB = glm::rotate(deltaB, angle);
			deltaC = glm::rotate(deltaC, angle);
			deltaD = glm::rotate(deltaD, angle);

			ImVec2 a = {pos.x + deltaA.x, pos.y + deltaA.y};
			ImVec2 b = {pos.x + deltaB.x, pos.y + deltaB.y};
			ImVec2 c = {pos.x + deltaC.x, pos.y + deltaC.y};
			ImVec2 d = {pos.x + deltaD.x, pos.y + deltaD.y};

			ImVec2 uv_c = {sprite.getX(), sprite.getY()};
			ImVec2 uv_d = {sprite.getX() + sprite.getWidth(), sprite.getY()};
			ImVec2 uv_a = {sprite.getX() + sprite.getWidth(), sprite.getY() + sprite.getHeight()};
			ImVec2 uv_b = {sprite.getX(), sprite.getY() + sprite.getHeight()};

			drawList->PrimQuadUV(a, b, c, d, uv_a, uv_b, uv_c, uv_d, color);
		}

		void addCenteredImageQuad(ImDrawList* drawList, const vin::SpriteView& sprite,
			const ImVec2& pos, ImVec2& size, float angle, const ImColor& color) {

			ImVec2 delta = {size.x * std::cos(angle), size.y * std::sin(angle)};

			vin::Vec2 deltaA(-size.x * 0.5f, -size.y * 0.5f);
			vin::Vec2 deltaB(size.x * 0.5f, -size.y * 0.5f);
			vin::Vec2 deltaC(size.x * 0.5f, size.y * 0.5f);
			vin::Vec2 deltaD(-size.x * 0.5f, size.y * 0.5f);

			deltaA = glm::rotate(deltaA, angle);
			deltaB = glm::rotate(deltaB, angle);
			deltaC = glm::rotate(deltaC, angle);
			deltaD = glm::rotate(deltaD, angle);

			ImVec2 a = {pos.x + deltaA.x, pos.y + deltaA.y};
			ImVec2 b = {pos.x + deltaB.x, pos.y + deltaB.y};
			ImVec2 c = {pos.x + deltaC.x, pos.y + deltaC.y};
			ImVec2 d = {pos.x + deltaD.x, pos.y + deltaD.y};

			ImVec2 uv_c = {sprite.getX(), sprite.getY()};
			ImVec2 uv_d = {sprite.getX() + sprite.getWidth(), sprite.getY()};
			ImVec2 uv_a = {sprite.getX() + sprite.getWidth(), sprite.getY() + sprite.getHeight()};
			ImVec2 uv_b = {sprite.getX(), sprite.getY() + sprite.getHeight()};

			drawList->PrimQuadUV(a, b, c, d, uv_a, uv_b, uv_c, uv_d, color);
		}

	}

	IMGUI_API void Bar(float height, const ImColor& color) {
		ImVec2 size = ImGui::GetWindowSize();
		ImGui::GetWindowDrawList()->AddRectFilled({0, 0}, {size.x, height}, color);
	}

	IMGUI_API void Image(const vin::SpriteView& sprite, const ImVec2& size, const ImVec4& tint_col,
		const ImVec4& border_col) {
		Image((ImTextureID)(intptr_t) sprite, size);
	}

	IMGUI_API void Image(const vin::SpriteView& sprite, const ImVec2& pos, ImVec2& size, float rotate, const ImColor& color) {
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		drawList->PushTextureID((ImTextureID)(intptr_t) sprite);
		drawList->PrimReserve(6, 4);
		addImageQuad(drawList, sprite, pos, size, rotate, color);
		drawList->PopTextureID();
	}

	IMGUI_API bool ImageButton(const vin::SpriteView& sprite, const ImVec2& size, const ImVec4& tint_col,
		const ImVec4& border_col) {
		return ImageButton((ImTextureID)(intptr_t) sprite, size);
	}

	IMGUI_API void ImageBackground(const vin::SpriteView& sprite) {
		ImVec2 size = ImGui::GetWindowSize();
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		drawList->PushTextureID((ImTextureID)(intptr_t) sprite);
		drawList->PrimReserve(6, 4);
		addImageQuad(drawList, sprite, {0, 0}, size, 0.f, { 1.f, 1.f, 1.f, 1.f });
		drawList->PopTextureID();
	}

	IMGUI_API bool ManButton(const char* idStr, int& nbr, int max, const vin::SpriteView& noMan,
		const vin::SpriteView& man,	const ImVec2& size, ImColor color) {
		
		ImVec2 p = GetCursorScreenPos();

		ImGui::PushID(idStr);
		int buttons = (nbr > 0 ? nbr : 1);
		ImGui::InvisibleButton("", {size.x * buttons, size.y});
			
		ImVec2 delta = {0, 0};
		ImVec2 scaledSize = size;
			
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

			scaledSize.x *= 1.2f;
			scaledSize.y *= 1.2f;
			delta.x = 0.1f * scaledSize.x;
			delta.y = 0.1f * scaledSize.y;
		}
		ImGui::PopID();

		ImDrawList* drawList = ImGui::GetWindowDrawList();
		drawList->PushTextureID((ImTextureID)(intptr_t) noMan);
		buttons = (nbr > 0 ? nbr : 2);
		drawList->PrimReserve(6 * buttons, 4 * buttons);
		
		for (int i = 1; i <= nbr || nbr == 0 && i == 1; ++i) {
			float x = p.x;
			if (nbr != 0) {
				x = p.x + size.x * (i -1);
				addImageQuad(drawList, man, {x - delta.x, p.y - delta.y}, scaledSize, 0.f, color);
			} else {
				addImageQuad(drawList, man, {x - delta.x, p.y - delta.y}, scaledSize, 0.f, color);
				addImageQuad(drawList, noMan, {x - delta.x, p.y - delta.y}, scaledSize, 0.f, color);
			}
		}

		drawList->PopTextureID();
		return pushed;
	}

	IMGUI_API void LoadingBar(const ImColor& color1, const ImColor& color2, const ImVec2& size) {
		ImGui::Dummy(size);
		auto pos = ImGui::GetCursorPos();
		ImGui::GetWindowDrawList()->AddRectFilled(pos, { pos.x + size.x, pos.y + size.y }, color1);

		static float sign = 1;   // Obs!!!! Only possible with one loading bar at a time.
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
		ImGui::GetWindowDrawList()->AddRectFilled({ x, pos.y }, { x + width, pos.y + size.y }, color2);
	}

} // Namespace ImGui.
