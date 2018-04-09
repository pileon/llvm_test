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

        lexer::token match(char ch);

        ast::node_pointer statement();
        ast::node_pointer identifier();
        ast::node_pointer expression();
    };
}

#endif //LLVM_TEST_PARSER_H
