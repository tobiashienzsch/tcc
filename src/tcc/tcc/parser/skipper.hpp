#if !defined(TCC_PARSER_QI_SKIPPER_HPP)
#define TCC_PARSER_QI_SKIPPER_HPP

#include <boost/spirit/include/qi.hpp>

namespace tcc
{
namespace parser
{
namespace qi    = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

/**
 *   The skipper grammar
 */
template<typename Iterator>
struct Skipper : qi::grammar<Iterator>
{
    Skipper() : Skipper::base_type(start)
    {
        qi::char_type char_;
        ascii::space_type space;

        // clang-format off
    start = space                               
        | "/*"                              
        >> *(char_ - "*/") 
        >> "*/"  
    ;
        // clang-format on
    }

    qi::rule<Iterator> start;
};
}  // namespace parser
}  // namespace tcc

#endif
