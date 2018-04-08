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

#include <iostream>
#include <sstream>
#include "lexer.h"

int main()
{
    std::istringstream input("a\n1 foo 23\nbar ho\n45.67 *\n7 | (boo)\n");
    lexer::lexer l(input, "<stdin>");

    while (true)
    {
        auto token = l.next();

        std::cout << "Line " << token.line() << ": Got token " << token;
        if (token == lexer::token::t_number)
        {
            std::cout << " (" << token.value().n << ')';
        }
        else if (token == lexer::token::t_identifier)
        {
            std::cout << " (" << token.value().s << ')';
        }
        else if (token == lexer::token::t_eof || token == lexer::token::t_error)
        {
            break;
        }

        std::cout << '\n';
    }
    return 0;
}
