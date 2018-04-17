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

#include "parser.h"
#include <memory>
#include <iostream>

namespace parser
{
    using namespace lexer;

    ast::node_pointer parser::parse()
    {
        top_ = std::make_unique<ast::function>();
        current_ = lexer_.next();

        while (current_ != token::t_eof)
        {
            auto stmt = statement();
            if (stmt != nullptr)
            {
                top_->statements.push_back(std::move(stmt));
            }
        }

        return std::move(top_);
    }

    bool parser::match(int token)
    {
        if (current_ == token)
        {
            current_ = lexer_.next();
            return true;
        }
        else
        {
            expected(token);
            return false;
        }
    }

    void parser::expected(char ch, token tok)
    {
        std::cout << current_.file() << ':' << current_.line() << " :: Syntax error: Expected '" << ch << "', got " << tok << '\n';
    }

    void parser::expected(token expected, token actual)
    {
        std::cout << current_.file() << ':' << current_.line() << " :: Syntax error: Expected " << expected << ", got " << actual << '\n';
    }

    void parser::expected(std::string const& exp, token actual)
    {
        std::cout << current_.file() << ':' << current_.line() << " :: Syntax error: Expected " << exp << ", got " << actual << '\n';
    }

    ast::node_pointer parser::statement()
    {
        if (current_ == token::t_identifier)
        {
            auto left = destination_expression();

            if (current_ == '=')
            {
                match('=');
                auto right = source_expression();

                return std::make_unique<ast::assignment>(std::move(left), std::move(right));
            }
            else
            {
                return std::move(left);
            }
        }

        if (current_ == token::t_for || current_ == token::t_while)
        {
            return loop_statement();
        }

        if (current_ == token::t_if)
        {
            return conditional_statement();
        }

        // TODO: Other statements

        return conditional_expression();
    }

    ast::node_pointer parser::source_expression()
    {
        return conditional_expression();
    }

    ast::node_pointer parser::destination_expression()
    {
        return select_expression();
    }

    ast::node_pointer parser::conditional_expression()
    {
        if (current_ == token::t_if)
        {
            current_ = lexer_.next();

            auto condition = logic_and_expression();
            auto expr = source_expression();

            std::vector<ast::node_pointer> elifs;
            while (current_ == token::t_elif)
            {
                current_ = lexer_.next();
                auto left = logic_and_expression();
                auto right = source_expression();
                elifs.push_back(std::make_unique<ast::binary>(token::t_elif, std::move(left), std::move(right)));
            }

            match(token::t_else);

            return std::make_unique<ast::conditional>(std::move(condition), std::move(expr), std::move(elifs), std::move(source_expression()));
        }
        else
        {
            return logic_and_expression();
        }
    }

    ast::node_pointer parser::logic_and_expression()
    {
        auto left = logic_or_expression();

        if (current_ == token::t_and)
        {
            current_ = lexer_.next();
            return std::make_unique<ast::binary>(token::t_and, std::move(left), std::move(logic_and_expression()));
        }

        return left;
    }

    ast::node_pointer parser::logic_or_expression()
    {
        auto left = logic_not_expression();

        if (current_ == token::t_or)
        {
            current_ = lexer_.next();
            return std::make_unique<ast::binary>(token::t_or, std::move(left), std::move(logic_or_expression()));
        }

        return left;
    }

    ast::node_pointer parser::logic_not_expression()
    {
        if (current_ == token::t_not)
        {
            current_ = lexer_.next();
            return std::make_unique<ast::unary>(token::t_not, std::move(logic_not_expression()));
        }
        else
        {
            return comparison_expression();
        }
    }

