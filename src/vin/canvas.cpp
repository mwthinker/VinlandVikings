#include "canvas.h"
#include "hexagon.h"
#include "logger.h"

namespace vin {

	namespace {

		const ImGuiWindowFlags ImGuiNoWindow = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove;

		const ImGuiWindowFlags ImGuiNoWindow2 = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		
		void addImageQuad(ImDrawList* drawList, const sdl::Sprite& sprite,
			const ImVec2& pos, ImVec2& size, const ImColor& color) {

			auto [texW, texH] = sprite.getTexture().getSize();

			ImVec2 a = {pos.x, pos.y};
			ImVec2 b = {pos.x + size.x, pos.y};
			ImVec2 c = {pos.x + size.x, pos.y + size.y};
			ImVec2 d = {pos.x, pos.y + size.y};

			ImVec2 uv_c = {sprite.getX() / texW, sprite.getY() / texH};
			ImVec2 uv_d = {(sprite.getX() + sprite.getWidth()) / texW, sprite.getY() / texH};
			ImVec2 uv_a = {(sprite.getX() + sprite.getWidth()) / texW, (sprite.getY() + sprite.getHeight()) / texH};
			ImVec2 uv_b = {sprite.getX() / texW, (sprite.getY() + sprite.getHeight()) / texH};

			drawList->PrimQuadUV(a, b, c, d, uv_a, uv_b, uv_c, uv_d, color);
		}

	}

	void grid(float zoom, float x, float y) {
		ImVec2 p = ImGui::GetCursorScreenPos();

		ImVec2 size = ImGui::GetWindowSize();
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		const ImU32 RED = Color(1.0f, 0.f, 0.f);
		const ImU32 BLUE = Color(0.0f, 0.f, 1.f);
		const ImU32 GREEN = Color(0.0f, 1.f, 0.f);

		const ImU32 GRID_COLOR = Color(0.5f, 0.5f, 0.5f);

		const float innerRadius = 11.f * zoom;
		const float outerRadius = 12.f * zoom;

		const Layout layout(layoutPointy, {outerRadius, outerRadius}, {300.f + x, 300.f + y});

		/*
		for (int i = -10; i < 10; ++i) {
			for (int j = -10; j < 10; ++j) {
				auto pos = hex_to_pixel(layout, Hex(i, j, 0));
				addHexagon(drawList, {pos.x, pos.y}, innerRadius, outerRadius, RED);
			}
		}
		*/

		int mapRadius = 10;
		for (int q = -mapRadius; q <= mapRadius; q++) {
			int r1 = std::max(-mapRadius, -q - mapRadius);
			int r2 = std::min(mapRadius, -q + mapRadius);
			for (int r = r1; r <= r2; r++) {
				Hex hex(q, r);
				auto pos = hexToPixel(layout, hex);
				auto color = GRID_COLOR;
				if (hex.q() == mapRadius || hex.r() == mapRadius || hex.s() == mapRadius) {
					color = WHITE;
				} else if (hex.q() + hex.r() == mapRadius || hex.q() + hex.s() == mapRadius || hex.r() + hex.s() == mapRadius) {
					color = WHITE;
				} else if (hex == HEX_ZERO) {
					color = WHITE;
				}

				if (hex == HEX_Q) {
					color = RED;
				}
				if (hex == HEX_R) {
					color = GREEN;
				}
				if (hex == HEX_S) {
					color = BLUE;
				}

				addHexagon(drawList, {pos.x, pos.y}, innerRadius, outerRadius, color);
			}
		}

	}

	void addHexagon(ImDrawList* drawList, ImVec2 center, float innerSize, float outerSize, ImU32 color) {
		auto v1 = getHexCorner(center, innerSize, 0);
		auto v2 = getHexCorner(center, outerSize, 0);
		for (int i = 0; i < 6; ++i) {
			auto v3 = getHexCorner(center, outerSize, (i + 1) % 6);
			auto v4 = getHexCorner(center, innerSize, (i + 1) % 6);
			drawList->AddTriangleFilled(v1, v2, v4, color);
			drawList->AddTriangleFilled(v4, v2, v3, color);
			v1 = v4;
			v2 = v3;
		}
	}

	Canvas::Canvas() {
		hasFocus_ = false;
		zoom_ = 1.f;
		x_ = 0.f;
		y_ = 0.f;
	}

	void Canvas::draw() {
		ImGui::BeginChild("Canvas");
		for (int n = 0; n < 50; n++)
			ImGui::Text("%04d: Some text", n);

		hasFocus_ = ImGui::IsWindowFocused();

		grid(zoom_, x_, y_);

		ImGui::EndChild();
	}

	void Canvas::update(double deltaTime) {

	}

	void Canvas::eventUpdate(const SDL_Event& windowEvent) {
		if (hasFocus_) {
			switch (windowEvent.type) {
				case SDL_MOUSEWHEEL:
					if (windowEvent.wheel.y > 0) // scroll up
					{
						zoom_ *= 1.1f;
						// Put code for handling "scroll up" here!
						logger()->info("windowEvent.wheel.y: {}", windowEvent.wheel.y);
					} else if (windowEvent.wheel.y < 0) // scroll down
					{
						zoom_ *= 1 / 1.1f;
						// Put code for handling "scroll down" here!
						logger()->info("windowEvent.wheel.y: {}", windowEvent.wheel.y);
					}

					if (windowEvent.wheel.x > 0) // scroll right
					{
						// ...
					} else if (windowEvent.wheel.x < 0) // scroll left
					{
						// ...
					}
					break;
				case SDL_MOUSEMOTION:
					if (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
						x_ += windowEvent.motion.xrel;
						y_ += windowEvent.motion.yrel;
					}
					break;
			}
		}
	}

} // Namespace vin.
