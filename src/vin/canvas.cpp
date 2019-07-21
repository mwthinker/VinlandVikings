#include "canvas.h"
#include "hexagon.h"
#include "logger.h"
#include "imguiextra.h"

namespace vin {

	namespace {

		const ImGuiWindowFlags ImGuiNoWindow = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove;

		const ImGuiWindowFlags ImGuiNoWindow2 = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		
		const float INNER_RADIUS = 11.f;
		const float OUTER_RADIUS = 12.f;

		float createInnerRadius(float zoom = 1) {
			return INNER_RADIUS * zoom;
		}

		float createOuterRadius(float zoom = 1) {
			return OUTER_RADIUS * zoom;
		}

		Layout createFlatLayout(float x, float y, float zoom) {
			return Layout(layoutPointy, {createInnerRadius(zoom), createOuterRadius(zoom)}, {300.f + x, 300.f + y});
		}

		float getRotationAngle(int rotations) {
			return std::fmod(rotations * PI / 3.f + PI / 2.f, 2.f * PI);
		}

		HexSides shiftHexSides(const HexSides& hexSides, int rotations) {
			HexSides shiftSides;
			for (int i = 0; i < 6; ++i) {
				shiftSides[(i + rotations) % 6] = hexSides[i];
			}
			return shiftSides;
		}

	}
	
	void drawGrid(const HexTileMap& hexes, float zoom, float x, float y) {
		ImVec2 p = ImGui::GetCursorScreenPos();

		ImVec2 size = ImGui::GetWindowSize();
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		const ImU32 RED = Color(1.0f, 0.f, 0.f);
		const ImU32 BLUE = Color(0.0f, 0.f, 1.f);
		const ImU32 GREEN = Color(0.0f, 1.f, 0.f);

		const ImU32 GRID_COLOR = Color(0.5f, 0.5f, 0.5f);

		auto layout = createFlatLayout(x, y, zoom);
		float innerRadius = createInnerRadius(zoom);
		float outerRadius = createOuterRadius(zoom);

		for (const auto& [hex, hexTile] : hexes) {
			auto pos = hexToPixel(layout, hex);
			auto color = GRID_COLOR;
			
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

	void drawGrid(const std::unordered_map<Hexi, HexImage>& hexes, float zoom, float x, float y) {
		ImVec2 p = ImGui::GetCursorScreenPos();

		ImVec2 size = ImGui::GetWindowSize();
		auto layout = createFlatLayout(x, y, zoom);
		float innerRadius = createInnerRadius(zoom);

		for (const auto& [hex, hexImage] : hexes) {
			auto pos = hexToPixel(layout, hex);
			HexagonImage(hexImage.getImage(), {pos.x, pos.y}, {innerRadius * 2.f, innerRadius * 2.f}, getRotationAngle(hexImage.getRotations()));
		}

	}

	void HexagonImage(const sdl::Sprite& image, ImVec2 pos, ImVec2 size, float angle) {
		if (!image.getTexture().isValid()) {
			return;
		}

		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImGui::Image(image, ImVec2(pos.x, pos.y), size, angle, WHITE);
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
		rotations_ = 0;

		auto hexes = createFlatHexShape(10);
		//auto hexes = createParallelogramShape(10, 5);
		hexTileMap_ = HexTileMap(hexes.begin(), hexes.end());
	}

	void Canvas::draw() {
		ImGui::BeginChild("Canvas");
		//for (int n = 0; n < 50; n++)
			//ImGui::Text("%04d: Some text", n);

		hasFocus_ = ImGui::IsWindowFocused();

		drawGrid(hexTileMap_, zoom_, x_, y_);

		drawGrid(hexImages_, zoom_, x_, y_);

		ImVec2 pos = ImGui::GetMousePos();
		ImVec2 size(createInnerRadius(zoom_) * 2, createInnerRadius(zoom_) * 2);
		HexagonImage(hexImage_.getImage(), pos, size, getRotationAngle(rotations_));

		
		//logger()->info("Pixel: ({}, {})", x_, y_);
		//logger()->info("Hex: ({}, {}, {})", hexi.q(), hexi.r(), hexi.s());
		Hexi hexi = getHexFromMouse();
		auto pixel = hexToPixel(createFlatLayout(x_, y_, zoom_), hexi);
		if (hexTileMap_.isEmpty(hexi)) {
			addHexagon(ImGui::GetWindowDrawList(), ImVec2(pixel.x, pixel.y), 0, createInnerRadius(zoom_), Color(0.0f, 7.f, 0.f, 0.5f));
		} else {
			addHexagon(ImGui::GetWindowDrawList(), ImVec2(pixel.x, pixel.y), 0, createInnerRadius(zoom_), Color(0.7f, 0.f, 0.f, 0.5f));
		}

		ImGui::EndChild();
	}

	Hexi Canvas::getHexFromMouse() const {
		ImVec2 pos = ImGui::GetMousePos();
		Hexf hexf = pixelToHex(createFlatLayout(x_, y_, zoom_), Vec2(pos.x, pos.y));
		return hexRound(hexf);
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
				case SDL_MOUSEBUTTONUP:
					switch (windowEvent.button.button) {
						case SDL_BUTTON_LEFT:
						{
							if (activateHexagon_) {
								//logger()->info("Hex: ({}, {}, {})", hexi.q(), hexi.r(), hexi.s());
								Hexi hex = getHexFromMouse();
								HexSides sides = hexImage_.getHexSides();
								HexTile hexTile(hex, shiftHexSides(sides, rotations_));
								if (hexTileMap_.put(hexTile)) {
									hexImages_[hex] = HexImage(hexImage_.getFilename(), hexImage_.getImage(), sides, hexImage_.isFlat(), rotations_);
								}
								//hexTileMap_.isAllowed(hexTile);
							}
							break;
						}
						case SDL_BUTTON_MIDDLE:
							rotations_ = (rotations_ + 1) % 6;
							break;
					}
					break;
			}
		}
	}

} // Namespace vin.
