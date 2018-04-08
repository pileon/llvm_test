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
#ifndef LLVM_TEST_TOKEN_H
#define LLVM_TEST_TOKEN_H

#include <string>

namespace lexer
{
    class token
    {
    public:
        enum token_type
        {
            t_eof           = -1,
            t_identifier    = -2,
            t_number        = -4,

            // TODO: The keywords and operators
        };

        union value_type
        {
            value_type()
                : s{}
            {
            }

            value_type(value_type const& other)
                : s{other.s}
            {
            }

            ~value_type()
            {
            }

            std::string   s;
            double        n;
            std::uint64_t i;
        };

        value_type value() const
        {
            return value_;
        }

        value_type& value()
        {
            return value_;
        }

    private:

        token_type      token_;
        value_type      value_;
        std::uint32_t   line_;  // Line number of token
        std::string     file_;  // File name of token
    };
}

#endif // LLVM_TEST_TOKEN_H
