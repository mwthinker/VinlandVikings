#include <vin/types.h>

// Used by gtest to make error output easier to read, PrintTo is a overloaded function.

namespace vin {

	namespace hex {

		inline void PrintTo(const Hexf& hex, std::ostream* os) {
			*os << fmt::format("{}", hex);
		}

		inline void PrintTo(const Hexi& hex, std::ostream* os) {
			*os << fmt::format("{}", hex);
		}

	}

	inline void PrintTo(const vin::Vec2& p, std::ostream* os) {
		*os << fmt::format("{}", p);
	}

	inline void PrintTo(const Vec3& p, std::ostream* os) {
		*os << fmt::format("{}", p);
	}

	inline void PrintTo(const Vec4& p, std::ostream* os) {
		*os << fmt::format("{}", p);
	}

	inline void PrintTo(const Color& color, std::ostream* os) {
		*os << fmt::format("{}", color);
	}

	inline void PrintTo(const Mat4& mat, std::ostream* os) {
		*os << fmt::format("{}", mat);
	}

}

namespace glm {

	inline void PrintTo(const vin::Vec2& p, std::ostream* os) {
		*os << fmt::format("{}", p);
	}

	inline void PrintTo(const vin::Vec3& p, std::ostream* os) {
		*os << fmt::format("{}", p);
	}

	inline void PrintTo(const vin::Vec4& p, std::ostream* os) {
		*os << fmt::format("{}", p);
	}

	inline void PrintTo(const vin::Mat4& mat, std::ostream* os) {
		*os << fmt::format("{}", mat);
	}

}

namespace sdl {

	inline void PrintTo(const vin::Color& color, std::ostream* os) {
		*os << fmt::format("{}", color);
	}

}
