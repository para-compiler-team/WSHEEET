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

#include <string>
#include <string_view>
#include <unordered_map>

namespace wsheeet {

class Identifier {
  std::string Id_;

public:
  Identifier(std::string_view Name) : Id_(Name) {}
}; // class Identifier

class IdentifierInfo {
  IDecl *Decl_;
public:
  IdentifierInfo(Scope &S, IType &Ty) : Scope_{&S}, Ty_{&Ty}, ParentId_{nullptr} {}
  IdentifierInfo(Scope &S, IType &Ty, IdentifierInfo &ParentId) : Scope_{&S}, Ty_{&Ty}, ParentId_{&ParentId} {}
}; // class IdentifierInfo

class SymbolTable {
  std::unordered_map<Identifier, std::vector<IdentifierInfo>> Table;
}; // class SymbolTable

} // namespace wsheeet
