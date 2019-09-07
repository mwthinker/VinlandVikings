#include "canvas.h"
#include "hexagon.h"
#include "logger.h"
#include "imguiextra.h"

namespace vin {

	namespace {

		float getRotationAngle(int rotations) {
			return std::fmod(rotations * PI / 3.f + PI / 2.f, 2.f * PI);
		}

		void rotate(HexSides& hexSides, int rotations) {
			std::rotate(hexSides.begin(), hexSides.begin() + rotations % 6, hexSides.end());
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
		//auto hexes = createFlatHexShape(10);
		auto hexes = createRectangleShape(10, 10);
		//auto hexes = createParallelogramShape(10, 5);
		hexTileMap_ = HexTileMap(hexes.begin(), hexes.end());
	}

	void Canvas::drawHexImage(const sdl::ImGuiShader& imGuiShader, Hexi hex, const HexImage& image) {
		hexagonBatch_.clear();
		image.getImage().bindTexture();
		imGuiShader.setTextureId(1);
		glActiveTexture(GL_TEXTURE1);
		//hexagonBatch_.addRectangle(0, 0, 1, 1, image.getImage(), WHITE);
		hexagonBatch_.addHexagonImage(0, 0, 1, image.getImage(), WHITE);

		hexagonBatch_.uploadToGraphicCard();
		hexagonBatch_.draw(imGuiShader);
	}

	void Canvas::updateCanvasSize() {
		auto pos = ImGui::GetWindowPos();
		windowSize_ = {pos.x, pos.y};
		auto size = ImGui::GetWindowSize();
		windowSize_ = {size.x, size.y};

		const auto& x = windowPos_.x;
		const auto& y = windowPos_.y;
		const auto& w = windowSize_.x;
		const auto& h = windowSize_.y;

		glViewport((GLint)x, (GLint)y, (GLsizei)w, (GLsizei)h);
		projection_ = glm::ortho(-1.f, 1.f, -1.f * h / w, 1.f * h / w, -100.f, 100.f);		
		projection_ = glm::scale(projection_, Vec3{zoom_, zoom_, 1});		

		camera_.setPosition({x_, y_});
		camera_.setAngle(angle_);
	}

	void Canvas::drawImgui() {
		ImGui::BeginChild("Canvas");
		updateCanvasSize();
		//for (int n = 0; n < 50; n++)
			//ImGui::Text("%04d: Some text", n);


		hasFocus_ = ImGui::IsWindowFocused();

		//drawGrid(hexTileMap_, zoom_, x_, y_);

		//drawGrid(hexImages_, zoom_, x_, y_);

		//ImVec2 pos = ImGui::GetMousePos();
		//ImVec2 size(createInnerRadius(zoom_) * 2, createInnerRadius(zoom_) * 2);
		//HexagonImage(hexImage_.getImage(), pos, size, getRotationAngle(rotations_));
		
		//logger()->info("Pixel: ({}, {})", x_, y_);
		//logger()->info("Hex: ({}, {}, {})", hexi.q(), hexi.r(), hexi.s());
		Hexi hexi = getHexFromMouse();
		//auto pixel = hexToPixel(createFlatLayout(x_, y_, zoom_), hexi);

		auto pixel = createHexToCoordModel() * Vec2(x_, y_);// glm::rotate(Vec2(x_, y_), 0.f)
        HexSides sides = hexImage_.getHexSides();
		rotate(sides, rotations_);
        HexTile hexTile(hexi, sides);
        if (lastHexTile_ != hexTile) {
            lastAllowed_ = hexTileMap_.isAllowed(hexTile);
            lastHexTile_ = hexTile;
        }

		if (lastAllowed_) {
			//addHexagon(ImGui::GetWindowDrawList(), ImVec2(pixel.x, pixel.y), 0, createInnerRadius(zoom_), Color(0.0f, 7.f, 0.f, 0.5f));
		} else {
			//addHexagon(ImGui::GetWindowDrawList(), ImVec2(pixel.x, pixel.y), 0, createInnerRadius(zoom_), Color(0.7f, 0.f, 0.f, 0.5f));
		}

		ImGui::EndChild();
	}

	Hexi Canvas::getHexFromMouse() const {
		ImVec2 pos = ImGui::GetMousePos();
		Hexf hexf{0,0}; // = pixelToHex(createFlatLayout(x_, y_, zoom_), Vec2(pos.x, pos.y));
		return hexRound(hexf);
	}

	void Canvas::addGrid() {
		constexpr float innerRadius = 0.8f;
		constexpr float outerRadius = 1.f;

		//constexpr Layout layout(layoutPointy, {outerRadius, outerRadius}, {0.f, 0.f});
		for (const auto& [hex, hexTile] : hexTileMap_) {
			auto pos = createHexToCoordModel(PI/2) * Vec2{hex.q(), hex.r()};
			if (hex == Hexi{0,0}) {
				graphic_.addPointyHexagon(pos, innerRadius, outerRadius, BLUE);
			} else {
				graphic_.addPointyHexagon(pos, innerRadius, outerRadius, RED);
			}
		}

	}

