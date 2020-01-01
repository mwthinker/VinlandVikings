#include "imguiextra.h"

#include "types.h"
#include "vertex.h"

#include <sdl/sprite.h>
#include <sdl/sprite.h>
#include <utility>
#include <glm/gtx/rotate_vector.hpp>

namespace ImGui {

	namespace {

		bool isMouseLeftClicked() {
			return ImGui::GetIO().MouseClicked[0];
		}

		bool isMouseRightClicked() {
			return ImGui::GetIO().MouseClicked[1];
		}

		void addHexagon(ImVec2 center, float innerSize, float outerSize, ImU32 color) {
			auto drawList = ImGui::GetWindowDrawList();
			auto v1 = vin::hex::getHexCorner(center, innerSize, 0);
			auto v2 = vin::hex::getHexCorner(center, outerSize, 0);
			for (int i = 0; i < 6; ++i) {
				auto v3 = vin::hex::getHexCorner(center, outerSize, (i + 1) % 6);
				auto v4 = vin::hex::getHexCorner(center, innerSize, (i + 1) % 6);
				drawList->AddTriangleFilled(v1, v2, v4, color);
				drawList->AddTriangleFilled(v4, v2, v3, color);
				v1 = v4;
				v2 = v3;
			}
		}

		void addImageQuad(const sdl::TextureView& sprite,
			const ImVec2& pos, ImVec2& size, float angle, const ImColor& color) {

			auto drawList = ImGui::GetWindowDrawList();
			ImVec2 delta{size.x * std::cos(angle), size.y * std::sin(angle)};

			vin::Vec2 deltaA{-size.x * 0.5f, -size.y * 0.5f};
			vin::Vec2 deltaB{size.x * 0.5f, -size.y * 0.5f};
			vin::Vec2 deltaC{size.x * 0.5f, size.y * 0.5f};
			vin::Vec2 deltaD{-size.x * 0.5f, size.y * 0.5f};

			deltaA = glm::rotate(deltaA, angle);
			deltaB = glm::rotate(deltaB, angle);
			deltaC = glm::rotate(deltaC, angle);
			deltaD = glm::rotate(deltaD, angle);

			ImVec2 a{pos.x + deltaA.x, pos.y + deltaA.y};
			ImVec2 b{pos.x + deltaB.x, pos.y + deltaB.y};
			ImVec2 c{pos.x + deltaC.x, pos.y + deltaC.y};
			ImVec2 d{pos.x + deltaD.x, pos.y + deltaD.y};

			ImVec2 uv_c{sprite.getX(), sprite.getY()};
			ImVec2 uv_d{sprite.getX() + sprite.getWidth(), sprite.getY()};
			ImVec2 uv_a{sprite.getX() + sprite.getWidth(), sprite.getY() + sprite.getHeight()};
			ImVec2 uv_b{sprite.getX(), sprite.getY() + sprite.getHeight()};

			drawList->PrimQuadUV(a, b, c, d, uv_a, uv_b, uv_c, uv_d, color);
		}

		void addCenteredImageQuad(const sdl::TextureView& sprite,
			const ImVec2& pos, ImVec2& size, float angle, const ImColor& color) {

			auto drawList = ImGui::GetWindowDrawList();
			ImVec2 delta{size.x * std::cos(angle), size.y * std::sin(angle)};

			vin::Vec2 deltaA{-size.x * 0.5f, -size.y * 0.5f};
			vin::Vec2 deltaB{size.x * 0.5f, -size.y * 0.5f};
			vin::Vec2 deltaC{size.x * 0.5f, size.y * 0.5f};
			vin::Vec2 deltaD{-size.x * 0.5f, size.y * 0.5f};

			deltaA = glm::rotate(deltaA, angle);
			deltaB = glm::rotate(deltaB, angle);
			deltaC = glm::rotate(deltaC, angle);
			deltaD = glm::rotate(deltaD, angle);

			ImVec2 a{pos.x + deltaA.x, pos.y + deltaA.y};
			ImVec2 b{pos.x + deltaB.x, pos.y + deltaB.y};
			ImVec2 c{pos.x + deltaC.x, pos.y + deltaC.y};
			ImVec2 d{pos.x + deltaD.x, pos.y + deltaD.y};

			ImVec2 uv_c{sprite.getX(), sprite.getY()};
			ImVec2 uv_d{sprite.getX() + sprite.getWidth(), sprite.getY()};
			ImVec2 uv_a{sprite.getX() + sprite.getWidth(), sprite.getY() + sprite.getHeight()};
			ImVec2 uv_b{sprite.getX(), sprite.getY() + sprite.getHeight()};

			drawList->PrimQuadUV(a, b, c, d, uv_a, uv_b, uv_c, uv_d, color);
		}
			
		void primWriteVtx(ImDrawList* drawList, const vin::Vertex& vertex) {
			drawList->PrimWriteVtx({vertex.pos.x, vertex.pos.y}, {vertex.tex.x, vertex.tex.y}, vertex.color.toImU32());
		}

