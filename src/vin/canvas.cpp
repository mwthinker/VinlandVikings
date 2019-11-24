#include "canvas.h"
#include "hex/hexagon.h"
#include "logger.h"
#include "imguiextra.h"
#include "hexdimension.h"

namespace vin {

	namespace {

		float getRotationAngle(int rotations) {
			return std::fmod(rotations * PI / 3.f + PI / 2.f, 2.f * PI);
		}

		void rotate(hex::HexSides& hexSides, int rotations) {
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
		
		Vec2 viewPortCoordToClipSpace(const ViewPort& viewPort, Vec2 pos) {
			return {(pos.x / viewPort.size.x - 0.5f) * 2.f, (pos.y / viewPort.size.y - 0.5f) * 2.f};
		}

		Vec2 sdlCoordToViewPortCoord(const ViewPort& viewPort, Vec2 pos) {
			auto size = ImGui::GetIO().DisplaySize;
			float x = viewPort.pos.x + pos.x;
			float y = size.y - viewPort.pos.y - pos.y - 1;
			return {x, y};
		}

		Vec2 deviceCoordToClipSpace(const ViewPort& viewPort, Vec2 pos) {
			return viewPortCoordToClipSpace(viewPort, sdlCoordToViewPortCoord(viewPort, pos));
		}

		void glViewport(const ViewPort& viewPort) {
			::glViewport((GLint) viewPort.pos.x, (GLint) viewPort.pos.y, (GLsizei) viewPort.size.x, (GLsizei) viewPort.size.y);
		}

		Mat4 ortho(const ViewPort& viewPort, float zoom) {
			const auto& x = viewPort.pos.x;
			const auto& y = viewPort.pos.y;
			const auto& w = viewPort.size.x;
			const auto& h = viewPort.size.y;
			auto projection = glm::ortho(-1.f, 1.f, -1.f * h / w, 1.f * h / w, -100.f, 100.f);
			return glm::scale(projection, Vec3{zoom, zoom, 1});
		}

		constexpr HexDimension HEX_DIMENSION;

	}

	void HexagonImage(const vin::SpriteView& image, ImVec2 pos, ImVec2 size, float angle) {
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImGui::Image(image, ImVec2(pos.x, pos.y), size, angle, WHITE);
	}

	void addHexagon(ImDrawList* drawList, ImVec2 center, float innerSize, float outerSize, ImU32 color) {
		auto v1 = hex::getHexCorner(center, innerSize, 0);
		auto v2 = hex::getHexCorner(center, outerSize, 0);
		for (int i = 0; i < 6; ++i) {
			auto v3 = hex::getHexCorner(center, outerSize, (i + 1) % 6);
			auto v4 = hex::getHexCorner(center, innerSize, (i + 1) % 6);
			drawList->AddTriangleFilled(v1, v2, v4, color);
			drawList->AddTriangleFilled(v4, v2, v3, color);
			v1 = v4;
			v2 = v3;
		}
	}

	Canvas::Canvas() : 
		hexToWorldModel_{hex::createHexToCoordModel(HEX_DIMENSION.angle, HEX_DIMENSION.outerSize)},
		tilesGraphic_{HEX_DIMENSION, hexToWorldModel_} {

		//auto hexes = createFlatHexShape(10);
		auto hexes = hex::createRectangleShape(10, 10);
		//auto hexes = createParallelogramShape(10, 5);
		hexTileMap_ = hex::HexTileMap(hexes.begin(), hexes.end());
		for (const auto& [hex, tile] : hexTileMap_) {
			tilesGraphic_.fillGrid(hex, RED);
		}
	}

	void Canvas::drawHexImage(const sdl::ImGuiShader& imGuiShader, hex::Hexi hex, const HexImage& image) {
		hexagonBatch_.clear();
		image.getImage().bind();
		imGuiShader.setTextureId(1);
		glActiveTexture(GL_TEXTURE1);
		//hexagonBatch_.addRectangle(0, 0, 1, 1, image.getImage(), WHITE);
		//hexagonBatch_.addHexagonImage(0, 0, 1, image.getImage(), WHITE.toImU32());

		hexagonBatch_.uploadToGraphicCard();
		hexagonBatch_.draw(imGuiShader);
	}

	void Canvas::updateCanvasSize() {
		auto pos = ImGui::GetWindowPos();
		auto size = ImGui::GetWindowSize();
		const auto& s = ImGui::GetIO().DisplaySize;
		viewPort_ = {{pos.x, s.y - size.y - pos.y}, {size.x, size.y}};

		glViewport(viewPort_);
		projection_ = ortho(viewPort_, zoom_);

		hasFocus_ = ImGui::IsWindowFocused();
	}

	void Canvas::drawImgui() {
		ImGui::BeginChild("Canvas");
		updateCanvasSize();
		ImGui::EndChild();
	}

	hex::Hexi Canvas::worldToHex(Vec2 pos) const {
		auto hex = glm::inverse<>(hexToWorldModel_) * pos;
		return hex::hexRound({hex.x, hex.y});
	}

	Vec2 Canvas::hexToWorld(hex::Hexi pos) const {
		return hexToWorldModel_ * Vec2{pos.q(), pos.r()};
	}

	hex::Hexi Canvas::getHexFromMouse() const {
		ImVec2 pos = ImGui::GetMousePos();
		auto result = deviceCoordToClipSpace(viewPort_, sdlMousePos);
		Vec4 result2 = glm::inverse<>(projection_ * camera_.getView()) * Vec4 { result, 0.1f, 1.f };
		return worldToHex({result2.x, result2.y});
	}

