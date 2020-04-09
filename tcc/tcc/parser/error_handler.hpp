#if !defined(TCC_PARSER_QI_ERROR_HANDLER_HPP)
#define TCC_PARSER_QI_ERROR_HANDLER_HPP

#include <iostream>
#include <string>
#include <vector>

#include "tsl/tsl.hpp"

namespace tcc
{

template<typename Iterator>
struct ErrorHandler
{
    template<typename, typename, typename>
    struct result
    {
        using type = void;
    };

    ErrorHandler(Iterator f, Iterator l, std::ostream& o)
        : first(f)
        , last(l)
        , out(o)
    {
    }

    template<typename Message, typename What>
    void operator()(Message const& message, What const& what,
                    Iterator err_pos) const
    {
        int line;
        Iterator line_start = get_pos(err_pos, line);
        if (err_pos != last)
        {
            auto msg = fmt::format("{0}{1} line {2}:\n{3}\n", message, what,
                                   line, get_line(line_start));
            for (; line_start != err_pos; ++line_start)
            {
                msg.append(" ");
            }
            msg.append("^\n");
            out << msg;
        }
        else
        {
            auto const msg
                = fmt::format("Unexpected end of file. {0}{1} line {2}:\n",
                              message, what, line);
            out << msg;
        }
    }

    [[nodiscard]] Iterator get_pos(Iterator err_pos, int& line) const
    {
        line                = 1;
        Iterator i          = first;
        Iterator line_start = first;
        while (i != err_pos)
        {
            bool eol = false;
            if (i != err_pos && *i == '\r')  // CR
            {
                eol        = true;
                line_start = ++i;
            }
            if (i != err_pos && *i == '\n')  // LF
            {
                eol        = true;
                line_start = ++i;
            }
            if (eol)
            {
                ++line;
            }
            else
            {
                ++i;
            }
        }
        return line_start;
    }

    [[nodiscard]] std::string get_line(Iterator err_pos) const
    {
        Iterator i = err_pos;
        // position i to the next EOL
        while (i != last && (*i != '\r' && *i != '\n'))
        {
            ++i;
        }
        return std::string(err_pos, i);
    }

    [[nodiscard]] std::vector<Iterator>& GetIterators() { return iters; }

private:
    Iterator first              = {};
    Iterator last               = {};
    std::vector<Iterator> iters = {};
    std::ostream& out;
};
}  // namespace tcc

#endif
