/**
 * @file warnings.hpp
 * @copyright Copyright 2019 Modern Circuits. All rights reserved.
 */

#pragma once

namespace tcc
{

/**
 * @brief Handy function for avoiding unused variable warnings.
 */
template<typename... Types>
constexpr auto IgnoreUnused(Types&&... /*unused*/) noexcept -> void
{
}

}  // namespace tcc