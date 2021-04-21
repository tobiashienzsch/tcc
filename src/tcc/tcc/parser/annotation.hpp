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
    struct Result
    {
        using type = void;
    };

    // NOLINTNEXTLINE(hicpp-explicit-conversions)
    Annotation(std::vector<Iterator>& it) : iters_(it) { }

    void operator()(ast::Operand& ast, Iterator pos) const
    {
        auto const id = static_cast<int>(iters_.size());
        iters_.push_back(pos);
        boost::apply_visitor(SetId(id), ast);
    }

    void operator()(ast::VariableDeclaration& ast, Iterator pos) const
    {
        auto const id = static_cast<int>(iters_.size());
        iters_.push_back(pos);
        ast.left.id = id;
    }

    void operator()(ast::Assignment& ast, Iterator pos) const
    {
        auto const id = static_cast<int>(iters_.size());
        iters_.push_back(pos);
        ast.left.id = id;
    }

    void operator()(ast::ReturnStatement& ast, Iterator pos) const
    {
        auto const id = static_cast<int>(iters_.size());
        iters_.push_back(pos);
        ast.id = id;
    }

    void operator()(ast::Identifier& ast, Iterator pos) const
    {
        auto const id = static_cast<int>(iters_.size());
        iters_.push_back(pos);
        ast.id = id;
    }

private:
    struct SetId
    {
        using result_type = void;
        int id;

        // NOLINTNEXTLINE(hicpp-explicit-conversions)
        SetId(int i) : id(i) { }

        template<typename T>
        void operator()(T& /*unused*/) const
        {
        }
        void operator()(ast::FunctionCall& x) const { x.funcName.id = id; }
        void operator()(ast::Identifier& x) const { x.id = id; }
    };

    std::vector<Iterator>& iters_;
};
}  // namespace tcc

#endif
