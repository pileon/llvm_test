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

    lexer::token parser::match(char ch)
    {
        if (current_ == ch)
        {
            return current_ = lexer_.next();
        }
        else
        {
            expected(ch);
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
            match('=');
            auto right = source_expression();

            return std::make_unique<ast::assignment>(std::move(left), std::move(right));
        }

        return conditional_expression();
    }

    ast::node_pointer parser::identifier()
    {
        if (current_ == token::t_identifier)
        {
            ast::node_pointer id = std::make_unique<ast::identifier>(current_.value<std::string>());
            current_ = lexer_.next();
            return id;
        }
        expected(token::t_identifier);

        return nullptr;
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
        return logic_and_expression();
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
        return primary_expression();
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
        else
        {
            expected("primary expression");
        }

        current_ = lexer_.next();

        return node;
    }
}