    ast::node_pointer parser::comparison_expression()
    {
        auto left = add_sub_expression();

        if (current_ == '=')
        {
            current_ = lexer_.next();
            return std::make_unique<ast::binary>('=', std::move(left), std::move(add_sub_expression()));
        }
        else if (current_ == '<')
        {
            current_ = lexer_.next();
            return std::make_unique<ast::binary>('<', std::move(left), std::move(add_sub_expression()));
        }
        else if (current_ == '>')
        {
            current_ = lexer_.next();
            return std::make_unique<ast::binary>('>', std::move(left), std::move(add_sub_expression()));
        }
        else if (current_ == token::t_not_equal)
        {
            current_ = lexer_.next();
            return std::make_unique<ast::binary>(token::t_not_equal, std::move(left), std::move(add_sub_expression()));
        }
        else if (current_ == token::t_lt_equal)
        {
            current_ = lexer_.next();
            return std::make_unique<ast::binary>(token::t_lt_equal, std::move(left), std::move(add_sub_expression()));
        }
        else if (current_ == token::t_gt_equal)
        {
            current_ = lexer_.next();
            return std::make_unique<ast::binary>(token::t_gt_equal, std::move(left), std::move(add_sub_expression()));
        }
        else if (current_ == token::t_aprx_equal)
        {
            current_ = lexer_.next();
            return std::make_unique<ast::binary>(token::t_aprx_equal, std::move(left), std::move(add_sub_expression()));
        }
        else if (current_ == token::t_aprx_not_equal)
        {
            current_ = lexer_.next();
            return std::make_unique<ast::binary>(token::t_aprx_not_equal, std::move(left), std::move(add_sub_expression()));
        }
        else
        {
            return left;
        }
    }

    ast::node_pointer parser::add_sub_expression()
    {
        auto left = mul_div_expression();

        while (true)
        {
            if (current_ == '+')
            {
                current_ = lexer_.next();
                return std::make_unique<ast::binary>('+', std::move(left), std::move(add_sub_expression()));
            }
            else if (current_ == '-')
            {
                current_ = lexer_.next();
                return std::make_unique<ast::binary>('-', std::move(left), std::move(add_sub_expression()));
            }
            else
            {
                break;
            }
        }

        return left;
    }

    ast::node_pointer parser::mul_div_expression()
    {
        auto left = bit_and_expression();

        while (true)
        {
            if (current_ == '*')
            {
                current_ = lexer_.next();
                return std::make_unique<ast::binary>('*', std::move(left), std::move(mul_div_expression()));
            }
            else if (current_ == '/')
            {
                current_ = lexer_.next();
                return std::make_unique<ast::binary>('/', std::move(left), std::move(mul_div_expression()));
            }
            else if (current_ == '%')
            {
                current_ = lexer_.next();
                return std::make_unique<ast::binary>('%', std::move(left), std::move(mul_div_expression()));
            }
            else
            {
                break;
            }
        }

        return left;
    }

    ast::node_pointer parser::bit_and_expression()
    {
        auto left = bit_or_expression();

        if (current_ == '&')
        {
            current_ = lexer_.next();
            return std::make_unique<ast::binary>('&', std::move(left), std::move(bit_and_expression()));
        }

        return left;
    }

    ast::node_pointer parser::bit_or_expression()
    {
        auto left = bit_xor_expression();

        if (current_ == '|')
        {
            current_ = lexer_.next();
            return std::make_unique<ast::binary>('|', std::move(left), std::move(bit_or_expression()));
        }

        return left;
    }

    ast::node_pointer parser::bit_xor_expression()
    {
        auto left = bit_shift_expression();

        if (current_ == '^')
        {
            current_ = lexer_.next();
            return std::make_unique<ast::binary>('^', std::move(left), std::move(bit_xor_expression()));
        }

        return left;
    }

    ast::node_pointer parser::bit_shift_expression()
    {
        auto left = prefix_expression();

        if (current_ == token::t_left_shift)
        {
            current_ = lexer_.next();
            return std::make_unique<ast::binary>(token::t_left_shift, std::move(left), std::move(prefix_expression()));
        }
        else if (current_ == token::t_right_shift)
        {
            current_ = lexer_.next();
            return std::make_unique<ast::binary>(token::t_right_shift, std::move(left), std::move(prefix_expression()));
        }
        else
        {
            return left;
        }
    }

    ast::node_pointer parser::prefix_expression()
    {
        if (current_ == '+')
        {
            current_ = lexer_.next();
            return select_expression();
        }
        else if (current_ == '-')
        {
            current_ = lexer_.next();
            return std::make_unique<ast::unary>('-', std::move(select_expression()));
        }
        else if (current_ == '~')
        {
            current_ = lexer_.next();
            return std::make_unique<ast::unary>('~', std::move(prefix_expression()));
        }
        else if (current_ == '|')
        {
            current_ = lexer_.next();
            return std::make_unique<ast::unary>('|', std::move(prefix_expression()));
        }
        else
        {
            return select_expression();
        }
    }

