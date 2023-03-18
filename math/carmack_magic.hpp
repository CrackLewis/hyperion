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
  float Q_rsqrt(float number) {
    union {
      float f;
      uint32_t i;
    } conv = {.f = number};
    conv.i = 0x5f3759df - (conv.i >> 1);
    conv.f *= 1.5F - (number * 0.5F * conv.f * conv.f);
    return conv.f;
  }
};

#endif