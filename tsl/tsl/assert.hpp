#pragma once

#define TCC_ENABLE_ASSERTIONS 1
#if defined(TCC_ENABLE_ASSERTIONS)
#define TCC_ASSERT(x, msg)                                                                                             \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!x)                                                                                                        \
        {                                                                                                              \
            fmt::print("{}\n", msg);                                                                                   \
        }                                                                                                              \
                                                                                                                       \
    } while (false);

#else
#define TCC_ASSERT(x, msg) x
#endif