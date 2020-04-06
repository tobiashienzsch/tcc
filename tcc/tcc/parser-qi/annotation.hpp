/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_MINIC_ANNOTATION_HPP)
#define BOOST_SPIRIT_MINIC_ANNOTATION_HPP

#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <map>

#include "tcc/parser-qi/ast.hpp"

namespace client {
///////////////////////////////////////////////////////////////////////////////
//  The annotation handler links the AST to a map of iterator positions
//  for the purpose of subsequent semantic error handling when the
//  program is being compiled.
///////////////////////////////////////////////////////////////////////////////
template <typename Iterator>
struct annotation {
  template <typename, typename>
  struct result {
    using type = void;
  };

  std::vector<Iterator>& iters;
  annotation(std::vector<Iterator>& iters) : iters(iters) {}

  struct set_id {
    using result_type = void;

    int id;
    set_id(int id) : id(id) {}

    void operator()(ast::FunctionCall& x) const { x.function_name.id = id; }

    void operator()(ast::Identifier& x) const { x.id = id; }

    template <typename T>
    void operator()(T&) const {
      // no-op
    }
  };

  void operator()(ast::operand& ast, Iterator pos) const {
    int id = iters.size();
    iters.push_back(pos);
    boost::apply_visitor(set_id(id), ast);
  }

  void operator()(ast::VariableDeclaration& ast, Iterator pos) const {
    int id = iters.size();
    iters.push_back(pos);
    ast.lhs.id = id;
  }

  void operator()(ast::Assignment& ast, Iterator pos) const {
    int id = iters.size();
    iters.push_back(pos);
    ast.lhs.id = id;
  }

  void operator()(ast::ReturnStatement& ast, Iterator pos) const {
    int id = iters.size();
    iters.push_back(pos);
    ast.id = id;
  }

  void operator()(ast::Identifier& ast, Iterator pos) const {
    int id = iters.size();
    iters.push_back(pos);
    ast.id = id;
  }
};
}  // namespace client

#endif
