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

namespace ast
{
    struct ast_base
    {

    };

    struct assignment : ast_base
    {
        ast_base* left;
        ast_base* right;
    };

    struct number : ast_base
    {
        double value;
    };

    struct identifier : ast_base
    {
        std::string name;
    };

    struct string : ast_base
    {
        std::string string;
    };
}

#endif //LLVM_TEST_AST_H
