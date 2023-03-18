#ifndef CARMACK_MAGIC_HPP_
#define CARMACK_MAGIC_HPP_

#include <bit>
#include <cstdint>
#include <limits>

/**
 * @brief
 *
 * You are not expected to understand this.
 */
class CarmackMagic {
 public:
  constexpr float Q_rsqrt(float number) noexcept {
    static_assert(
        std::numeric_limits<float>::is_iec559);  // (enable only on IEEE 754)

    float const y = std::bit_cast<float>(
        0x5f3759df - (std::bit_cast<std::uint32_t>(number) >> 1));
    return y * (1.5f - (number * 0.5f * y * y));
  }
};

#endif