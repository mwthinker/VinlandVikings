#ifndef VINLANDVIKINGS_VIN__IMGUIEXTRA_H
#define VINLANDVIKINGS_VIN__IMGUIEXTRA_H

#include "action.h"
#include "textureview.h"

#include <sdl/sprite.h>

#include <imgui.h>

namespace ImGui {

	IMGUI_API bool Checkbox(vin::Action& action, bool active);

	IMGUI_API bool MenuItem(vin::Action& action);

	IMGUI_API void Bar(float height, const ImColor& color);

	IMGUI_API void Image(const vin::SpriteView& sprite, const ImVec2& size,
		const ImVec4& tint_col = ImVec4(1, 1, 1, 1), const ImVec4& border_col = ImVec4(0, 0, 0, 0));

	IMGUI_API void Image(const vin::SpriteView& sprite, const ImVec2& pos, ImVec2& size, float rotate, const ImColor& color);

	IMGUI_API bool ImageButton(const vin::SpriteView& texture, const ImVec2& size,
		const ImVec4& tint_col = ImVec4(1, 1, 1, 1), const ImVec4& border_col = ImVec4(0, 0, 0, 0));	

	IMGUI_API void ImageBackground(const vin::SpriteView& sprite);

	IMGUI_API bool ManButton(const char* idStr, int& nbr, int max, const vin::SpriteView& noMan, const vin::SpriteView& man,
		const ImVec2& size = {64.f, 64.f}, ImColor color = {1.f, 1.f, 1.f, 1.f});

	IMGUI_API void LoadingBar(const ImColor& color1 = {0.8f, 0.8f, 0.8f, 1.f},
		const ImColor& color2 = {0.5f, 0.5f, 0.5f, 1.f},
		const ImVec2& size = {150.f, 25.f});

} // Namespace ImGui.

#endif // VINLANDVIKINGS_VIN_
