// Copyright 2018 Joachim Pileborg
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "ast.h"

namespace ast
{
    std::ostream& operator<<(std::ostream& os, print_visitor::indent const& ind)
    {
        for (std::uint32_t i = 0; i < ind.indent_; ++i)
        {
            os << ' ';
        }
        return os;
    }

    void print_visitor::visit(assignment const& a)
    {
        output_ << indent(indent_);
        a.left->accept(*this);
        output_ << " = ";
        a.right->accept(*this);
        output_ << '\n';
    }

    void print_visitor::visit(function const& f)
    {
        output_ << indent(indent_) << "function(";

        for (auto arg = begin(f.arguments); arg != end(f.arguments); ++arg)
        {
            (*arg)->accept(this);
            if (arg + 1 != end(f.arguments))
            {
                output_ << ", ";
            }
        }
        output_ << ") {\n";

        indent_ += 4;
        for (auto const& statement : f.statements)
        {
            statement->accept(this);
        }
        indent_ -= 4;
        output_ << std::setw(indent_) << "}\n";
    }

    void print_visitor::visit(identifier const& i)
    {
        output_ << i.name;
    }

    void print_visitor::visit(number const& n)
    {
        output_ << n.value;
    }

    void print_visitor::visit(string const& s)
    {
        output_ << '\"' << s.str << '\"';
    }

    void print_visitor::visit(binary const& b)
    {
        output_ << b.left << ' ' << b.op << ' ' << b.right;
    }

    void print_visitor::visit(unary const& u)
    {
        output_ << u.op << ' ' << u.expression;
    }

    void print_visitor::visit(call const&)
    {
    }
}
