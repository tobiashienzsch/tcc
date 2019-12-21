#pragma once

#include "fmt/format.h"

#include <cstdint>

#include <iostream>

namespace tcc
{
using Integer  = int64_t;
using UInteger = uint64_t;

/**
 * @brief Handy function for avoiding unused variable warnings.
 */
template<typename... Types>
constexpr auto IgnoreUnused(Types&&... /*unused*/) noexcept -> void
{
}

/**
 * @brief Handy helper struct for std::variant::vist.
 * See https://en.cppreference.com/w/cpp/utility/variant/visit
 */
template<class... Ts>
struct overloaded : Ts...
{
    using Ts::operator()...;
};

template<class... Ts>
overloaded(Ts...)->overloaded<Ts...>;

}  // namespace tcc