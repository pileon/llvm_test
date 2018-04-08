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
#ifndef LLVM_TEST_LEXER_H
#define LLVM_TEST_LEXER_H

#include "token.h"

namespace lexer
{
    class lexer
    {
    public:
        lexer(std::istream& input_, std::string const& file_)
            : input_(input_), file_(file_), line_(1)
        {
        }

        token next();

    private:
        std::istream& input_;   // Current input stream
        std::string   file_;    // Name of current file
        std::uint32_t line_;    // Current line

        void skip_space();
        token check_keyword(std::string);
        std::string get_string();
    };
}

#endif //LLVM_TEST_LEXER_H
