#ifndef VINLANDVIKINGS_VIN_HEX_HEX_H
#define VINLANDVIKINGS_VIN_HEX_HEX_H

#include <type_traits>

namespace vin::hex {

	template <typename Type, typename = std::enable_if_t<std::is_arithmetic_v<Type>, Type>>
	class Hex {
	public:
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

	constexpr Hexi HEXI_ZERO{0, 0};

	constexpr Hexi HEXI_Q{1, 0};

	constexpr Hexi HEXI_R{0, 1};

	constexpr Hexi HEXI_S{-1, 1};

}

#endif
