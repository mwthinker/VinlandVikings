#include "hexcanvas.h"
#include "hex/hexagon.h"
#include "logger.h"
#include "imguiextra.h"
#include "hexdimension.h"
#include "hex/shape.h"

namespace vin {

	namespace {

		constexpr HexDimension Dimension;

		constexpr float ZoomMin = 0.02f;
		constexpr float ZoomMax = 3.0f;

		constexpr auto ClearColor = sdl::color::html::DimGray;

		constexpr auto InvalidHexColor = Color{1.f, 0, 0, 0.4f};

		bool isMouseMiddleButtonDown() {
			return SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_MIDDLE);
		}
		
	}

	HexCanvas::HexCanvas()
		: coordinateSystem_{Dimension}
		, tilesGraphic_{Dimension, coordinateSystem_.getHexToWorld()}
		, commandManager_{*this} {

		auto hexes = hex::shape::createHex(10);

		tileBoard_ = hex::TileBoard(hexes.begin(), hexes.end());
		for (const auto& [hex, tile] : tileBoard_) {
			tilesGraphic_.fillGrid(hex, Red);
		}
		tilesGraphic_.fill(ClearColor);
	}

	void HexCanvas::setSize(int width, int height, const Viewport& viewport) {
		coordinateSystem_.setViewport(width, height, viewport);
	}

	void HexCanvas::addTileMapToGraphic() {
		Random random;
		for (const auto& [pos, sides] : tileBoard_) {
			auto tiles = tileLexicon_.getInvariantTiles(hex::TileInvariantKey{sides});
			if (tiles.size() > 0) {
				auto tile = tiles[random.generateInt(0, static_cast<int>(tiles.size() - 1))];
				if (tile.rotateUntilEqual(sides)) {
					tilesGraphic_.fillTile(pos, tile);
				} else {
					spdlog::warn("[HexCanvas] Something is wrong");
				}
			} else {
				spdlog::warn("[HexCanvas] Tile images failed to fill board");
			}
		}
	}

	hex::Hexi HexCanvas::getHexFromMouse() const {
		return coordinateSystem_.getHexFromScreen(mousePos_.x, mousePos_.y);
	}

	void HexCanvas::addMouseHexToGraphic() {
		hex::Hexi hex = getHexFromMouse();

		auto pos = coordinateSystem_.hexToWorld(hex);
		int rotation = currentTile_.sprite.rotations;
		auto spriteTile = currentTile_;
		for (int i = 0; i < 6; ++i) {
			if (tileBoard_.isAllowed(hex, spriteTile.tile)) {
				graphic_.addHexagonImage(pos, Dimension.outerSize, currentTile_.sprite.sprite, rotation * Pi / 3 + Dimension.angle);
				currentTile_ = spriteTile;
				return;
			}
			spriteTile.rotateLeft();
		}
		graphic_.addHexagonImage(pos, Dimension.outerSize, currentTile_.sprite.sprite, rotation * Pi / 3 + Dimension.angle);
		if (!tileBoard_.isAllowed(hex, currentTile_.tile)) {
			graphic_.addFilledHexagon(pos, Dimension.outerSize, InvalidHexColor, rotation * Pi / 3 + Dimension.angle);
		}
	}

	void HexCanvas::drawCanvas(sdl::Shader& shader, const std::chrono::high_resolution_clock::duration& deltaTime) {
		const auto& viewport = coordinateSystem_.getViewport();
		gl::glViewport(viewport.x, viewport.y, viewport.w, viewport.h);

		gl::glEnable(gl::GL_BLEND);
		gl::glBlendEquation(gl::GL_FUNC_ADD);
		gl::glBlendFunc(gl::GL_SRC_ALPHA, gl::GL_ONE_MINUS_SRC_ALPHA);

		auto matrix = coordinateSystem_.getMatrix(CoordinateSystem::Space::World, CoordinateSystem::Space::Clip);
		tilesGraphic_.setWorldToClip(matrix);
		tilesGraphic_.draw(shader);

		graphic_.clear();
		graphic_.setMatrix(matrix);
		addMouseHexToGraphic();
		graphic_.upload(shader);
	}

	Deck HexCanvas::getDeck() const {
		return deck_;
	}

	void HexCanvas::setDeck(const Deck& deck) {
		deck_ = deck;

		spdlog::info("[HexCanvas] Remove all history and future commands");
		commandManager_.clear();
	}

	void HexCanvas::setTileLexicon(const TileLexicon& tileLexicon) {
		tileLexicon_ = tileLexicon;
	}

	void HexCanvas::clear() {
		commandManager_.pushCommand([this]() {
			tilesGraphic_.fill(ClearColor);
			return tileBoard_.clear();
		});
	}

	void HexCanvas::zoomIn() {
		commandManager_.pushCommand([this]() {
			zoom(1.25f);
			spdlog::info("[HexCanvas] Zoom value");
			return false;
		});
	}

	void HexCanvas::zoomOut() {
		commandManager_.pushCommand([this]() {
			zoom(1.f / 1.25f);
			spdlog::info("[HexCanvas] Zoom");
			return false;
		});
	}

	void HexCanvas::setHexCoords(bool activate) {
		commandManager_.pushCommand([this, activate]() {
			tilesGraphic_.setHexCoords(activate);
			return false;
		});
	}

	void HexCanvas::setXYCoords(bool activate) {
		commandManager_.pushCommand([this, activate]() {
			tilesGraphic_.setXYCoords(activate);
			return false;
		});
	}

	void HexCanvas::setGrid(bool activate) {
		commandManager_.pushCommand([this, activate]() {
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
		commandManager_.pushCommand([this]() {
			std::vector<hex::Tile> tiles;
			for (const auto& key : deck_) {
				auto spriteTiles = tileLexicon_.getInvariantTiles(key);
				for (const auto& spriteTile : spriteTiles) {
					tiles.push_back(spriteTile.tile);
				}
			}
			tileBoard_.clear();
			tilesGraphic_.fill(ClearColor);
			hexMapGenerator_.fill(tileBoard_, tiles, {0, 0});
			addTileMapToGraphic();
			return true;
		});
	}

	bool HexCanvas::canRedo() const {
		return commandManager_.canRedo();
	}

	void HexCanvas::redo() {
		commandManager_.redo();
	}

	void HexCanvas::undo() {
		commandManager_.undo();
	}

	CanvasSnapshot HexCanvas::getSnapshot() const {
		return CanvasSnapshot{tileBoard_, tilesGraphic_.getMap()};
	}

	void HexCanvas::setSnapshot(const CanvasSnapshot& snapshot) {
		tileBoard_ = snapshot.tileBoard;
		tilesGraphic_.setMap(snapshot.tileMap);
	}

	void HexCanvas::zoom(float scale) {
		coordinateSystem_.zoom(scale);
	}

	void HexCanvas::move(float x, float y) {
		coordinateSystem_.move(x, y);
	}

	void HexCanvas::tilt(float angle) {
		coordinateSystem_.tilt(angle);
	}

	void HexCanvas::eventUpdate(const SDL_Event& windowEvent) {
		switch (windowEvent.type) {
			case SDL_MOUSEWHEEL:
				if (windowEvent.wheel.y > 0) {
					zoom(1.1f);
				}
				if (windowEvent.wheel.y < 0) {
					zoom(0.9f);
				}
				break;
			case SDL_KEYDOWN: {
				constexpr float Step = 10.f;
				switch (windowEvent.key.keysym.sym) {
					case SDLK_a:
						//spdlog::debug("[HexCanvas] Future size: {}, History size: {}", future_.size(), history_.size());
						break;
					case SDLK_LEFT:
						move(-Step, 0.f);
						break;
					case SDLK_RIGHT:
						move(Step, 0.f);
						break;
					case SDLK_UP:
						move(0, Step);
						break;
					case SDLK_DOWN:
						move(0, -Step);
						break;
					case SDLK_PAGEUP:
						tilt(-0.1f);
						break;
					case SDLK_PAGEDOWN:
						tilt(0.1f);
						break;
					break;
				}
				break;
			}
			case SDL_MOUSEMOTION: {
				mousePos_ = Vec2{windowEvent.motion.x, windowEvent.motion.y};
				if (isMouseMiddleButtonDown()) {
					auto delta = coordinateSystem_.getMatrix(CoordinateSystem::Space::Screen, CoordinateSystem::Space::World) * glm::vec4{windowEvent.motion.xrel, windowEvent.motion.yrel, 0.f, 0.f};
					move(delta.x, delta.y);
				}
				break;
			}
			case SDL_MOUSEBUTTONUP:
				auto hex = coordinateSystem_.getHexFromScreen(static_cast<float>(windowEvent.button.x), static_cast<float>(windowEvent.button.y));
				spdlog::info("Hex: {}", hex);
				switch (windowEvent.button.button) {
					case SDL_BUTTON_LEFT:
						//spdlog::info("{} {}", screenToClip_, glm::inverse(screenToClip_));
						spdlog::info("delta {}: ", coordinateSystem_.getMatrix(CoordinateSystem::Space::Screen, CoordinateSystem::Space::World) * glm::vec4{windowEvent.button.x, windowEvent.button.y, 0.f, 1.f});
						spdlog::info("delta2 {}\n: ", coordinateSystem_.getMatrix(CoordinateSystem::Space::Screen, CoordinateSystem::Space::World) * glm::vec4{windowEvent.button.x, windowEvent.button.y, 1.f, 1.f});
						spdlog::info("delta3 {}: ", coordinateSystem_.getMatrix(CoordinateSystem::Space::World, CoordinateSystem::Space::Screen) * glm::vec4{10.f, 10.f, 0.f, 1.f});
						spdlog::info("delta4 {}\n: ", coordinateSystem_.getMatrix(CoordinateSystem::Space::World, CoordinateSystem::Space::Screen) * glm::vec4{5.f, 50.f, 0.f, 1.f});
						if (activateHexagon_) {
							spdlog::info("(q, r, s): {}", hex);
							commandManager_.pushCommand([this, hex]() {
								if (tileBoard_.put(hex, currentTile_.tile)) {
									tilesGraphic_.fillTile(hex, currentTile_);
									return true;
								}
								return false;
							});
						}
						break;
					case SDL_BUTTON_MIDDLE:
						rotateCurrentTile(hex);
						break;
					case SDL_BUTTON_RIGHT:
						commandManager_.pushCommand([this, hex]() {
							if (tileBoard_.remove(hex)) {
								tilesGraphic_.clearTile(hex);
								tilesGraphic_.fillTile(hex, ClearColor);
								return true;
							}
							return false;
						});
						break;
				}
				break;
		}
	}

	void HexCanvas::rotateCurrentTile(hex::Hexi hex) {
		auto tile = currentTile_;
		for (int i = 1; i <= 6; ++i) {
			tile.rotateLeft();
			if (tileBoard_.isAllowed(hex, tile.tile)) {
				currentTile_.rotateLeft(i);
				break;
			}
		}
	}

}
