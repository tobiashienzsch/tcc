#if !defined(TCC_PARSER_QI_ANNOTATION_HPP)
#define TCC_PARSER_QI_ANNOTATION_HPP

#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <map>

#include "tcc/parser/ast.hpp"

namespace tcc
{

/**
 *  @brief The annotation handler links the AST to a map of iterator positions
 *  for the purpose of subsequent semantic error handling when the
 *  program is being compiled.
 */
template<typename Iterator>
class Annotation
{
public:
    template<typename, typename>
    struct result
    {
        using type = void;
    };

    Annotation(std::vector<Iterator>& it) : iters(it) { }

    void operator()(ast::Operand& ast, Iterator pos) const
    {
        auto const id = static_cast<int>(iters.size());
        iters.push_back(pos);
        boost::apply_visitor(set_id(id), ast);
    }

    void operator()(ast::VariableDeclaration& ast, Iterator pos) const
    {
        auto const id = static_cast<int>(iters.size());
        iters.push_back(pos);
        ast.lhs.id = id;
    }

    void operator()(ast::Assignment& ast, Iterator pos) const
    {
        auto const id = static_cast<int>(iters.size());
        iters.push_back(pos);
        ast.lhs.id = id;
    }

    void operator()(ast::ReturnStatement& ast, Iterator pos) const
    {
        auto const id = static_cast<int>(iters.size());
        iters.push_back(pos);
        ast.id = id;
    }

    void operator()(ast::Identifier& ast, Iterator pos) const
    {
        auto const id = static_cast<int>(iters.size());
        iters.push_back(pos);
        ast.id = id;
    }

private:
    struct set_id
    {
        using result_type = void;
        int id;

        set_id(int i) : id(i) { }

        template<typename T>
        void operator()(T& /*unused*/) const
        {
        }
        void operator()(ast::FunctionCall& x) const { x.function_name.id = id; }
        void operator()(ast::Identifier& x) const { x.id = id; }
    };

    std::vector<Iterator>& iters;
};
}  // namespace tcc

#endif
