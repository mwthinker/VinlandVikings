#ifndef VINLANDVIKINGS_VIN_IMGUIEXTRA_H
#define VINLANDVIKINGS_VIN_IMGUIEXTRA_H

#include "action.h"
#include "hex/hexagon.h"

#include <sdl/textureview.h>
#include <sdl/sprite.h>

#include <imgui.h>

namespace ImGui {

	bool Checkbox(vin::Action& action, bool active);

	bool MenuItem(vin::Action& action);

	void Bar(float height, const ImColor& color);

	void Hexagon(const sdl::TextureView& sprite, const ImVec2& size, bool flat = true);

	void Image(const sdl::TextureView& sprite, const ImVec2& size,
		const ImVec4& tint_col = ImVec4{1, 1, 1, 1}, const ImVec4& border_col = ImVec4{0, 0, 0, 0});

	bool ImageButton(const sdl::TextureView& texture, const ImVec2& size);

	void ImageBackground(const sdl::TextureView& sprite);

	bool ManButton(const char* idStr, int& nbr, int max, const sdl::TextureView& noMan, const sdl::TextureView& man,
		const ImVec2& size = {64.f, 64.f}, ImColor color = {1.f, 1.f, 1.f, 1.f});

	void LoadingBar(const ImColor& color1 = {0.8f, 0.8f, 0.8f, 1.f},
		const ImColor& color2 = {0.5f, 0.5f, 0.5f, 1.f},
		const ImVec2& size = {150.f, 25.f});

} // Namespace ImGui.

#endif // VINLANDVIKINGS_VIN_IMGUIEXTRA_H
