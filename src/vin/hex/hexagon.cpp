#include "hexagon.h"

namespace vin::hex {

	namespace {

		const ImGuiWindowFlags ImGuiNoWindow = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove;

		const ImGuiWindowFlags ImGuiNoWindow2 = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		
		/*
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
		*/

	}

	ImVec2 getHexCorner(ImVec2 center, float size, int nbr) {
		auto rad = PI / 3 * nbr;
		return {center.x + size * std::cos(rad), center.y + size * std::sin(rad)};
	}

	Vec2 getHexCorner(int nbr, float startAngle) {
		return glm::rotate(Vec2{1, 0.f}, PI / 3 * nbr + startAngle);
	}

	Vec2 getHexCorner(Vec2 center, GLfloat size, int nbr, float startAngle) {
		return center + size * getHexCorner(nbr, startAngle);
	}

	std::array<Vec2, 6> getHexCorners(Vec2 center, GLfloat radius, float startAngle) {
		std::array<Vec2, 6> corners;
		for (int i = 0; i < 6; ++i) {
			corners[i] = getHexCorner(center, radius, i, startAngle);
		}
		return corners;
	}

	ImDrawVert createHexCornerVertex(const ImDrawVert& vertex, float size, int nbr) {
		return {getHexCorner(vertex.pos, size, nbr), vertex.uv, vertex.col};
	}

	Hexi hexRound(Hexf h) {
		auto q = (int)round(h.q());
		auto r = (int)round(h.r());
		auto s = (int)round(h.s());
		auto q_diff = std::abs(q - h.q());
		auto r_diff = std::abs(r - h.r());
		auto s_diff = std::abs(s - h.s());
		if (q_diff > r_diff && q_diff > s_diff) {
			q = -r - s;
		} else if (r_diff > s_diff) {
			r = -q - s;
		}
		return {q, r};
	}

} // Namespace vin::hex.
