#ifndef VINLANDVIKINGS_VIN_HEXAGONBATCH_H
#define VINLANDVIKINGS_VIN_HEXAGONBATCH_H

#include <sdl/batch.h>
#include <sdl/imguishader.h>



#include <imgui.h>

namespace vin {

    class HexagonBatch {
    public:
        HexagonBatch() : HexagonBatch(100) {

        }

        HexagonBatch(int maxVertexes) : batch_(GL_TRIANGLES, GL_DYNAMIC_DRAW, maxVertexes) {

            sdl::logger()->info("[BatchTriangles] {} Mib\n", batch_.getVboSizeInMiB());
        }

        virtual ~HexagonBatch() = default;

        void uploadToGraphicCard() {
            batch_.uploadToGraphicCard();
        }

        void addHexagon(float x, float y, float size, ImU32 color = ImColor(255, 255, 255));

        //void addRectangle(float x, float y, float w, float h, const sdl::Sprite& sprite) {
         //   int textureW = sprite.getTexture().getWidth();
        //    int textureH = sprite.getTexture().getHeight();

            //ImGui::ImDrawVert vert;

            //addTriangle(ImGuiVert(x, y, sprite.getX() / textureW, sprite.getY() / textureH),
              //          TestShader2::Vertex(x + w, y, (sprite.getX() + sprite.getWidth()) / textureW, sprite.getY() / textureH),
                //        TestShader2::Vertex(x, y + h, sprite.getX() / textureW, (sprite.getY() + sprite.getHeight()) / textureH));

            //addTriangle(TestShader2::Vertex(x, y + h, sprite.getX() / textureW, (sprite.getY() + sprite.getHeight()) / textureH),
              //          TestShader2::Vertex(x + w, y, (sprite.getX() + sprite.getWidth()) / textureW, sprite.getY() / textureH),
              //          TestShader2::Vertex(x + w, y + h, (sprite.getX() + sprite.getWidth()) / textureW, (sprite.getY() + sprite.getHeight()) / textureH));
        //}

        //void addRectangle(const TestShader2::Vertex& v1, const TestShader2::Vertex& v2, const TestShader2::Vertex& v3, const TestShader2::Vertex& v4) {
            //addTriangle(v1, v2, v3);
            //addTriangle(v3, v4, v1);
        //}

        void addCircle(float x, float y, float radius, const int iterations = 40) {
            const float PI = 3.14159265359f;

            //const TestShader2::Vertex v1(x, y);
            //TestShader2::Vertex v2(x + radius, y);
            //for (int i = 1; i <= iterations; ++i) {
              //  TestShader2::Vertex v3(x + radius * std::cos(2 * PI * i / iterations), y + radius * std::sin(2 * PI * i / iterations));
                //addTriangle(v1, v2, v3);
               // v2 = v3;
            //}
        }

        void addLine(float x1, float y1, float x2, float y2, float w) {
            float angle = atan2(x2 - x1, y2 - y1);
            float dx = 0.5f * w * std::cos(angle);
            float dy = 0.5f * w * std::sin(angle);
            //addRectangle(TestShader2::Vertex(x1 - dx, y1 + dy), TestShader2::Vertex(x1 + dx, y1 - dy), TestShader2::Vertex(x2 + dx, y2 - dy), TestShader2::Vertex(x2 - dx, y2 + dy));
        }

        void addCircle(float x, float y, float outerRadius, float innerRadius, const int iterations = 40) {
            //const auto PI = glm::pi<GLfloat>();

            //TestShader2::Vertex v1(x + innerRadius, y);
            //TestShader2::Vertex v4(x + outerRadius, y);
            //for (int i = 1; i <= iterations; ++i) {
              //  TestShader2::Vertex v2(x + innerRadius * std::cos(2 * PI * i / iterations), y + innerRadius * std::sin(2 * PI * i / iterations));
              //  TestShader2::Vertex v3(x + outerRadius * std::cos(2 * PI * i / iterations), y + outerRadius * std::sin(2 * PI * i / iterations));
              //  addTriangle(v1, v3, v4);
              //  addTriangle(v1, v2, v3);
              //  v1 = v2;
              //  v4 = v3;
            //}
        }

        void addTriangle(const ImDrawVert& v1, const ImDrawVert& v2, const ImDrawVert& v3) {
            batch_.add(v1);
            batch_.add(v2);
            batch_.add(v3);
        }

        void init(const sdl::ImGuiShader& shader) {
            shader.useProgram();
            vao_.create();
            batch_.bindBuffer();
            shader.setVertexAttribPointer();
        }

        void draw(const sdl::ImGuiShader& shader) const {
            shader.useProgram();
            vao_.bind();
            batch_.draw();
        }

    private:
        sdl::Batch<ImDrawVert> batch_;
        sdl::VertexArrayObject vao_;
    };

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_HEXAGONBATCH_H
