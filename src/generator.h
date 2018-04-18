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
#ifndef LLVM_TEST_GENERATOR_H
#define LLVM_TEST_GENERATOR_H

#include "ast.h"
#include <memory>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>

// TODO: Need to implement scoping of variables

namespace generator
{
    class generator_base
    {
    public:

    protected:
        generator_base()
            : context_(), builder_(context_)
        {
        }

    private:
        llvm::LLVMContext             context_;
        llvm::IRBuilder<>             builder_;
        std::unique_ptr<llvm::Module> module_;
    };

    class generator : public ast::visitor_base
    {
    public:
        generator();

        void visit(ast::assignment const& a) override;
        void visit(ast::function const& f) override;
        void visit(ast::identifier const& i) override;
        void visit(ast::number const& n) override;
        void visit(ast::string const& s) override;
        void visit(ast::binary const&) override;
        void visit(ast::unary const&) override;
        void visit(ast::call const&) override;
        void visit(ast::special_value const&) override;
        void visit(ast::conditional const&) override;
        void visit(ast::class_definition const&) override;
        void visit(ast::while_statement const&) override;
        void visit(ast::block_statement const&) override;
        void visit(ast::for_in_statement const&) override;



    };
}

#endif //LLVM_TEST_GENERATOR_H
