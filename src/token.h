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
            t_error         = -1,
            t_eof           = -2,
            t_identifier    = -3,
            t_number        = -4,
            t_string        = -5,

            t_function      = -6,
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

            value_type& operator=(value_type const& other)
            {
                if (this != &other)
                {
                    s = other.s;
                }
                return *this;
            }

            std::string   s;
            double        n;
            std::uint64_t i;
        };

        token(token_type token_, uint32_t line_, std::string file_)
            : token_(token_), line_(line_), file_(std::move(file_))
        {
        }

        token(token_type token_, uint32_t line_, std::string file_, std::string value)
            : token_(token_), line_(line_), file_(std::move(file_))
        {
            value_.s = std::move(value);
        }

        token(token_type token_, uint32_t line_, std::string file_, double value)
            : token_(token_), line_(line_), file_(std::move(file_))
        {
            value_.n = value;
        }

        token(token_type token_, uint32_t line_, std::string file_, std::uint64_t value)
            : token_(token_), line_(line_), file_(std::move(file_))
        {
            value_.i = value;
        }

        token()
        {
        }

        token(token&&) = default;
        token(token const&) = default;

        token& operator=(token&& other)
        {
            if (this != &other)
            {
                token_ = other.token_;
                value_ = std::move(other.value_);
                line_  = other.line_;
                file_  = other.file_;
            }

            return *this;
        }

        token& operator=(token const& other)
        {
            if (this != &other)
            {
                token_ = other.token_;
                value_ = other.value_;
                line_  = other.line_;
                file_  = other.file_;
            }

            return *this;
        }

        value_type value() const
        {
            return value_;
        }

        value_type& value()
        {
            return value_;
        }

        std::uint32_t line() const
        {
            return line_;
        }

        friend std::ostream& operator<<(std::ostream& os, token const& t)
        {
            return os << to_string(t);
        }

        static std::string to_string(token const& t)
        {
            return t.to_string();
        }

        std::string to_string() const;

        bool operator==(token_type token) const
        {
            return token_ == token;
        }

        bool operator==(token const& other) const
        {
            return *this == other.token_;
        }

        bool operator==(char ch) const
        {
            return *this == static_cast<token_type>(ch);
        }

    private:

        token_type      token_;
        value_type      value_;
        std::uint32_t   line_;  // Line number of token
        std::string     file_;  // File name of token
    };
}

#endif // LLVM_TEST_TOKEN_H
