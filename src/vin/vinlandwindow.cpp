#include "vinlandwindow.h"

namespace vin {

    namespace {

        const ImGuiWindowFlags ImGuiNoWindow = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove;

        const ImGuiWindowFlags ImGuiNoWindow2 = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

		class Hex {
		public:
			const int q_, r_;

			constexpr Hex(int q, int r, int s) : q_(q), r_(r) {
				//assert(q + r + s == 0);
			}

			constexpr int q() const { return q_; }
			constexpr int r() const { return r_; }
			constexpr int s() const { return -q_ - r_; }
		};

		constexpr std::array<Hex, 6> CUBE_DIRECTIONS = {
			Hex(1, -1, 0),
			Hex(1, 0, -1),
			Hex(0, 1, -1),
			Hex(-1, 1, 0),
			Hex(-1, 0, 1),
			Hex(0, -1, 1)
		};

		constexpr Hex cubeDirection(int direction) {
			return CUBE_DIRECTIONS[direction];
		}

		constexpr int abs(int value) {
			return value > 0 ? value : -value;
		}

		//constexpr CubeCoord cubeNeighbor(CubeCoord cube, CubeCoord direction) {
			//return cube_add(cube, cube_direction(direction))
		//}

		constexpr int cubeDistance(Hex a, Hex b) {
			return (abs(a.q() - b.q()) + abs(a.r() - b.r()) + abs(a.s() - b.s())) / 2;
		}
				
		constexpr float SQRT_3 = 1.7320508075688772935274463f;

		class Orientation {
		public:
			const float f0, f1, f2, f3;
			const float b0, b1, b2, b3;
			
			const float startAngle; // in multiples of 60°
			constexpr Orientation(float f0_, float f1_, float f2_, float f3_,
				float b0_, float b1_, float b2_, float b3_,
				float start_angle_)
				: f0(f0_), f1(f1_), f2(f2_), f3(f3_),
				b0(b0_), b1(b1_), b2(b2_), b3(b3_),
				startAngle(start_angle_) {}
		};

		constexpr Orientation layoutPointy
			= Orientation(SQRT_3, SQRT_3 / 2.f, 0.f, 3.f / 2.f,
			SQRT_3 / 3.f, -1.f / 3.f, 0.f, 2.f / 3.f,
			0.5f);

		constexpr Orientation layoutFlat
			= Orientation(3.f / 2.f, 0.f, SQRT_3 / 2.f, SQRT_3,
			2.f / 3.f, 0.f, -1.f / 3.f, SQRT_3 / 3.f,
			0.f);

		struct Layout {
			const Orientation orientation;
			const Vec2 size;
			const Vec2 origin;
			constexpr Layout(const Orientation& orientation_, Vec2 size_, Vec2 origin_)
				: orientation(orientation_), size(size_), origin(origin_) {}
		};

		constexpr Vec2 hex_to_pixel(const Layout& layout, Hex h) {
			const Orientation& M = layout.orientation;
			float x = (M.f0 * h.q() + M.f1 * h.r()) * layout.size.x;
			float y = (M.f2 * h.q() + M.f3 * h.r()) * layout.size.y;
			return {x + layout.origin.x, y + layout.origin.y};
		}

    }

	VinlandWindow::VinlandWindow() : page_(Page::START), whiteSquare_(1, 1, 255, 255, 255, 255) {

	}

	VinlandWindow::~VinlandWindow() {

	}

	void VinlandWindow::eventUpdate(const SDL_Event& windowEvent) {
		sdl::ImGuiWindow::eventUpdate(windowEvent);

		switch (windowEvent.type) {
			case SDL_WINDOWEVENT:
				switch (windowEvent.window.event) {
					case SDL_WINDOWEVENT_LEAVE:
						break;
					case SDL_WINDOWEVENT_CLOSE:
						sdl::Window::quit();
				}
				break;
			case SDL_QUIT:
				sdl::Window::quit();
				break;
		}
	}

	void VinlandWindow::update(double deltaTime) {
        sdl::ImGuiWindow::update(deltaTime);
        const auto& shader = getImGuiShader();
        shader.setMatrix(glm::mat4x4(1));
        shader.setTextureId(1);
//		glActiveTexture(GL_TEXTURE1);
//		whiteSquare_.bindTexture();
//        hexagonBatch_.draw(getImGuiShader());
	}

    void VinlandWindow::updateImGui(double deltaTime) {

    }

	void VinlandWindow::initOpenGl() {
        sdl::ImGuiWindow::initOpenGl();
	}

	void VinlandWindow::initPreLoop() {
        sdl::ImGuiWindow::initPreLoop();
        //glViewport(0,0, 400, 400);

		/*
		const auto& shader = getImGuiShader();
		hexagonBatch_.addHexagon(0.1f, 0.1f, 0.5f);

		const ImU32 RED = Color(1.0f, 0.f, 0.f);
		hexagonBatch_.addHexagon(0.1f, 0.1f, 0.6f, 0.8f, RED);
		constexpr float innerRadius = 0.19f;
		constexpr float outerRadius = 0.2f;

		constexpr Layout layout(layoutPointy, {outerRadius, outerRadius}, {0.f, 0.f});

		for (int i = -5; i < 5; ++i) {
			for (int j = -5; j < 5; ++j) {
				auto pos = hex_to_pixel(layout, Hex(i, j, 0));
				hexagonBatch_.addHexagon(pos.x, pos.y, innerRadius, outerRadius, RED);
			}
		}

        hexagonBatch_.init(shader);
		glActiveTexture(GL_TEXTURE1);
		whiteSquare_.bindTexture();
		*/
	}

} // Namespace vin.
