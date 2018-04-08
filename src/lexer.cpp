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

namespace lexer
{
    token lexer::next()
    {
        skip_space();

        int ch = input_.get();

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

            return token(token::t_identifier, line_, file_, word);
        }

        return token(token::t_error, line_, file_);
    }

    void lexer::skip_space()
    {
        while (isspace(input_.peek()))
        {
            int ch = input_.get();
            if (ch == '\n')
            {
                ++line_;
            }
        }
    }
}
