/*    Tests that need to be performed
 *    Copyright (C) 2016  Eddie Ellams
 *
 *    This file is part of my_emulator.
 *
 *    My_emulator is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    My_emulator is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with my_emulator.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _MY_BITSET
#define _MY_BITSET

#include "system.hpp"
#include "singleton.hpp"
#include "logger.hpp"
#include "bussedItem.hpp"

#include <string>
#include <sstream>
#include <bitset>

#define MY_BITSET_DEFAULT_NAME "UNKNOWN_BITSET"

class BussedItem;

template<size_t N>
class MyBitset : public std::bitset<N> {
public:
  MyBitset(BussedItem *parent = 0, std::string name = MY_BITSET_DEFAULT_NAME) : std::bitset<N>() {
    SetParent(parent);
    SetName(name);
   }
  ~MyBitset() {};

  void SetName(std::string name) { _name = name; }
  std::string GetName() { return _name; }

  void SetParent(BussedItem *parent) { _parent = parent; }

private:
  std::string createLogPrefix() {
    std::string toReturn;
    toReturn = "[" + _parent->GetTypeName() + ": " + _parent->GetName() + "] " + \
      "[bitset: " + GetName() + "] ";

    return toReturn;
  }

  BussedItem *_parent;

  std::string _name;
  std::string _parentType;
  std::string _parentName;
};

#endif
