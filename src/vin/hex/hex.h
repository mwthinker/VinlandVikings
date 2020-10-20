#ifndef VINLANDVIKINGS_VIN_HEX_HEX_H
#define VINLANDVIKINGS_VIN_HEX_HEX_H

#include <type_traits>

namespace vin::hex {

	template <typename Type, typename = std::enable_if_t<std::is_arithmetic_v<Type>, Type>>
	class Hex {
	public:
		using value_type = Type;

		constexpr Hex() = default;

		constexpr Hex(Type q, Type r)
			: q_{q}
			, r_{r} {
		}

		constexpr Type q() const { return q_; }
		constexpr Type r() const { return r_; }
		constexpr Type s() const { return -q_ - r_; }

		constexpr bool operator==(Hex hex) const {
			return q_ == hex.q_ && r_ == hex.r_;
		}

		constexpr bool operator!=(Hex hex) const {
			return q_ != hex.q_ || r_ != hex.r_;
		}

		constexpr Hex operator+(Hex hex) const {
			return {q_ + hex.q_, r_ + hex.r_};
		}

		constexpr Hex operator-(Hex hex) const {
			return {q_ - hex.q_, r_ - hex.r_};
		}

		constexpr Hex operator-() const {
			return {-q_, -r_};
		}

		constexpr Hex operator*(Type nbr) const {
			return {q_ * nbr, r_ * nbr};
		}

		constexpr Hex operator+(Type hex) const {
			return {q_ + hex.q_, r_ + hex.r_};
		}

		constexpr Hex operator-(Type hex) const {
			return {q_ - hex.q_, r_ - hex.r_};
		}

	private:
		Type q_{0};
		Type r_{0};
	};

	using Hexi = Hex<int>;
	using Hexf = Hex<float>;

	constexpr Hexi HexiZero{0, 0};

	constexpr Hexi HexiQ{1, 0};

	constexpr Hexi HexiR{0, 1};

	constexpr Hexi HexiS{-1, 1};

	struct HexComparator {
		constexpr bool operator()(const Hexi& a, const Hexi& b) const {
			return (a.q() < b.q()) || (b.q() == a.q() && a.r() < b.r());
		}
	};

}

#endif
