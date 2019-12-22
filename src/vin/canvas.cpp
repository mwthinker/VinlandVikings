#include "hexcanvas.h"
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
	
	void Canvas::eventUpdate(const SDL_Event& windowEvent) {

	}

	void Canvas::updateCanvasSize() {
		auto pos = ImGui::GetWindowPos();
		auto size = ImGui::GetWindowSize();
		const auto& s = ImGui::GetIO().DisplaySize;
		viewPort_ = {{pos.x, s.y - size.y - pos.y}, {size.x, size.y}};

		glViewport(viewPort_);
		isHovering_ = ImGui::IsWindowFocused();
	}

	Mat4 Canvas::getProjection() const {
		return projection_;
	}

} // Namespace vin.
