#include "hexcanvas.h"
#include "hex/hexagon.h"
#include "logger.h"
#include "imguiextra.h"
#include "hexdimension.h"
#include "hex/shape.h"

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
		
		template <class T>
		void popAll(std::stack<T>& stack) {
			while (!stack.empty()) {
				stack.pop();
			}
		}

		constexpr HexDimension HEX_DIMENSION;

		constexpr float ZOOM_MIN = 0.02f;
		constexpr float ZOOM_MAX = 3.0f;

		const sdl::Color CLEAR_COLOR{0.6f, 0.6f, 0.6f, 0.6f};
		
	}

	HexCanvas::HexCanvas(const sdl::Shader& shader)
		: shader_{shader}
		, hexToWorldModel_{hex::createHexToCoordModel(HEX_DIMENSION.angle, HEX_DIMENSION.outerSize)}
		, tilesGraphic_{HEX_DIMENSION, hexToWorldModel_} {

		auto hexes = hex::shape::createHex(10);
		
		tileBoard_ = hex::TileBoard(hexes.begin(), hexes.end());
		for (const auto& [hex, tile] : tileBoard_) {
			tilesGraphic_.fillGrid(hex, RED);
		}
		tilesGraphic_.fill(CLEAR_COLOR);
	}

	void HexCanvas::updateCanvasSize(const Vec2& pos, const Vec2& size) {
		viewPort_ = {pos, size};

		glViewport(viewPort_);
		projection_ = ortho(viewPort_, zoom_);
	}

	void HexCanvas::addTileMapToGraphic() {
		Random random;
		for (const auto& [pos, sides] : tileBoard_) {
			auto tiles = tileLexicon_.getInvariantTiles(sides);
			if (tiles.size() > 0) {
				auto tile = tiles[random.generateInt(0, static_cast<int>(tiles.size() - 1))];
				if (tile.rotateUntilEqual(sides)) {
					tilesGraphic_.fillTile(pos, tile);
				} else {
					logger()->warn("Something is wrong");
				}
			} else {
				logger()->warn("Tile images failed to fill board");
			}
		}
	}

	hex::Hexi HexCanvas::worldToHex(Vec2 pos) const {
		auto hex = glm::inverse<>(hexToWorldModel_) * pos;
		return hex::hexRound({hex.x, hex.y});
	}

	Vec2 HexCanvas::hexToWorld(hex::Hexi pos) const {
		return hexToWorldModel_ * Vec2{pos.q(), pos.r()};
	}

	hex::Hexi HexCanvas::getHexFromMouse() const {
		ImVec2 pos = ImGui::GetMousePos();
		auto result = deviceCoordToClipSpace(viewPort_, sdlMousePos);
		Vec4 result2 = glm::inverse<>(projection_ * camera_.getView()) * Vec4 { result, 0.1f, 1.f };
		return worldToHex({result2.x, result2.y});
	}

	hex::Hexi HexCanvas::getHexFromMouse(Uint32 windowsId, int x, int y) const {
		auto result = deviceCoordToClipSpace(viewPort_, {x, y});
		Vec4 result2 = glm::inverse<>(projection_ * camera_.getView()) * Vec4 { result, 0.1f, 1.f };
		return worldToHex({result2.x, result2.y});
	}

	Vec2 HexCanvas::screenDeltaPosToWorld(Vec2 pos) {
		Vec4 rel{pos.x / -4.f / 100, pos.y / 4.f / 100, 0.f, 1.f};
		return glm::inverse<>(projection_) * rel;
	}

	void HexCanvas::addMouseHexToGraphic() {
		hex::Hexi hex = getHexFromMouse();

		auto pos = hexToWorld(hex);
		int rotation = currentTile_.sprite.rotations;
		auto tile = currentTile_;
		for (int i = 0; i < 6; ++i) {
			if (tileBoard_.isAllowed(hex, tile.sides)) {
				graphic_.addHexagonImage(pos, HEX_DIMENSION.outerSize, currentTile_.sprite.sprite, rotation * PI / 3 + HEX_DIMENSION.angle);
				currentTile_ = tile;
				return;
			}
			tile.rotateLeft();
		}
		graphic_.addHexagonImage(pos, HEX_DIMENSION.outerSize, currentTile_.sprite.sprite, rotation * PI / 3 + HEX_DIMENSION.angle);
		if (!tileBoard_.isAllowed(hex, currentTile_.sides)) {
			graphic_.addFilledHexagon(pos, HEX_DIMENSION.outerSize, Color{1.f, 0, 0, 0.4f}, rotation * PI / 3 + HEX_DIMENSION.angle);
		}
	}

	void HexCanvas::drawCanvas(const std::chrono::high_resolution_clock::duration& deltaTime) {
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		Mat4 model{1};
		graphic_.clearDraw();
		graphic_.pushMatrix(projection_ * camera_.getView() * model);

		tilesGraphic_.setMatrix(projection_ * camera_.getView() * model);
		addMouseHexToGraphic();
		graphic_.pushMatrix(projection_);

		//graphic_.addRectangle({-1.f, -1.f}, {2.f, 2.f}, RED); // Bottom
		tilesGraphic_.draw(shader_);
		graphic_.draw(shader_);
	}

	void HexCanvas::setDeck(const std::vector<HexImage>& deck) {
		deck_ = deck;

		logger()->info("[HexCanvas] Remove all history and future commands");
		popAll(future_);
		popAll(history_);
	}

	void HexCanvas::setTileLexicon(const TileLexicon& tileLexicon) {
		tileLexicon_ = tileLexicon;
	}

	void HexCanvas::clear() {
		pushCommand([this]() {
			tilesGraphic_.fill(CLEAR_COLOR);
			return tileBoard_.clear();
		});
	}

	void HexCanvas::zoomIn() {
		pushCommand([this]() {
			zoom_ *= 1.25f;
			zoom_ = std::clamp(zoom_, ZOOM_MIN, ZOOM_MAX);
			logger()->info("[HexCanvas] Zoom value: {}", zoom_);
			return false;
		});
	}

	void HexCanvas::zoomOut() {
		pushCommand([this]() {
			zoom_ *= 1 / 1.25f;
			zoom_ = std::clamp(zoom_, ZOOM_MIN, ZOOM_MAX);
			logger()->info("[HexCanvas] Zoom value: {}", zoom_);
			return false;
		});
	}

	void HexCanvas::setHexCoords(bool activate) {
		pushCommand([this, activate]() {
			tilesGraphic_.setHexCoords(activate);
			return false;
		});
	}

	void HexCanvas::setXYCoords(bool activate) {
		pushCommand([this, activate]() {
			tilesGraphic_.setXYCoords(activate);
			return false;
		});
	}

	void HexCanvas::setGrid(bool activate) {
		pushCommand([this, activate]() {
			tilesGraphic_.setGrid(activate);
			return false;
		});
	}

	bool HexCanvas::isHexCoords() const {
		return tilesGraphic_.isHexCoords();
	}

	bool HexCanvas::isXYCoords() const {
		return tilesGraphic_.isXYCoords();
	}

	bool HexCanvas::isGrid() const {
		return tilesGraphic_.isGrid();
	}

	void HexCanvas::clearAndGenerateMap() {
		pushCommand([this]() {
			std::vector<hex::HexSides> tiles;
			if (tiles.empty()) {
				for (const auto& hexImage : deck_) {
					tiles.push_back(hexImage.getHexSides());
				}
			}
			tileBoard_.clear();
			tilesGraphic_.fill(CLEAR_COLOR);
			hexMapGenerator_.fill(tileBoard_, tiles, {0, 0});
			addTileMapToGraphic();
			return true;
		});
	}

	void HexCanvas::redo() {
		if (!future_.empty()) {
			logger()->info("[HexCanvas] Future size: {}, History size: {}", future_.size(), history_.size());
			history_.push(State{tileBoard_, tilesGraphic_.getMap()});
			auto& state = future_.top();
			tileBoard_ = state.tileBoard;
			tilesGraphic_.fill(state.tileMap);
			future_.pop();
		}
	}

	void HexCanvas::undo() {
		if (!history_.empty()) {
			logger()->info("[HexCanvas] Future size: {}, History size: {}", future_.size(), history_.size());
			future_.emplace(State{tileBoard_, tilesGraphic_.getMap()});
			auto& state = history_.top();
			tileBoard_ = state.tileBoard;
			tilesGraphic_.fill(state.tileMap);
			history_.pop();
		}
	}

	void HexCanvas::pushCommand(const Command& command) {
		hex::TileBoard copy = tileBoard_;
		auto map = tilesGraphic_.getMap();
		if (command()) {
			logger()->info("[HexCanvas] Future size: {}, History size: {}", future_.size(), history_.size());
			popAll(future_);
			history_.push(State{copy, map});
		}
	}

	void HexCanvas::eventUpdate(const SDL_Event& windowEvent) {
		switch (windowEvent.type) {
			case SDL_MOUSEWHEEL:
				if (windowEvent.wheel.y > 0) { // scroll up
					zoom_ *= 1.1f;
				} else if (windowEvent.wheel.y < 0) { // scroll down
					zoom_ *= 1 / 1.1f;
				}
				zoom_ = std::clamp(zoom_, ZOOM_MIN, ZOOM_MAX);
				break;
			case SDL_KEYDOWN:
			{
				constexpr float STEP = 10.f;
				switch (windowEvent.key.keysym.sym) {
					case SDLK_a:
						logger()->info("[HexCanvas] Future size: {}, History size: {}", future_.size(), history_.size());
						break;
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
					break;
				}
			}
			break;
			case SDL_MOUSEMOTION:
			{
				sdlMousePos = {windowEvent.motion.x, windowEvent.motion.y};
				if (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_MIDDLE)) {
					auto result = screenDeltaPosToWorld({windowEvent.motion.xrel, windowEvent.motion.yrel * viewPort_.size.x / viewPort_.size.y});
					camera_.move(result);
				}
			}
			break;
			case SDL_MOUSEBUTTONUP:
				hex::Hexi hex = getHexFromMouse(windowEvent.button.windowID, windowEvent.button.x, windowEvent.button.y);
				switch (windowEvent.button.button) {
					case SDL_BUTTON_LEFT:
					{
						if (activateHexagon_) {
							//logger()->info("(q, r, s): {}", hex);
							pushCommand([this, hex]() {
								if (tileBoard_.put(hex, currentTile_.sides)) {
									tilesGraphic_.fillTile(hex, currentTile_);
									return true;
								}
								return false;
							});
						}
						break;
					}
					case SDL_BUTTON_MIDDLE:
					{
						auto tile = currentTile_;
						for (int i = 1; i <= 6; ++i) {
							tile.rotateLeft();
							if (tileBoard_.isAllowed(hex, tile.sides)) {
								currentTile_.rotateLeft(i);
								break;
							}
						}
						// All rotation failed, allow to rotate anyway.
						tile.rotateLeft();
						break;
					}
					case SDL_BUTTON_RIGHT:
					{
						pushCommand([this, hex]() {
							tilesGraphic_.clearTile(hex);
							tilesGraphic_.fillTile(hex, CLEAR_COLOR);
							return tileBoard_.remove(hex);
						});
						break;
					}
				}
				break;
		}
	}

} // Namespace vin.
