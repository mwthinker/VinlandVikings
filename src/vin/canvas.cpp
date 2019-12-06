#include "canvas.h"
#include "hex/hexagon.h"
#include "logger.h"
#include "imguiextra.h"
#include "hexdimension.h"

namespace vin {

	namespace {

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

		constexpr float ZOOM_MIN = 0.02f;
		constexpr float ZOOM_MAX = 3.0f;

	}

	void HexagonImage(const vin::SpriteView& image, ImVec2 pos, ImVec2 size, float angle) {
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImGui::Image(image, ImVec2{pos.x, pos.y}, size, angle, WHITE);
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
	
	void Canvas::eventUpdate(const SDL_Event& windowEvent) {

	}

	void Canvas::updateCanvasSize() {
		auto pos = ImGui::GetWindowPos();
		auto size = ImGui::GetWindowSize();
		const auto& s = ImGui::GetIO().DisplaySize;
		viewPort_ = {{pos.x, s.y - size.y - pos.y}, {size.x, size.y}};

		glViewport(viewPort_);
		//projection_ = ortho(viewPort_, zoom_);

		isHovering_ = ImGui::IsWindowFocused();

		logger()->info("ImGui::IsWindowHovered(): {}", ImGui::IsWindowHovered());
	}

	Mat4 Canvas::getProjection() const {
		return projection_;
	}

	HexWorldCanvas::HexWorldCanvas() :
		hexToWorldModel_{hex::createHexToCoordModel(HEX_DIMENSION.angle, HEX_DIMENSION.outerSize)},
		tilesGraphic_{HEX_DIMENSION, hexToWorldModel_} {

		auto hexes = hex::createHexShape(10);
		//auto hexes = hex::createRectangleShape(10, 10);
		//auto hexes = createParallelogramShape(10, 5);
		tileBoard_ = hex::TileBoard(hexes.begin(), hexes.end());
		for (const auto& [hex, tile] : tileBoard_) {
			tilesGraphic_.fillGrid(hex, RED);
		}
		tilesGraphic_.fill(sdl::Color{0.6f, 0.6f, 0.6f, 0.6f});
	}

	void HexWorldCanvas::updateCanvasSize() {
		auto pos = ImGui::GetWindowPos();
		auto size = ImGui::GetWindowSize();
		const auto& s = ImGui::GetIO().DisplaySize;
		viewPort_ = {{pos.x, s.y - size.y - pos.y}, {size.x, size.y}};

		glViewport(viewPort_);
		projection_ = ortho(viewPort_, zoom_);

		isHovering_ = ImGui::IsWindowHovered();
	}

	void HexWorldCanvas::drawImgui() {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 10.1f);
		ImGui::PushStyleColor(ImGuiCol_Border, RED.toImU32());
		ImGui::BeginChild("Canvas", ImVec2{0, 0}, true);
		
		updateCanvasSize();
		ImGui::EndChild();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
	}

	void HexWorldCanvas::addTileMapToGraphic() {
		Random random;
		/*
		for (const auto& [pos, sides] : tileBoard_) {
			auto it = hexImages_.find(sides);
			if (it != hexImages_.end()) {
				const auto& images = hexImages_[sides].hexImages_;
				auto image = images[random.generateInt(0, images.size() - 1)];
				tilesGraphic_.fillTile(pos, image);
			}
		}
		*/
	}

	hex::Hexi HexWorldCanvas::worldToHex(Vec2 pos) const {
		auto hex = glm::inverse<>(hexToWorldModel_) * pos;
		return hex::hexRound({hex.x, hex.y});
	}

	Vec2 HexWorldCanvas::hexToWorld(hex::Hexi pos) const {
		return hexToWorldModel_ * Vec2{pos.q(), pos.r()};
	}

	hex::Hexi HexWorldCanvas::getHexFromMouse() const {
		ImVec2 pos = ImGui::GetMousePos();
		auto result = deviceCoordToClipSpace(viewPort_, sdlMousePos);
		Vec4 result2 = glm::inverse<>(projection_ * camera_.getView()) * Vec4 { result, 0.1f, 1.f };
		return worldToHex({result2.x, result2.y});
	}

	hex::Hexi HexWorldCanvas::getHexFromMouse(Uint32 windowsId, int x, int y) const {
		auto result = deviceCoordToClipSpace(viewPort_, {x, y});
		Vec4 result2 = glm::inverse<>(projection_ * camera_.getView()) * Vec4{result, 0.1f, 1.f};
		return worldToHex({result2.x, result2.y});
	}

	Vec2 HexWorldCanvas::screenDeltaPosToWorld(Vec2 pos) {
		Vec4 rel{pos.x / -4.f / 100, pos.y / 4.f / 100, 0.f, 1.f};
		return glm::inverse<>(projection_) * rel;
	}
	
