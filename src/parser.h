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

#pragma once
#ifndef LLVM_TEST_PARSER_H
#define LLVM_TEST_PARSER_H

#include "ast.h"
#include "token.h"
#include "lexer.h"

namespace parser
{
    class parser
    {
    public:
        parser(lexer::lexer lexer)
            : lexer_(std::move(lexer))
        {
        }

        ast::node_pointer parse();

    private:
        lexer::lexer lexer_;
        std::unique_ptr<ast::function> top_;    // Top-level function
        lexer::token current_;                  // Current token

        bool match(int token);

        ast::node_pointer statement();
        ast::node_pointer conditional_statement();
        ast::node_pointer loop_statement();
        ast::node_pointer source_expression();
        ast::node_pointer destination_expression();
        ast::node_pointer conditional_expression();
        ast::node_pointer logic_and_expression();
        ast::node_pointer logic_or_expression();
        ast::node_pointer logic_not_expression();
        ast::node_pointer comparison_expression();
        ast::node_pointer add_sub_expression();
        ast::node_pointer mul_div_expression();
        ast::node_pointer bit_and_expression();
        ast::node_pointer bit_or_expression();
        ast::node_pointer bit_xor_expression();
        ast::node_pointer bit_shift_expression();
        ast::node_pointer prefix_expression();
        ast::node_pointer select_expression();
        ast::node_pointer suffix_expression();
        ast::node_pointer primary_expression();
        ast::node_pointer function_definition();
        ast::node_pointer class_definition();

        void expected(char, lexer::token);
        void expected(lexer::token, lexer::token);
        void expected(std::string const&, lexer::token);

        void expected(int ch)
        {
            expected(ch, current_);
        }

        void expected(lexer::token token)
        {
            expected(token, current_);
        }

        void expected(std::string const& exp)
        {
            expected(exp, current_);
        }
    };
}

#endif //LLVM_TEST_PARSER_H
