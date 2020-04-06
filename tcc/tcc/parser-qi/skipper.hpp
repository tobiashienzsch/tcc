#if !defined(TCC_PARSER_QI_SKIPPER_HPP)
#define TCC_PARSER_QI_SKIPPER_HPP

#include <boost/spirit/include/qi.hpp>

namespace client {
namespace parser {
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

//  The skipper grammar
template <typename Iterator>
struct skipper : qi::grammar<Iterator> {
  skipper() : skipper::base_type(start) {
    qi::char_type char_;
    ascii::space_type space;

    start = space                              // tab/space/cr/lf
            | "/*" >> *(char_ - "*/") >> "*/"  // C-style comments
        ;
  }

  qi::rule<Iterator> start;
};
}  // namespace parser
}  // namespace client

#endif
