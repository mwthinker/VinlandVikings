#include "hexagonbatch.h"

#include <sdl/opengl.h>

#include <imgui.h>
#include <glm/gtc/constants.hpp>

namespace vin {

    namespace {

        const auto PI = glm::pi<GLfloat>();

		inline ImDrawVert createVertex(float x, float y, float xTex, float yTex, ImU32 color) {
			return  {{x, y}, ImVec2(1.f, 1.f), color};
		}

        inline ImDrawVert createVertex(float x, float y, ImU32 color) {
           return  {{x, y}, ImVec2(1.f, 1.f), color};
        }

        inline ImVec2 getHexCorner(ImVec2 center, float size, int nbr) {
            auto angleDeg = 60 * nbr - 30;
            auto angleRad = PI / 180 * angleDeg;
            return {center.x + size * std::cos(angleRad), center.y + size * std::sin(angleRad)};
        }

        inline ImDrawVert createHexCornerVertex(const ImDrawVert& vertex, float size, int nbr) {
            return  {getHexCorner(vertex.pos, size, nbr), vertex.uv, vertex.col};
        }

    }

	HexagonBatch::HexagonBatch() : batch_(GL_TRIANGLES, GL_DYNAMIC_DRAW) {
	}

	void HexagonBatch::init(const sdl::ImGuiShader& shader) {
		shader.useProgram();
		vao_.generate();
		vao_.bind();
		batch_.bind();
		shader.setVertexAttribPointer();
		batch_.uploadToGraphicCard();
		sdl::logger()->info("[BatchTriangles] {} Mib\n", batch_.getVboSizeInMiB());
	}

	void HexagonBatch::draw(const sdl::ImGuiShader& shader) const {
		shader.useProgram();
		vao_.bind();
		batch_.draw();
	}

	void HexagonBatch::uploadToGraphicCard() {
		batch_.uploadToGraphicCard();
	}

	void HexagonBatch::addRectangle(float x, float y, float w, float h, const sdl::Sprite& sprite, ImU32 color) {
		int textureW = sprite.getTexture().getWidth();
		int textureH = sprite.getTexture().getHeight();

		addTriangle(createVertex(x, y, sprite.getX() / textureW, sprite.getY() / textureH, color),
			createVertex(x + w, y, (sprite.getX() + sprite.getWidth()) / textureW, sprite.getY() / textureH, color),
			createVertex(x, y + h, sprite.getX() / textureW, (sprite.getY() + sprite.getHeight()) / textureH, color));

		addTriangle(createVertex(x, y + h, sprite.getX() / textureW, (sprite.getY() + sprite.getHeight()) / textureH, color),
			createVertex(x + w, y, (sprite.getX() + sprite.getWidth()) / textureW, sprite.getY() / textureH, color),
			createVertex(x + w, y + h, (sprite.getX() + sprite.getWidth()) / textureW, (sprite.getY() + sprite.getHeight()) / textureH, color));
	}

    void HexagonBatch::addHexagon(float x, float y, float size, ImU32 color) {
        auto center = createVertex(x, y, color);

        for (int i = 0; i < 6; ++i) {
            batch_.add(center);
            batch_.add(createHexCornerVertex(center, size, i));
            batch_.add(createHexCornerVertex(center, size, (i + 1) % 6));
        }
    }

	void HexagonBatch::addHexagon(float x, float y, float innerSize, float outerSize, ImU32 color) {
		auto center = createVertex(x, y, color);

		auto v1 = createHexCornerVertex(center, innerSize, 0);
		auto v2 = createHexCornerVertex(center, outerSize, 0);
		for (int i = 0; i < 6; ++i) {
			auto v3 = createHexCornerVertex(center, outerSize, (i + 1) % 6);
			auto v4 = createHexCornerVertex(center, innerSize, (i + 1) % 6);
			addTriangle(v1, v2, v4);
			addTriangle(v4, v2, v3);
			v1 = v4;
			v2 = v3;
		}
	}

	void HexagonBatch::addCircle(float x, float y, float radius, ImU32 color, int iterations) {
		auto v1 = createVertex(x, y, color);
		auto v2 = createVertex(x + radius, y, color);
		for (int i = 1; i <= iterations; ++i) {
			auto v3 = createVertex(x + radius * std::cos(2 * PI * i / iterations), y + radius * std::sin(2 * PI * i / iterations), color);
			addTriangle(v1, v2, v3);
			v2 = v3;
		}
	}

	void HexagonBatch::addAnnulus(float x, float y, float outerRadius, float innerRadius, ImU32 color, int iterations) {
		auto v1 = createVertex(x + innerRadius, y, color);
		auto v4 = createVertex(x + outerRadius, y, color);
		for (int i = 1; i <= iterations; ++i) {
			auto v2 = createVertex(x + innerRadius * std::cos(2 * PI * i / iterations), y + innerRadius * std::sin(2 * PI * i / iterations), color);
			auto v3 = createVertex(x + outerRadius * std::cos(2 * PI * i / iterations), y + outerRadius * std::sin(2 * PI * i / iterations), color);
			addTriangle(v1, v3, v4);
			addTriangle(v1, v2, v3);
			v1 = v2;
			v4 = v3;
		}
	}

	void HexagonBatch::addTriangle(const ImDrawVert& v1, const ImDrawVert& v2, const ImDrawVert& v3) {
		batch_.add(v1);
		batch_.add(v2);
		batch_.add(v3);
	}

	void HexagonBatch::addRectangle(const ImDrawVert& v1, const ImDrawVert& v2, const ImDrawVert& v3, const ImDrawVert& v4, ImU32 color) {
		addTriangle(v1, v2, v3);
		addTriangle(v3, v4, v1);
	}

	void HexagonBatch::addLine(float x1, float y1, float x2, float y2, float w, ImU32 color) {
		float angle = atan2(x2 - x1, y2 - y1);
		float dx = 0.5f * w * std::cos(angle);
		float dy = 0.5f * w * std::sin(angle);
		addRectangle(createVertex(x1 - dx, y1 + dy, color), createVertex(x1 + dx, y1 - dy, color), createVertex(x2 + dx, y2 - dy, color), createVertex(x2 - dx, y2 + dy, color));
	}

} // Namespace vin.
