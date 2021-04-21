#if !defined(TCC_PARSER_QI_ERROR_HANDLER_HPP)
#define TCC_PARSER_QI_ERROR_HANDLER_HPP

#include <iostream>
#include <string>
#include <vector>

#include "tcsl/tcsl.hpp"

namespace tcc
{

template<typename Iterator>
struct ErrorHandler
{
    template<typename, typename, typename>
    struct Result
    {
        using type = void;
    };

    ErrorHandler(Iterator f, Iterator l, std::ostream& o) : first_(f), last_(l), out_(o) { }

    template<typename Message, typename What>
    void operator()(Message const& message, What const& what, Iterator errPos) const
    {
        int line           = 0;
        Iterator lineStart = getPos(errPos, line);
        if (errPos != last_)
        {
            auto msg = fmt::format("{0}{1} line {2}:\n{3}\n", message, what, line, getLine(lineStart));
            for (; lineStart != errPos; ++lineStart) { msg.append(" "); }
            msg.append("^\n");
            out_ << msg;
        }
        else
        {
            auto const msg = fmt::format("Unexpected end of file. {0}{1} line {2}:\n", message, what, line);
            out_ << msg;
        }
    }

    [[nodiscard]] auto getPos(Iterator errPos, int& line) const -> Iterator
    {
        line               = 1;
        Iterator i         = first_;
        Iterator lineStart = first_;
        while (i != errPos)
        {
            bool eol = false;
            if (i != errPos && *i == '\r')  // CR
            {
                eol       = true;
                lineStart = ++i;
            }
            if (i != errPos && *i == '\n')  // LF
            {
                eol       = true;
                lineStart = ++i;
            }
            if (eol) { ++line; }
            else
            {
                ++i;
            }
        }
        return lineStart;
    }

    [[nodiscard]] auto getLine(Iterator errPos) const -> std::string
    {
        Iterator i = errPos;
        // position i to the next EOL
        while (i != last_ && (*i != '\r' && *i != '\n')) { ++i; }
        return std::string(errPos, i);
    }

    [[nodiscard]] auto getIterators() -> std::vector<Iterator>& { return iters_; }

private:
    Iterator first_              = {};
    Iterator last_               = {};
    std::vector<Iterator> iters_ = {};
    std::ostream& out_;
};
}  // namespace tcc

#endif
