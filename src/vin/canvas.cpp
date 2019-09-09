#include "canvas.h"
#include "hexagon.h"
#include "logger.h"
#include "imguiextra.h"

namespace vin {

	struct ViewPort {
		Vec2 pos;
		Vec2 size;
	};

	namespace {

		float getRotationAngle(int rotations) {
			return std::fmod(rotations * PI / 3.f + PI / 2.f, 2.f * PI);
		}

		void rotate(HexSides& hexSides, int rotations) {
			std::rotate(hexSides.begin(), hexSides.begin() + rotations % 6, hexSides.end());
		}

		constexpr float normalizedDeviceCoordsToWindowCoords(float pos, float size, float normalizedPos) {
			return (pos + 1.f) * size / 2.f + normalizedPos;;
		}

		Vec2 normalizedDeviceCoordsToWindowCoords(ViewPort viewPort, Vec2 normalizedDeviceCoord) {
			float x = normalizedDeviceCoordsToWindowCoords(viewPort.pos.x, viewPort.size.x, normalizedDeviceCoord.x);
			float y = normalizedDeviceCoordsToWindowCoords(viewPort.pos.y, viewPort.size.y, normalizedDeviceCoord.y);
			return {x, y};
		}

		Vec2 sdlCoordToViewPortCoord(Uint32 windowsId, const ViewPort& viewPort, Vec2 pos) {
			int w, h;
			SDL_GetWindowSize(SDL_GetWindowFromID(windowsId), &w, &h);

			float x = viewPort.pos.x + pos.x;
			float y = h + viewPort.pos.y - pos.y - 1;
			return {x, y};
		}
		
		Vec2 viewPortCoordToClipSpace(const ViewPort & viewPort, Vec2 pos) {
			return {(pos.x / viewPort.size.x - 0.5f) * 2.f, (pos.y / viewPort.size.y - 0.5f) * 2.f};
		}

		constexpr float HEX_INNER_SIZE = 0.8f;
		constexpr float HEX_OUTER_SIZE = 1.f;
		constexpr float HEX_ANGLE = PI / 2;

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
		hexToWorldModel_ = createHexToCoordModel(HEX_ANGLE, HEX_OUTER_SIZE);
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

		glViewport((GLint)x , (GLint) y, (GLsizei) w, (GLsizei) h);
		projection_ = glm::ortho(-1.f, 1.f, -1.f * h / w, 1.f * h / w, -100.f, 100.f);		
		projection_ = glm::scale(projection_, Vec3{zoom_, zoom_, 1});

		hasFocus_ = ImGui::IsWindowFocused();
	}

	void Canvas::drawImgui() {
		ImGui::BeginChild("Canvas");
		updateCanvasSize();
		//for (int n = 0; n < 50; n++)
			//ImGui::Text("%04d: Some text", n);

		//drawGrid(hexTileMap_, zoom_, x_, y_);

		//drawGrid(hexImages_, zoom_, x_, y_);

		//ImVec2 pos = ImGui::GetMousePos();
		//ImVec2 size(createInnerRadius(zoom_) * 2, createInnerRadius(zoom_) * 2);
		//HexagonImage(hexImage_.getImage(), pos, size, getRotationAngle(rotations_));
		
		//logger()->info("Pixel: ({}, {})", x_, y_);
		//logger()->info("Hex: ({}, {}, {})", hexi.q(), hexi.r(), hexi.s());
		Hexi hexi = getHexFromMouse();
		//auto pixel = hexToPixel(createFlatLayout(x_, y_, zoom_), hexi);

		auto pixel = createHexToCoordModel() * camera_.getLookAtPosition();// glm::rotate(Vec2(x_, y_), 0.f)
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

	Hexi Canvas::worldToHex(Vec2 pos) const {
		auto hex = glm::inverse<>(hexToWorldModel_) * pos;
		return hexRound({hex.x, hex.y});
	}

	Vec2 Canvas::hexToWorld(Hexi pos) const {
		return hexToWorldModel_ * Vec2{pos.q(), pos.r()};
	}

	Hexi Canvas::getHexFromMouse() const {
		ImVec2 pos = ImGui::GetMousePos();
		Hexf hexf{0,0}; // = pixelToHex(createFlatLayout(x_, y_, zoom_), Vec2(pos.x, pos.y));
		return hexRound(hexf);
	}

	Hexi Canvas::getHexFromMouse(Uint32 windowsId, int x, int y) const {
		ViewPort viewPort = {
			windowPos_,
			windowSize_
		};
		auto result = sdlCoordToViewPortCoord(windowsId, viewPort, {x, y});
		result = viewPortCoordToClipSpace(viewPort, result);
		result = glm::inverse<>(projection_ * camera_.getView()) * Vec4 { result, 0, 1.f };
		return worldToHex({result.x, result.y});
	}

	void Canvas::addGrid() {
		//constexpr Layout layout(layoutPointy, {outerRadius, outerRadius}, {0.f, 0.f});
		for (const auto& [hex, hexTile] : hexTileMap_) {
			auto pos = hexToWorld(hex);
			if (hex == Hexi{0,0}) {
				graphic_.addPointyHexagon(pos, HEX_INNER_SIZE, HEX_OUTER_SIZE, BLUE);
			} else {
				graphic_.addPointyHexagon(pos, HEX_INNER_SIZE, HEX_OUTER_SIZE, RED);
			}
		}

	}

	Vec2 Canvas::screenPosToWorld(Vec2 pos) {
		Vec4 rel = {pos.x / -4.f, pos.y / 4.f, 0.f, 1.f};
		return glm::inverse<>(projection_) * rel;
	}
	
	void Canvas::addGridImages() {
		for (const auto& [hex, hexTile] : hexImages_) {
			auto pos = hexToWorld(hex);
			graphic_.addPointyHexagonImage(pos, 1.f, hexTile.getImage());
		}
	}

	void Canvas::drawCanvas(double deltaTime) {
		auto model = Mat4(1);
		graphic_.clearDraw();
		graphic_.pushMatrix(projection_ * camera_.getView() * model);

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
		glActiveTexture(GL_TEXTURE1);
		graphic_.draw();
	}

	void Canvas::init(const sdl::ImGuiShader& imGuiShader) {
		addGrid();
		addGridImages();
	}

	void Canvas::eventUpdate(const SDL_Event& windowEvent) {
		if (hasFocus_) {
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
								camera_.move({-STEP,0});
                                break;
                            case SDLK_RIGHT:
								camera_.move({STEP,0});
                                break;
                            case SDLK_UP:
								camera_.move({0,STEP});
                                break;
                            case SDLK_DOWN:
								camera_.move({0,-STEP});
                                break;
							case SDLK_PAGEUP:
								camera_.angleDelta(-0.1f);
								break;
							case SDLK_PAGEDOWN:
								camera_.angleDelta(0.1f);
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
						camera_.move(result);
						//x_ += result.x;
						//y_ += result.y;
						//logger()->info("(x, y): ({}, {})", x_, y_);
					}
					break;
				case SDL_MOUSEBUTTONUP:
					switch (windowEvent.button.button) {
						case SDL_BUTTON_LEFT:
						{							
							if (activateHexagon_) {
								const auto& button = windowEvent.button;
								Hexi hex = getHexFromMouse(button.windowID, button.x, button.y);
								logger()->info("(q, r, s): ({}, {}, {})", hex.q(), hex.r(), hex.s());
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