	void HexWorldCanvas::addMouseHexToGraphic() {
		hex::Hexi hex = getHexFromMouse();
		hex::HexSides sides = hexImage_.getHexSides();
		rotate(sides, rotations_);
		auto pos = hexToWorld(hex);
		graphic_.addHexagonImage(pos, HEX_DIMENSION.outerSize, hexImage_.getImage(), rotations_ * PI / 3 + HEX_DIMENSION.angle);
		if (!tileBoard_.isAllowed(hex, sides)) {
			graphic_.addFilledHexagon(pos, HEX_DIMENSION.outerSize, Color{1.f, 0, 0, 0.4}, rotations_ * PI / 3 + HEX_DIMENSION.angle);
		}
		logger()->warn("hex: {}", hex);
		
		graphic_.addFilledHexagon(hexToWorld(hex + hex::CUBE_DIRECTIONS[0] * 2), HEX_DIMENSION.outerSize, RED, rotations_ * PI / 3 + HEX_DIMENSION.angle);
		graphic_.addFilledHexagon(hexToWorld(hex + hex::CUBE_DIRECTIONS[1] * 2), HEX_DIMENSION.outerSize, GREEN, rotations_ * PI / 3 + HEX_DIMENSION.angle);
		graphic_.addFilledHexagon(hexToWorld(hex + hex::CUBE_DIRECTIONS[2] * 2), HEX_DIMENSION.outerSize, BLUE, rotations_ * PI / 3 + HEX_DIMENSION.angle);		

		graphic_.addFilledHexagon(hexToWorld(hex + hex::CUBE_DIRECTIONS[3] * 2), HEX_DIMENSION.outerSize, ORANGE, rotations_ * PI / 3 + HEX_DIMENSION.angle);
		graphic_.addFilledHexagon(hexToWorld(hex + hex::CUBE_DIRECTIONS[4] * 2), HEX_DIMENSION.outerSize, CYAN, rotations_ * PI / 3 + HEX_DIMENSION.angle);
		graphic_.addFilledHexagon(hexToWorld(hex + hex::CUBE_DIRECTIONS[5] * 2), HEX_DIMENSION.outerSize, WHITE, rotations_ * PI / 3 + HEX_DIMENSION.angle);
	}

	void HexWorldCanvas::drawCanvas(double deltaTime) {
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		Mat4 model{1};
		graphic_.clearDraw();
		graphic_.pushMatrix(projection_ * camera_.getView() * model);

		tilesGraphic_.setMatrix(projection_ * camera_.getView() * model);
		if (isHovering_) {
			addMouseHexToGraphic();
		}
		graphic_.pushMatrix(projection_);		
		
		//graphic_.addRectangle({-1.f, -1.f}, {2.f, 2.f}, RED); // Bottom
		tilesGraphic_.draw(shader_);
		graphic_.draw(shader_);
	}

	void HexWorldCanvas::setDefaultHexSprite(const HexImage& hexImage) {
		//tilesGraphic_.fill(hexImage);
	}

	void HexWorldCanvas::setDeck(const std::vector<HexImage>& deck) {
		deck_ = deck;
	}

	void HexWorldCanvas::setHexImagesMap(const HexImagesMap& hexTypes) {
		hexTypes_ = hexTypes;
		/*
		for (const auto& [sides, invariantHexImageType] : hexTypes_) {
			
			for (const auto& imageType : invariantHexImageType.hexImages_)				
				for (int i = 0; i < 6; ++i) {
					const hex::HexSides& sides = invariantHexImageType.hexImages_[i].getHexSides();
					auto& hexImages = hexImages_[sides];
					//hexImages.hexImages_.push_back()
				}
		}
		*/
	}

	void HexWorldCanvas::eventUpdate(const SDL_Event& windowEvent) {
		switch (windowEvent.type) {
			case SDL_MOUSEWHEEL:
				if (isHovering_) {
					if (windowEvent.wheel.y > 0) { // scroll up
						zoom_ *= 1.1f;
					} else if (windowEvent.wheel.y < 0) { // scroll down						
						zoom_ *= 1 / 1.1f;
					}
					zoom_ = std::clamp(zoom_, ZOOM_MIN, ZOOM_MAX);
				}
				break;
			case SDL_KEYDOWN:
			    {
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
							tileBoard_.clear();
							tilesGraphic_.fill(sdl::Color{0.6f, 0.6f, 0.6f, 0.6f});
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
						case SDLK_r:
							{
								std::vector<hex::HexSides> tiles;
								for (const auto& hexImage : deck_) {
									tiles.push_back(hexImage.getHexSides());
								}
								tileBoard_.clear();
								tilesGraphic_.fill(sdl::Color{0.6f, 0.6f, 0.6f, 0.6f});
								hexMapGenerator_.fill(tileBoard_, tiles, {0, 0});
								addTileMapToGraphic();
							}
							break;
                    }
					logger()->info("Inv: {}", glm::inverse<>(projection_ * camera_.getView()));
					logger()->info("determinant: {}", glm::determinant(glm::inverse<>(projection_ * camera_.getView())));
                }
			    break;
			case SDL_MOUSEMOTION:
				if (isHovering_) {
					sdlMousePos = {windowEvent.motion.x, windowEvent.motion.y};
					if (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_MIDDLE)) {
						auto result = screenDeltaPosToWorld({windowEvent.motion.xrel, windowEvent.motion.yrel * viewPort_.size.x / viewPort_.size.y});
						camera_.move(result);
					}
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if (isHovering_) {
					switch (windowEvent.button.button) {
						case SDL_BUTTON_LEFT:
						{
							if (activateHexagon_) {
								const auto& button = windowEvent.button;
								hex::Hexi pos = getHexFromMouse(button.windowID, button.x, button.y);
								//logger()->info("(q, r, s): {}", hex);
								hex::HexSides sides = hexImage_.getHexSides();
								rotate(sides, rotations_);
								//logger()->info("Allowed {}", hexTileMap_.isAllowed(hexTile) ? "True" : "False");
								if (tileBoard_.put(pos, sides)) {
									HexImage image{hexImage_.getFilename(), hexImage_.getImage(), sides, hexImage_.isFlat(), rotations_};
									tilesGraphic_.fillTile(pos, image);
									logger()->error("ROTATE = {}", glm::rotate(Vec2{1, 0.f}, 0.f));
								}
							}
							break;
						}
						case SDL_BUTTON_MIDDLE:
							rotations_ = (rotations_ + 1) % 6;
							break;
					}
				}
				break;
		}
	}

} // Namespace vin.
