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

namespace parser
{
    using namespace lexer;

    ast::node_pointer parser::parse()
    {
        top_ = std::make_unique<ast::function>();

        while (true)
        {
            current_ = lexer_.next();
            if (current_ == token::t_eof)
            {
                break;
            }

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

        return nullptr;
    }

    ast::node_pointer parser::expression()
    {
        return ast::node_pointer();
    }

    lexer::token parser::match(char ch)
    {
        if (current_ == ch)
        {
            return current_ = lexer_.next();
        }
        else
        {
            // TODO: Error
        }
    }
}
