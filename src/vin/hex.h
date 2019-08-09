#ifndef VINLANDVIKINGS_VIN_HEX_H
#define VINLANDVIKINGS_VIN_HEX_H

#include <type_traits>

namespace vin {

	template <class Type>
	class Hex {
	public:
		constexpr Hex() noexcept : q_(0), r_(0) {
			IS_ARITHMETIC<Type>();
		}

		constexpr Hex(Type q, Type r) noexcept : q_(q), r_(r) {
			IS_ARITHMETIC<Type>();
		}

		constexpr Hex(const Hex& hex) noexcept = default;
		constexpr Hex(Hex&&) noexcept = default;

		Hex& operator=(const Hex& hex) noexcept = default;
		Hex& operator=(Hex&&) noexcept = default;

		constexpr Type q() const noexcept { return q_; }
		constexpr Type r() const noexcept { return r_; }
		constexpr Type s() const noexcept { return -q_ - r_; }

		constexpr bool operator==(Hex hex) const noexcept {
			return q_ == hex.q_ && r_ == hex.r_;
		}

		constexpr bool operator!=(Hex hex) const noexcept {
			return q_ != hex.q_ || r_ != hex.r_;
		}

		constexpr Hex operator+(Hex hex) const noexcept {
			return Hex(q_ + hex.q_, r_ + hex.r_);
		}

		constexpr Hex operator-(Hex hex) const noexcept {
			return Hex(q_ - hex.q_, r_ - hex.r_);
		}

		constexpr Hex operator-() const noexcept {
			return Hex(-q_, -r_);
		}

		constexpr Hex operator*(Type nbr) const noexcept {
			return Hex(q_ * nbr, r_ * nbr);
		}

		constexpr Hex operator+(Type hex) const noexcept {
			return Hex(q_ + hex.q_, r_ + hex.r_);
		}

		constexpr Hex operator-(Type hex) const noexcept {
			return Hex(q_ - hex.q_, r_ - hex.r_);
		}

	private:
		template <class AritmeticType>
		static constexpr void IS_ARITHMETIC() {
			static_assert(std::is_arithmetic<AritmeticType>(), "Hex type must be of arithmetic type");
		}

		Type q_, r_;
	};

	using Hexi = Hex<int>;
	using Hexf = Hex<float>;

	constexpr Hexi HEX_ZERO(0, 0);

	constexpr Hexi HEX_Q(1, 0);

	constexpr Hexi HEX_R(0, 1);

	constexpr Hexi HEX_S(-1, 1);

} // Namespace vin.

#endif // VINLANDVIKINGS_VIN_HEX_H