	hex::Hexi Canvas::getHexFromMouse(Uint32 windowsId, int x, int y) const {
		auto result = deviceCoordToClipSpace(viewPort_, {x, y});
		logger()->info("(deviceCoordToClipSpace): ({})", result);
		Vec4 result2 = glm::inverse<>(projection_ * camera_.getView()) * Vec4{result, 0.1f, 1.f};
		logger()->info("(worldSpace): ({})", result2);
		logger()->info("(clipSpace): ({})", projection_ * camera_.getView() * result2);
				
		logger()->info("(worldSpace11): ({})", Vec2{1, 1});
		auto r = projection_ * camera_.getView() * Vec4{1, 1, 0, 1};
		logger()->info("(clipSpace11): ({})", r);
		//logger()->info("(worldSpace11): ({})", glm::inverse<>(projection_ * camera_.getView()) * Vec4{r.x, r.y, r.z, 1});
		
		
		logger()->info("(clipSpace11): ({})", Vec4{r.x, r.y, 0, 1} + glm::inverse<>(projection_ * camera_.getView()) * glm::inverse<>(projection_) * glm::inverse<>(camera_.getView()) * Vec4{camera_.getEye(),0});
		//logger()->info("(eye worldSpace11): ({})", glm::inverse<>(projection_)* Vec4 { camera_.getEye(), 0 });
		//logger()->info("(camera worldSpace11): ({})", glm::inverse<>(projection_)* Vec4 { camera_.getEye(), 0 });


		return worldToHex({result2.x, result2.y});
	}

	Vec2 Canvas::screenDeltaPosToWorld(Vec2 pos) {
		Vec4 rel = Vec4{pos.x / -4.f / 100, pos.y / 4.f / 100, 0.f, 1.f};
		return glm::inverse<>(projection_) * rel;
	}
	
	void Canvas::addMouseHex() {
		hex::Hexi hex = getHexFromMouse();
		//logger()->info("(q, r, s): {}", hex);

		hex::HexSides sides = hexImage_.getHexSides();
		rotate(sides, rotations_);
		//logger()->info("Rotations: {}", rotations_);
		hex::Tile hexTile{hex, sides};
		/*
		if (lastHexTile_ != hexTile) {
			lastAllowed_ = hexTileMap_.isAllowed(hexTile);
			lastHexTile_ = hexTile;
		}
		*/
		//logger()->info("Allowed {}", hexTileMap_.isAllowed(hexTile) ? "True" : "False");
		//logger()->info("Allowed {},{},{},{},{},{}",
			//hexTile.getHexSides()[0], hexTile.getHexSides()[1], hexTile.getHexSides()[2],
			//hexTile.getHexSides()[3], hexTile.getHexSides()[4], hexTile.getHexSides()[5]);
		auto pos = hexToWorld(hex);
		//graphic_.addHexagonImage(pos, HEX_DIMENSION.outerSize, hexImage_.getImage(), rotations_ * PI / 3 + HEX_DIMENSION.angle);
	}

	void Canvas::drawCanvas(double deltaTime) {
		auto model = Mat4(1);
		//graphic_.clearDraw();
		//graphic_.pushMatrix(projection_ * camera_.getView() * model);

		tilesGraphic_.setMatrix(projection_ * camera_.getView() * model);
		addMouseHex();

		//graphic_.addRectangle({-0.3f, -0.3f}, {0.3f, 0.3f}, GREEN);

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
		//graphic_.draw();
		tilesGraphic_.draw(shader_);
	}

	void Canvas::init(const sdl::ImGuiShader& imGuiShader) {
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
							case SDLK_g:
								tilesGraphic_.setGrid(!tilesGraphic_.isGrid());
								break;
							case SDLK_x:
								tilesGraphic_.setXYCoord(!tilesGraphic_.isXYCoord());
								break;
							case SDLK_h:
								tilesGraphic_.setHexCoord(!tilesGraphic_.isHexCoord());
								break;
                        }
						logger()->info("Inv: {}", glm::inverse<>(projection_ * camera_.getView()));
						logger()->info("determinant: {}", glm::determinant(glm::inverse<>(projection_ * camera_.getView())));
                    }
			        break;
				case SDL_MOUSEMOTION:
					sdlMousePos = {windowEvent.motion.x, windowEvent.motion.y};
					if (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
						auto result = screenDeltaPosToWorld({windowEvent.motion.xrel, windowEvent.motion.yrel * viewPort_.size.x / viewPort_.size.y});
						camera_.move(result);
					}
					break;
				case SDL_MOUSEBUTTONUP:
					switch (windowEvent.button.button) {
						case SDL_BUTTON_LEFT:
						{
							if (activateHexagon_) {
								const auto& button = windowEvent.button;
								hex::Hexi hex = getHexFromMouse(button.windowID, button.x, button.y);
								logger()->info("(q, r, s): {}", hex);
								hex::HexSides sides = hexImage_.getHexSides();
								rotate(sides, rotations_);
								hex::Tile hexTile{hex, sides};
								logger()->info("Allowed {}", hexTileMap_.isAllowed(hexTile)? "True" : "False");
								if (hexTileMap_.put(hexTile)) {
									hexImages_[hex] = HexImage{hexImage_.getFilename(), hexImage_.getImage(), sides, hexImage_.isFlat(), rotations_};
									tilesGraphic_.fillTile(hex, hexImages_[hex]);
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