    ast::node_pointer parser::select_expression()
    {
        auto left = suffix_expression();

        if (current_ == '.')
        {
            current_ = lexer_.next();
            return std::make_unique<ast::binary>('.', std::move(left), std::move(select_expression()));
        }

        return left;
    }

    ast::node_pointer parser::suffix_expression()
    {
        auto left = primary_expression();

        if (current_ == '(')
        {
            std::vector<ast::node_pointer> arguments;
            do
            {
                current_ = lexer_.next();
                arguments.push_back(source_expression());
            } while (current_ == ',');

            match(')');

            return std::make_unique<ast::call>(std::move(left), std::move(arguments));
        }
        else if (current_ == '[')
        {
            current_ = lexer_.next();
            auto right = source_expression();
            match(']');
            return std::make_unique<ast::binary>('[', std::move(left), std::move(right));
        }
        else
        {
            return left;
        }
    }

    ast::node_pointer parser::primary_expression()
    {
        ast::node_pointer node;
        if (current_ == token::t_number)
        {
            node = std::make_unique<ast::number>(current_.value<double>());
        }
        else if (current_ == token::t_string)
        {
            node = std::make_unique<ast::string>(current_.value<std::string>());
        }
        else if (current_ == token::t_identifier)
        {
            node = std::make_unique<ast::identifier>(current_.value<std::string>());
        }
        else if (current_ == token::t_true)
        {
            node = std::make_unique<ast::special_value>(token::t_true);
        }
        else if (current_ == token::t_false)
        {
            node = std::make_unique<ast::special_value>(token::t_false);
        }
        else if (current_ == token::t_null)
        {
            node = std::make_unique<ast::special_value>(token::t_null);
        }
        else if (current_ == '(')
        {
            current_ = lexer_.next();
            node = source_expression();
            match(')');
            return node;
        }
        else if (current_ == token::t_function)
        {
            current_ = lexer_.next();
            return function_definition();
        }
        else if (current_ == token::t_class)
        {
            current_ = lexer_.next();
            return class_definition();
        }
        else
        {
            expected("primary expression");
        }

        // TODO: Literal lists, functions, classes etc.

        current_ = lexer_.next();

        return node;
    }

    ast::node_pointer parser::function_definition()
    {
        auto fun = std::make_unique<ast::function>();
        match('(');

        std::vector<ast::node_pointer> arguments;
        while (current_ == token::t_identifier)
        {
            arguments.push_back(std::make_unique<ast::identifier>(current_.value<std::string>()));
            current_ = lexer_.next();

            if (current_ == ')')
            {
                break;
            }

            match(',');
        }

        match(')');

        match('{');

        std::vector<ast::node_pointer> statements;
        while (current_ != '}')
        {
            statements.push_back(std::move(statement()));
        }

        match('}');

        fun->arguments  = std::move(arguments);
        fun->statements = std::move(statements);
        return std::move(fun);
    }

    ast::node_pointer parser::class_definition()
    {
        auto cls = std::make_unique<ast::class_definition>();

        if (current_ == '(')
        {
            do
            {
                current_ = lexer_.next();

                auto base = source_expression();
                cls->inherits.push_back(std::move(base));
            } while (current_ == ',');

            match(')');
        }

        if (current_ == '{')
        {
            current_ = lexer_.next();

            do
            {
                auto dest = destination_expression();
                match('=');
                auto source = source_expression();

                cls->properties.push_back(std::make_unique<ast::assignment>(std::move(dest), std::move(source)));
            } while (current_ != '}');

            match('}');
        }

        return std::move(cls);
    }

    ast::node_pointer parser::conditional_statement()
    {
        if (current_ == token::t_if)
        {
            return if_statement();
        }
    }

    ast::node_pointer parser::loop_statement()
    {
        if (current_ == token::t_while)
        {
            return while_statement();
        }
        if (current_ == token::t_for)
        {
            return for_statement();
        }
    }

    ast::node_pointer parser::while_statement()
    {
        return ast::node_pointer();
    }

    ast::node_pointer parser::for_statement()
    {
        return ast::node_pointer();
    }

    ast::node_pointer parser::if_statement()
    {
        return ast::node_pointer();
    }
}
