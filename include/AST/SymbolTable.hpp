// -------------------------------------------------------------------------- //
// Copyright 2022 Yuly Tarasov
//
// This file is part of WSHEET.
//
// WSHEET is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// WSHEET is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// WSHEET. If not, see <https://www.gnu.org/licenses/>.
// -------------------------------------------------------------------------- //

#pragma once

#include "Decl.hpp"
#include "Type.hpp"

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

namespace wsheeet {

class SymbolTable final {
  std::unordered_map<std::string, std::vector<std::unique_ptr<DeclBase>>> Table_;
}; // class SymbolTable

} // namespace wsheeet
