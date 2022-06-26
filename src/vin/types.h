#ifndef VINLANDVIKINGS_VIN_TYPES_H
#define VINLANDVIKINGS_VIN_TYPES_H

#include "hex/hex.h"

#include <sdl/color.h>

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <fmt/format.h>

#include <iostream>

namespace vin {

	using Mat4 = glm::mat4;
	using Mat3 = glm::mat3;
	using Mat2 = glm::mat2;
	using Vec2 = glm::vec2;
	using Vec3 = glm::vec3;
	using Vec4 = glm::vec4;

	constexpr auto Pi = glm::pi<float>();

	using Color = sdl::Color;

	constexpr Color White = sdl::color::html::White;
	constexpr Color Black = sdl::color::html::Black;
	constexpr Color Red = sdl::color::Red;
	constexpr Color Green = sdl::color::Green;
	constexpr Color Blue = sdl::color::Blue;
	constexpr Color Cyan = sdl::color::html::Cyan;
	constexpr Color Orange = sdl::color::html::Orange;
	
	template <class T>
	Vec2 castFloat(const T& t) {
		return {static_cast<float>(t.x), static_cast<float>(t.y)};
	}

}

template <>
struct fmt::formatter<vin::Vec2> {
	template <typename ParseContext>
	constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

	template <typename FormatContext>
	auto format(const vin::Vec2& p, FormatContext& ctx) {
		return fmt::format_to(ctx.out(), "({:.{}f}, {:.{}f})", p.x, 3, p.y, 3);
	}
};

template <>
struct fmt::formatter<vin::hex::Hexf> {
	template <typename ParseContext>
	constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

	template <typename FormatContext>
	auto format(const vin::hex::Hexf& hex, FormatContext& ctx) {
		return fmt::format_to(ctx.out(), "({:.{}f}, {:.{}f}, {:.{}f})", hex.q(), 3, hex.r(), 3, hex.s(), 3);
	}
};

template <>
struct fmt::formatter<vin::hex::Hexi> {
	template <typename ParseContext>
	constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

	template <typename FormatContext>
	auto format(const vin::hex::Hexi& hex, FormatContext& ctx) {
		return fmt::format_to(ctx.out(), "({}, {}, {})", hex.q(), hex.r(), hex.s());
	}
};

template <>
struct fmt::formatter<vin::Vec3> {
	template <typename ParseContext>
	constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

	template <typename FormatContext>
	auto format(const vin::Vec3& p, FormatContext& ctx) {
		return fmt::format_to(ctx.out(), "({:.{}f}, {:.{}f}, {:.{}f})", p.x, 3, p.y, 3, p.z, 3);
	}
};

template <>
struct fmt::formatter<vin::Vec4> {
	template <typename ParseContext>
	constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

	template <typename FormatContext>
	auto format(const vin::Vec4& p, FormatContext& ctx) {
		return fmt::format_to(ctx.out(), "({:.{}f}, {:.{}f}, {:.{}f}, {:.{}f})", p.x, 3, p.y, 3, p.z, 3, p.w, 3);
	}
};

template <>
struct fmt::formatter<vin::Color> {
	template <typename ParseContext>
	constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

	template <typename FormatContext>
	auto format(const vin::Color& color, FormatContext& ctx) {
		return fmt::format_to(ctx.out(), "{0:#x}", color.toImU32());
	}
};

template <>
struct fmt::formatter<vin::Mat4> {
	template <typename ParseContext>
	constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

	template <typename FormatContext>
	auto format(const vin::Mat4& mat, FormatContext& ctx) {
		return fmt::format_to(ctx.out(), "\n{}\n{}\n{}\n{}\n", mat[0], mat[1], mat[2], mat[3]);
	}
};

#endif
