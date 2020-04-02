/**
 * @file warning.hpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */

#pragma once

namespace tcc {
/**
 * @brief Handy function for avoiding unused variable warnings.
 */
template <typename... Types>
constexpr auto IgnoreUnused(Types&&... /*unused*/) noexcept -> void {}
}  // namespace tcc