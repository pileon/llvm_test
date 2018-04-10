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
#include <ostream>
#include <iomanip>
#include <iostream>

namespace ast
{
    struct assignment;
    struct function;
    struct identifier;
    struct number;
    struct string;
    struct binary;
    struct unary;
    struct call;
    struct special_value;
    struct conditional;

    struct visitor_base
    {
        virtual void visit(assignment const&) = 0;
        virtual void visit(function const&) = 0;
        virtual void visit(identifier const&) = 0;
        virtual void visit(number const&) = 0;
        virtual void visit(string const&) = 0;
        virtual void visit(binary const&) = 0;
        virtual void visit(unary const&) = 0;
        virtual void visit(call const&) = 0;
        virtual void visit(special_value const&) = 0;
        virtual void visit(conditional const&) = 0;
    };

    struct ast_base
    {
        virtual ~ast_base() = default;

        virtual void accept(visitor_base*) = 0;
        void accept(visitor_base&& visitor)
        {
            accept(&visitor);
        }
        void accept(visitor_base& visitor)
        {
            accept(&visitor);
        }
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

        void accept(visitor_base* visitor) override
        {
            visitor->visit(*this);
        }
    };

    struct number : ast_base
    {
        number(double v)
            : value(v)
        {
        }

        double value;

        void accept(visitor_base* visitor) override
        {
            visitor->visit(*this);
        }
    };

    struct identifier : ast_base
    {
        identifier(std::string id)
            : name(std::move(id))
        {
        }

        std::string name;

        void accept(visitor_base* visitor) override
        {
            visitor->visit(*this);
        }
    };

    struct string : ast_base
    {
        string(std::string s)
            : str(std::move(s))
        {
        }

        std::string str;

        void accept(visitor_base* visitor) override
        {
            visitor->visit(*this);
        }
    };

    struct function : ast_base
    {
        function()
        {
        }

        function(std::vector<node_pointer> args, std::vector<node_pointer> stmts)
            : arguments(std::move(args)), statements(std::move(stmts))
        {
        }

        std::vector<node_pointer> arguments;
        std::vector<node_pointer> statements;

        void accept(visitor_base* visitor) override
        {
            visitor->visit(*this);
        }
    };

    struct binary : ast_base
    {
        binary(int op, node_pointer l, node_pointer r)
            : op(op), left(std::move(l)), right(std::move(r))
        {
        }

        int op;
        node_pointer left;
        node_pointer right;

        void accept(visitor_base* visitor) override
        {
            visitor->visit(*this);
        }
    };

    struct unary : ast_base
    {
        unary(int op, node_pointer e)
            : op(op), expression(std::move(e))
        {
        }

        int op;
        node_pointer expression;

        void accept(visitor_base* visitor) override
        {
            visitor->visit(*this);
        }
    };

    struct special_value : ast_base
    {
        special_value(int op)
            : op(op)
        {
        }

        int op;

        void accept(visitor_base* visitor) override
        {
            visitor->visit(*this);
        }
    };

    struct call : ast_base
    {
        call(node_pointer name, std::vector<node_pointer> args)
            : name(std::move(name)), arguments(std::move(args))
        {
        }

        node_pointer name;   // Function name
        std::vector<node_pointer> arguments;   // Argument list

        void accept(visitor_base* visitor) override
        {
            visitor->visit(*this);
        }
    };

    struct conditional : ast_base
    {
        conditional(node_pointer condition, node_pointer expression, std::vector<node_pointer> elifs, node_pointer els)
            : condition_(std::move(condition)), expression_(std::move(expression)), elifs_(std::move(elifs)), els_(std::move(els))
        {
        }

        node_pointer condition_;
        node_pointer expression_;
        std::vector<node_pointer> elifs_;
        node_pointer els_;

        void accept(visitor_base* visitor) override
        {
            visitor->visit(*this);
        }
    };

    class print_visitor : public visitor_base
    {
    private:
        struct indent
        {
            explicit indent(std::uint32_t indentation)
                : indent_(indentation)
            {
            }

            std::uint32_t indent_;
        };

        friend std::ostream& operator<<(std::ostream& os, indent const& ind);

    public:
        print_visitor(std::ostream& os)
            : output_(os), indent_(0)
        {
        }

        void visit(assignment const& a) override;
        void visit(function const& f) override;
        void visit(identifier const& i) override;
        void visit(number const& n) override;
        void visit(string const& s) override;
        void visit(binary const&) override;
        void visit(unary const&) override;
        void visit(call const&) override;
        void visit(special_value const&) override;
        void visit(conditional const&) override;

    private:
        std::ostream& output_;
        std::uint32_t indent_;

        static std::string op(int oper);
    };

    inline std::ostream& operator<<(std::ostream& os, node_pointer const& node)
    {
        node->accept(ast::print_visitor(std::cout));
    }
}

#endif //LLVM_TEST_AST_H
