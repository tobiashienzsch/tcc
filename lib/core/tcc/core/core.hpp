#pragma once

#include <cstdint>

namespace tcc
{
using Integer  = int32_t;
using UInteger = uint32_t;

/**
 * @brief Handy function for avoiding unused variable warnings.
 */
template<typename... Types>
constexpr auto IgnoreUnused(Types&&... /*unused*/) noexcept -> void
{
}

}  // namespace tcc