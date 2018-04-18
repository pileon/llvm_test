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
        a.left->accept(*this);
        output_ << " = ";
        a.right->accept(*this);
    }

    void print_visitor::visit(function const& f)
    {
        output_ << "function(";

        for (auto arg = begin(f.arguments); arg != end(f.arguments); ++arg)
        {
            (*arg)->accept(this);
            if (arg + 1 != end(f.arguments))
            {
                output_ << ", ";
            }
        }
        output_ << ") {";

        indent_ += 4;
        for (auto const& stmt : f.statements)
        {
            possible_statement(stmt);
        }
        indent_ -= 4;
        output_ << '\n';
        output_ << indent(indent_) << '}';
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
        bool left_is_binary = dynamic_cast<binary*>(b.left.get()) != nullptr;
        bool right_is_binary = dynamic_cast<binary*>(b.right.get()) != nullptr;

        if (left_is_binary)
        {
            output_ << '(';
        }
        output_ << b.left;
        if (left_is_binary)
        {
            output_ << ')';
        }

        output_ << ' ';
        output_ << op(b.op) << ' ';

        if (right_is_binary)
        {
            output_ << '(';
        }
        output_ << b.right;
        if (right_is_binary)
        {
            output_ << ')';
        }

        if (b.op == '[')
        {
            output_ << " ]";
        }
    }

    void print_visitor::visit(unary const& u)
    {
        output_ << op(u.op) << ' ' << u.expression;
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

    void print_visitor::visit(conditional const& c)
    {
        output_ << "if " << c.condition_;

        if (!c.elifs_.empty())
        {
            output_ << '\n';

            output_ << indent(indent_ + 4);
            c.expression_->accept(this);
            output_ << '\n';

            for (auto const& elif_iter : c.elifs_)
            {
                auto const& elif = dynamic_cast<ast::binary const&>(*elif_iter);
                output_ << indent(indent_) << "elif ";
                elif.left->accept(this);
                indent_ += 4;
                possible_statement(elif.right);
                indent_ -= 4;
                output_ << '\n';
            }

            if (c.els_)
            {
                output_ << indent(indent_) << "else\n";
                output_ << indent(indent_ + 4);
                c.els_->accept(this);
            }
        }
        else
        {
            output_ << indent(indent_) << "else\n";
            output_ << indent(indent_ + 4);
            c.els_->accept(this);
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

    void print_visitor::visit(class_definition const& c)
    {
        output_ << "class";

        if (!c.inherits.empty())
        {
            output_ << " (";
            for (auto i = begin(c.inherits); i != end(c.inherits); ++i)
            {
                (*i)->accept(this);
                if (i + 1 != end(c.inherits))
                {
                    output_ << ", ";
                }
            }
            output_ << ')';
        }

        output_ << " {\n";

        indent_ += 4;
        for (auto const& property : c.properties)
        {
            output_ << indent(indent_);
            property->accept(this);
            output_ << '\n';
        }
        indent_ -= 4;

        output_ << indent(indent_) << '}';
    }

    void print_visitor::visit(ast::while_statement const& w)
    {
        output_ << "while ";
        w.condition_->accept(this);

        indent_ += 4;
        possible_statement(w.statement_);
        indent_ -= 4;
    }

    void print_visitor::visit(ast::block_statement const& b)
    {
        for (auto const& stmt : b.statements_)
        {
            possible_statement(stmt);
        }
    }

    void print_visitor::visit(for_in_statement const& f)
    {
        output_ << "for ";
        f.iterator_->accept(this);
        output_ << " : ";
        f.source_->accept(this);

        indent_ += 4;
        possible_statement(f.statement_);
        indent_ -= 4;
    }

    void print_visitor::possible_statement(ast::node_pointer const& stmt)
    {
        if (dynamic_cast<ast::block_statement*>(stmt.get()))
        {
            output_ << " {";
        }
        else
        {
            output_ << '\n';
        }

        output_ << indent(indent_);
        stmt->accept(this);

        if (dynamic_cast<ast::block_statement*>(stmt.get()))
        {
            output_ << '\n' << indent(indent_ - 4) << '}';
        }
    }
}
