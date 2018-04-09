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
#ifndef LLVM_TEST_AST_H
#define LLVM_TEST_AST_H

#include <string>
#include <memory>
#include <vector>

namespace ast
{
    struct ast_base
    {
        virtual ~ast_base() = default;
    };

    using node_pointer = std::unique_ptr<ast_base>;

    struct assignment : ast_base
    {
        assignment(node_pointer l, node_pointer r)
            : left(std::move(l)), right(std::move(r))
        {
        }

        node_pointer left;
        node_pointer right;
    };

    struct number : ast_base
    {
        number(double v)
            : value(v)
        {
        }

        double value;
    };

    struct identifier : ast_base
    {
        identifier(std::string id)
            : name(std::move(id))
        {
        }

        std::string name;
    };

    struct string : ast_base
    {
        string(std::string s)
            : str(std::move(s))
        {
        }

        std::string str;
    };

    struct function : ast_base
    {
        function(std::vector<node_pointer> args, std::vector<node_pointer> stmts)
            : arguments(std::move(args)), statements(std::move(stmts))
        {
        }

        std::vector<node_pointer> arguments;
        std::vector<node_pointer> statements;
    };

    struct binary : ast_base
    {
        binary(char op, node_pointer l, node_pointer r)
            : op(op), left(std::move(l)), right(std::move(r))
        {
        }

        char op;
        node_pointer left;
        node_pointer right;
    };

    struct call : ast_base
    {
        call(std::string name, std::vector<node_pointer> args)
            : name(std::move(name)), arguments(std::move(args))
        {
        }

        std::string name;   // Function name
        std::vector<node_pointer> arguments;   // Argument list
    };
}

#endif //LLVM_TEST_AST_H
