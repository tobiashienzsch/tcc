/**
 * @file variant.hpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */

#pragma once

#include <variant>

namespace tcc
{
/**
 * @brief Handy helper struct for std::variant::vist.
 * See https://en.cppreference.com/w/cpp/utility/variant/visit
 */
template<class... Ts>
struct Overloaded : Ts...
{
    using Ts::operator()...;
};

template<class... Ts>
Overloaded(Ts...) -> Overloaded<Ts...>;

}  // namespace tcc