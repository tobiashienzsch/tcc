#pragma once

#define TCC_ENABLE_ASSERTIONS 1
#ifdef TCC_ENABLE_ASSERTIONS
#define TCC_ASSERT(x, ...)                                                                           \
    {                                                                                                \
        if (!(x))                                                                                    \
        {                                                                                            \
            fmt::print("Assertion Failed: {0}:{1}: {2}", __FILE__, __LINE__, __VA_ARGS__);           \
        }                                                                                            \
    }
#else
#define TCC_ASSERT(x, ...)
#endif