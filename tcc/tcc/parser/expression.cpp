#if defined(_MSC_VER)
#pragma warning(disable : 4345)
#endif

#include "tcc/parser/expression_def.hpp"

using iterator_type = std::string::const_iterator;
template struct tcc::parser::Expression<iterator_type>;
