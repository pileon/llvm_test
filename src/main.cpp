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
#include "parser.h"
#include "ast.h"

#ifdef __unix__
# include <unistd.h>
#endif

namespace
{
    std::string unescape(std::string const& in)
    {
        std::string out;

        for (auto const c : in)
        {
            switch (c)
            {
            case '\a':
                out += "\\a";
                break;
            case '\b':
                out += "\\b";
                break;
            case '\x1b':
                out += "\\e";
                break;
            case '\f':
                out += "\\f";
                break;
            case '\n':
                out += "\\n";
                break;
            case '\r':
                out += "\\r";
                break;
            case '\t':
                out += "\\t";
                break;
            case '\v':
                out += "\\v";
                break;

            default:
                out += c;
                break;
            }
        }

        return out;
    }

    void lexer_test()
    {
        std::istringstream input(
            R"(a boo
          foo bar
          12.34 + 1234
          hello = function(name) {
              print("Your name is ", name, "\n")
          }
          )");

        lexer::lexer l(input, "<stdin>");

        while (true)
        {
            auto token = l.next();

            std::cout << "Line " << token.line() << ": Got token " << token;
            if (token == lexer::token::t_number)
            {
                std::cout << " (" << token.value<double>() << ')';
            }
            else if (token == lexer::token::t_identifier || token == lexer::token::t_string)
            {
                std::cout << " (" << unescape(token.value<std::string>()) << ')';
            }
            else if (token == lexer::token::t_eof || token == lexer::token::t_error)
            {
                std::cout << '\n';
                break;
            }

            std::cout << '\n';
        }
    }

    void parser_test()
    {
        std::istringstream input(
            R"(foo = 123
               bar = foo
               hoo = "Hello"
               a = foo * bar - 123
               b = 1 = 2 and 2 != 3 or not foo
               c = 1 << 4 & 15 | 32
               d = |-1 + +5 - -5 & ~2
               e = true or false
               f = null
               g.h = y.z
               i = a[123 + 456]
               j = f(1, true, 34.32 * 1)
               k = if foo < 100 print("foo < 100") elif foo > 200 print("foo > 200") elif foo = 150 print("foo it exactly equal to 150") else print("foo is between 100 and 200")
               l = 2 * (3 + 5)
               m = function(a, b) {
                   x = a + b
                   y = a * b
               }
            )");

        lexer::lexer l(input, "<stdin>");
        parser::parser p(l);

        if (auto root = p.parse(); root != nullptr)
        {
            std::cout << root;
        }
    }
}

int main()
{
    //lexer_test();
    parser_test();

    // This seems to be needed when running in Clion from inside WSL
    std::cerr << std::flush;
    std::clog << std::flush;
    std::cout << std::endl << "Please press ENTER to continue..." << std::flush;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return 0;
}