	Vec2 Canvas::screenPosToWorld(Vec2 pos) {
		Vec4 rel = {pos.x / -4.f, pos.y / 4.f, 0.f, 1.f};
		return glm::inverse<>(projection_) * rel;
	}
	
	void Canvas::addGridImages() {
		//constexpr float innerRadius = 0.19f;
		//constexpr float outerRadius = 0.2f;

		//constexpr Layout layout(layoutPointy, {outerRadius, outerRadius}, {0.f, 0.f});
		for (const auto& [hex, hexTile] : hexImages_) {
			//auto pos = hexToPixel(layout, hex);
			auto pos = createHexToCoordModel(0.f) * Vec2{hex.q(), hex.r()};
			//graphic_.addPointyHexagon(pos, 0.8f, 1.f, RED);
		}
	}

	void Canvas::drawCanvas(double deltaTime) {
		auto model = Mat4(1);
		graphic_.clearDraw();
		graphic_.setMatrix(projection_ * camera_.getView() * model);

		addGrid();
		addGridImages();
		
		/*
		graphic_.addRectangle({0.f, 0.f}, {0.3f, 0.3f}, WHITE);
		graphic_.addRectangle({-0.3f, -0.3f}, {0.3f, 0.3f}, BLUE);
		graphic_.addRectangle({-0.8f, 0.6f}, {0.1f, 0.1f}, RED);
		graphic_.addCircle({0.1f,0.1f}, 0.1f, RED);
		graphic_.addFlatHexagon({-0.3f,0.3f}, 0.1f, BLUE);
		graphic_.addPointyHexagon({-0.4f,-0.3f}, 0.05f, BLUE);
		graphic_.addFlatHexagonImage({0.4f, 0.4f}, 0.5f, hexImage_.getImage());
		graphic_.addPointyHexagonImage({-0.4f, 0.4f}, 0.5f, hexImage_.getImage());		
		graphic_.addPointyHexagon({-0.4f,-0.3f}, 0.05f, BLUE);
		graphic_.addFlatHexagon({0.2f, 0.2f}, 0.2f, 0.3f, RED);
		graphic_.addPointyHexagon({-0.2f, -0.2f}, 0.2f, 0.3f, RED);
		*/

		graphic_.draw();
	}

	void Canvas::init(const sdl::ImGuiShader& imGuiShader) {
		addGrid();
		addGridImages();
	}

	void Canvas::eventUpdate(const SDL_Event& windowEvent) {
		if (hasFocus_ || true) {
			switch (windowEvent.type) {
				case SDL_MOUSEWHEEL:
					if (windowEvent.wheel.y > 0) { // scroll up
						zoom_ *= 1.1f;
						// Put code for handling "scroll up" here!
						//logger()->info("windowEvent.wheel.y: {}", windowEvent.wheel.y);
					} else if (windowEvent.wheel.y < 0) { // scroll down
						zoom_ *= 1 / 1.1f;
						// Put code for handling "scroll down" here!
						//logger()->info("windowEvent.wheel.y: {}", windowEvent.wheel.y);
					}
					if (windowEvent.wheel.x > 0) { // scroll right
						// ...
					} else if (windowEvent.wheel.x < 0) { // scroll left
						// ...
					}
					break;
			    case SDL_KEYDOWN:
			        if (hasFocus_ || true) {
			            constexpr float STEP = 10.f;
                        switch (windowEvent.key.keysym.sym) {
                            case SDLK_LEFT:
                                x_ -= STEP;
                                break;
                            case SDLK_RIGHT:
                                x_ += STEP;
                                break;
                            case SDLK_UP:
                                y_ += STEP;
                                break;
                            case SDLK_DOWN:
                                y_ -= STEP;
                                break;
							case SDLK_PAGEUP:
								angle_ -= 0.1f;
								break;
							case SDLK_PAGEDOWN:
								angle_ += 0.1f;
								break;
                            case SDLK_c:
                                hexImages_.clear();
                                hexTileMap_.clear();
                                break;
                        }
                    }
			        break;
				case SDL_MOUSEMOTION:
					if (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
						const auto& w = windowSize_.x;
						const auto& h = windowSize_.y;

						auto result = screenPosToWorld({windowEvent.motion.xrel, windowEvent.motion.yrel * w / h});

						x_ += result.x;
						y_ += result.y;
						logger()->info("(x, y): ({}, {})", x_, y_);
					}
					break;
				case SDL_MOUSEBUTTONUP:
					switch (windowEvent.button.button) {
						case SDL_BUTTON_LEFT:
						{
							const auto& w = windowSize_.x;
							const auto& h = windowSize_.y;
							auto result = screenPosToWorld({windowEvent.button.x, windowEvent.button.y * w / h});
							logger()->info("(x, y): ({}, {})", result.x, result.y);
							if (activateHexagon_) {
								//logger()->info("Hex: ({}, {}, {})", hexi.q(), hexi.r(), hexi.s());
								Hexi hex = getHexFromMouse();
								HexSides sides = hexImage_.getHexSides();
								rotate(sides, rotations_);
								HexTile hexTile(hex, sides);
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