		void addHexagonImage(vin::Vec2 center, float radius, const sdl::TextureView& sprite, float startAngle) {
			auto texSize = vin::Vec2{sprite.getWidth(), sprite.getHeight()} *0.5f;
			auto texPos = vin::Vec2{sprite.getX(), sprite.getY()} + texSize;

			auto drawList = ImGui::GetWindowDrawList();
			for (int i = 1; i <= 6; ++i) {
				drawList->PrimWriteIdx(drawList->_VtxCurrentIdx + 0);
				drawList->PrimWriteIdx(drawList->_VtxCurrentIdx + i);
				drawList->PrimWriteIdx(drawList->_VtxCurrentIdx + (i % 6) + 1);
			}

			vin::Vertex centerVertex{center, texPos, vin::WHITE};		
			primWriteVtx(drawList, centerVertex);

			for (int i = 0; i < 6; ++i) {
				auto v = vin::Vertex{vin::hex::getHexCorner(center, radius, i, 0), texPos + texSize * vin::hex::getHexCorner(i, startAngle), vin::WHITE};
				primWriteVtx(drawList, v);
			}
		}

	}

	bool Checkbox(vin::Action& action, bool active) {
		if (Checkbox(action.getMenuName(), &active)) {
			action();
			return true;
		}
		return false;
	}

	bool MenuItem(vin::Action& action) {
		if (MenuItem(action.getMenuName())) {
			action();
			return true;
		}
		return false;
	}

	void Bar(float height, const ImColor& color) {
		ImVec2 size = ImGui::GetWindowSize();
		ImGui::GetWindowDrawList()->AddRectFilled({0, 0}, {size.x, height}, color);
	}

	void Hexagon(const sdl::TextureView& sprite, const ImVec2& size, bool flat) {
		auto drawList = ImGui::GetWindowDrawList();
		drawList->PushTextureID((ImTextureID) (intptr_t) sprite);
		drawList->PrimReserve(6 * 3, 7);
		
		float angle = vin::PI / 2.f;
		if (flat) {
			angle = 0.f;
		}

		auto pos = ImGui::GetCursorScreenPos();
		addHexagonImage({pos.x + size.x * 0.5f, pos.y + size.x * 0.5f}, size.x * 0.5f, sprite, angle);
		drawList->PopTextureID();
		ImGui::Dummy(size);
	}

	void Image(const sdl::TextureView& sprite, const ImVec2& size, const ImVec4& tint_col,
		const ImVec4& border_col) {
		Image((ImTextureID) (intptr_t) sprite, size, {sprite.getX(), sprite.getY()}, {sprite.getWidth(), sprite.getHeight()}, tint_col, border_col);
	}

	bool ImageButton(const sdl::TextureView& sprite, const ImVec2& size) {
		return ImageButton((ImTextureID)(intptr_t) sprite, size, {sprite.getX(), sprite.getY()}, {sprite.getX() + sprite.getWidth(), sprite.getY() + sprite.getHeight()});
	}

	void ImageBackground(const sdl::TextureView& sprite) {
		auto size = ImGui::GetWindowSize();
		auto drawList = ImGui::GetWindowDrawList();
		drawList->PushTextureID((ImTextureID) (intptr_t) sprite);
		drawList->PrimReserve(6, 4);
		addImageQuad(sprite, {0, 0}, size, 0.f, {1.f, 1.f, 1.f, 1.f});
		drawList->PopTextureID();
	}

	bool ManButton(const char* idStr, int& nbr, int max, const sdl::TextureView& noMan,
		const sdl::TextureView& man, const ImVec2& size, ImColor color) {
		
		ImVec2 p = GetCursorScreenPos();

		ImGui::PushID(idStr);
		int buttons = (nbr > 0 ? nbr : 1);
		ImGui::InvisibleButton("", {size.x * buttons, size.y});
			
		ImVec2 delta{0, 0};
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

		auto drawList = ImGui::GetWindowDrawList();
		drawList->PushTextureID((ImTextureID)(intptr_t) noMan);
		buttons = (nbr > 0 ? nbr : 2);
		drawList->PrimReserve(6 * buttons, 4 * buttons);
		
		for (int i = 1; i <= nbr || nbr == 0 && i == 1; ++i) {
			float x = p.x;
			if (nbr != 0) {
				x = p.x + size.x * (i -1);
				addImageQuad(man, {x - delta.x, p.y - delta.y}, scaledSize, 0.f, color);
			} else {
				addImageQuad(man, {x - delta.x, p.y - delta.y}, scaledSize, 0.f, color);
				addImageQuad(noMan, {x - delta.x, p.y - delta.y}, scaledSize, 0.f, color);
			}
		}

		drawList->PopTextureID();
		return pushed;
	}

	void LoadingBar(const ImColor& color1, const ImColor& color2, const ImVec2& size) {
		ImGui::Dummy(size);
		auto pos = ImGui::GetCursorPos();
		ImGui::GetWindowDrawList()->AddRectFilled(pos, { pos.x + size.x, pos.y + size.y }, color1);

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
		ImGui::GetWindowDrawList()->AddRectFilled({ x, pos.y }, { x + width, pos.y + size.y }, color2);
	}

} // Namespace ImGui.
