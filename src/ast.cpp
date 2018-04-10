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
#include "token.h"

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
        //output_ << b.left << ' ' << print_visitor::op(b.op) << ' ' << b.right;
        output_ << b.left;
        output_ << ' ';
        output_ << op(b.op) << ' ';
        output_ << b.right;
        if (b.op == '[')
        {
            output_ << " ]";
        }
    }

    void print_visitor::visit(unary const& u)
    {
        output_ << op(u.op) << u.expression;
    }

    void print_visitor::visit(call const& c)
    {
        output_ << c.name << '(';
        for (auto arg = begin(c.arguments); arg != end(c.arguments); ++arg)
        {
            output_ << *arg;
            if (arg + 1 != end(c.arguments))
            {
                output_ << ", ";
            }
        }
        output_ << ')';
    }

    void print_visitor::visit(special_value const& v)
    {
        if (v.op == lexer::token::t_true)
        {
            output_ << "true";
        }
        else if (v.op == lexer::token::t_false)
        {
            output_ << "false";
        }
        else if (v.op == lexer::token::t_null)
        {
            output_ << "null";
        }
        else
        {
            output_ << "<unknown value>";
        }
    }

    std::string print_visitor::op(int oper)
    {
        if (oper >= 0)
        {
            return std::string(1, oper);
        }
        else
        {
            switch (oper)
            {
            case lexer::token::t_and:
                return "and";
            case lexer::token::t_or:
                return "or";
            case lexer::token::t_not:
                return "not";
            case lexer::token::t_not_equal:
                return "!=";
            case lexer::token::t_lt_equal:
                return "<=";
            case lexer::token::t_gt_equal:
                return ">=";
            case lexer::token::t_aprx_equal:
                return "~=";
            case lexer::token::t_aprx_not_equal:
                return "~!=";
            case lexer::token::t_left_shift:
                return "<<";
            case lexer::token::t_right_shift:
                return ">>";

            default:
                return "<unknown>";
            }
        }
    }
}
