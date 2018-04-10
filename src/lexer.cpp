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

#include "lexer.h"
#include <istream>
#include <unordered_map>

namespace lexer
{
    token lexer::next()
    {
        skip_space();

        int ch = input_.get();
        if (input_.eof() || ch == std::istream::traits_type::eof())
        {
            return token(token::t_eof, line_, file_);
        }

        if (std::isdigit(ch))
        {
            // A number
            std::string number;

            while (std::isdigit(ch))
            {
                number += static_cast<char>(ch);
                ch = input_.get();
            }

            if (ch == '.')
            {

                do
                {
                    number += static_cast<char>(ch);
                    ch = input_.get();
                } while (std::isdigit(ch));
            }

            // The last character we read was not part of the number, put it back
            input_.unget();

            return token(token::t_number, line_, file_, std::stod(number));
        }

        if (std::isalpha(ch) || ch == '_')
        {
            // An identifier or a keyword
            std::string word;
            while (std::isalnum(ch) || ch == '_')
            {
                word += static_cast<char>(ch);
                ch = input_.get();
            }

            // The last character we read was not part of the identifier, put it back
            input_.unget();

            return check_keyword(std::move(word));
        }

        if (ch == '\"')
        {
            // A string constant
            return token(token::t_string, line_, file_, get_string());
        }

        // Handle multi-character operators
        if (ch == '!')
        {
            int other = input_.get();
            if (other == '=')
            {
                return token(token::t_not_equal, line_, file_);
            }

            input_.unget();
        }

        if (ch == '~')
        {
            int other = input_.get();
            if (other == '=')
            {
                return token(token::t_aprx_equal, line_, file_);
            }

            if (other == '!')
            {
                other = input_.get();
                if (other == '=')
                {
                    return token(token::t_aprx_not_equal, line_, file_);
                }

                input_.unget();
            }

            input_.unget();
        }

        if (ch == '<')
        {
            int other = input_.get();
            if (other == '=')
            {
                return token(token::t_lt_equal, line_, file_);
            }

            if (other == '<')
            {
                return token(token::t_left_shift, line_, file_);
            }

            input_.unget();
        }

        if (ch == '>')
        {
            int other = input_.get();
            if (other == '=')
            {
                return token(token::t_gt_equal, line_, file_);
            }

            if (other == '>')
            {
                return token(token::t_right_shift, line_, file_);
            }

            input_.unget();
        }

        return token(input_.eof() ? token::t_eof : static_cast<token::token_type>(ch), line_, file_);
    }

    void lexer::skip_space()
    {
        while (std::isspace(input_.peek()))
        {
            int ch = input_.get();
            if (ch == '\n')
            {
                ++line_;
            }
        }
    }

    token lexer::check_keyword(std::string word)
    {
        static std::unordered_map<std::string, token::token_type> const keywords = {
            { "function"    , token::t_function     },
            { "and"         , token::t_and          },
            { "or"          , token::t_or           },
            { "not"         , token::t_not          },
            { "true"        , token::t_true         },
            { "false"       , token::t_false        },
            { "null"        , token::t_null         },
            { "if"          , token::t_if           },
            { "elif"        , token::t_elif         },
            { "else"        , token::t_else         },
        };

        if (auto const it = keywords.find(word); it != end(keywords))
        {
            return token(it->second, line_, file_, std::move(word));
        }

        return token(token::t_identifier, line_, file_, std::move(word));
    }

    std::string lexer::get_string()
    {
        std::string s;

        while (true)
        {
            int ch = input_.get();

            if (input_.eof() || ch == std::istream::traits_type::eof())
            {
                // TODO: ERROR!
                break;
            }

            if (ch == '\"')
            {
                break;
            }
            else if (ch == '\n')
            {
                ++line_;
            }
            else if (ch == '\\')
            {
                ch = input_.get();

                switch (ch)
                {
                case 'a':
                    ch = '\a';
                    break;
                case 'b':
                    ch = '\b';
                    break;
                case 'e':
                    ch = '\x1b';
                    break;
                case 'n':
                    ch = '\n';
                    break;
                case 'r':
                    ch = '\r';
                    break;
                case 't':
                    ch = '\t';
                    break;
                case 'v':
                    ch = '\v';
                    break;

                default:
                    // TODO: Warning
                    ch = ' ';
                    break;
                }
            }

            s += static_cast<char>(ch);
        }

        return s;
    }
}
