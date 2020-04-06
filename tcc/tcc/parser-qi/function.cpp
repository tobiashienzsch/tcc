#if defined(_MSC_VER)
#pragma warning(disable : 4345)
#endif

#include "tcc/parser-qi/function_def.hpp"

using iterator_type = std::string::const_iterator;
template struct client::parser::function<iterator_type>;
