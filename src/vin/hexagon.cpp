#include "canvas.h"

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

} // Namespace vin.
