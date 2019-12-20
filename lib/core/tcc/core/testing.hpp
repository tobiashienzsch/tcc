#pragma once

namespace tcc
{

/**
 * @brief Handy helper struct for table driven tests.
 */
template<typename InputType, typename ExpectedType>
struct TestCase
{
    InputType input;
    ExpectedType expected;
};

}  // namespace tcc