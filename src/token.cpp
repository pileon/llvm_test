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

#include <unordered_map>
#include "token.h"

namespace lexer
{
    std::string token::to_string() const
    {
        static std::unordered_map<token_type, std::string> const token_to_string = {
            { t_error       , "t_error" },
            { t_eof         , "t_eof" },
            { t_number      , "t_number" },
            { t_identifier  , "t_identifier" }
        };

        if (auto const& i = token_to_string.find(token_); i != end(token_to_string))
        {
            return i->second;
        }

        return std::string(1, static_cast<char>(token_));
    }
}
