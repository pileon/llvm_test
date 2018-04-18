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

#include "generator.h"
#include <llvm/IR/Constants.h>

namespace generator
{
    generator::generator()
    {
    }

    void generator::visit(ast::assignment const& a)
    {

    }

    void generator::visit(ast::function const& f)
    {

    }

    void generator::visit(ast::identifier const& i)
    {

    }

    void generator::visit(ast::number const& n)
    {
        //auto value = llvm::ConstantFP::get(context_, llvm::APFloat(n.value));
    }

    void generator::visit(ast::string const& s)
    {
    }

    void generator::visit(ast::binary const&)
    {

    }

    void generator::visit(ast::unary const&)
    {

    }

    void generator::visit(ast::call const&)
    {

    }

    void generator::visit(ast::special_value const&)
    {

    }

    void generator::visit(ast::conditional const&)
    {

    }

    void generator::visit(ast::class_definition const&)
    {

    }

    void generator::visit(ast::while_statement const&)
    {

    }

    void generator::visit(ast::block_statement const&)
    {

    }

    void generator::visit(ast::for_in_statement const&)
    {

    }
}
