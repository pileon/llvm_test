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

        return ast::node_pointer();
    }

    ast::node_pointer parser::statement()
    {
        auto left = identifier();
        match('=');
        auto right = expression();

        return std::make_unique<ast::assignment>(std::move(left), std::move(right));
    }

    ast::node_pointer parser::identifier()
    {
        if (current_ == token::t_identifier)
        {
            auto id = std::make_unique<ast::identifier>(current_.value().s);
            current_ = lexer_.next();
            return std::move(id);
        }
        expected(token::t_identifier);

        return nullptr;
    }

    ast::node_pointer parser::expression()
    {
        ast::node_pointer node;

        if (current_ == token::t_number)
        {
            node = std::make_unique<ast::number>(current_.value().n);
        }
        else if (current_ == token::t_string)
        {
            node = std::make_unique<ast::string>(current_.value().s);
        }
        else if (current_ == token::t_identifier)
        {
            node = std::make_unique<ast::identifier>(current_.value().s);
        }
        else
        {
            expected("expression");
        }
        current_ = lexer_.next();

        return std::move(node);
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
}
