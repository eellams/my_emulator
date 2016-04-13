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
//#include "bussedItem.hpp"
#include "item.hpp"

#include <string>
#include <sstream>
#include <bitset>

#define MY_BITSET_DEFAULT_NAME "UNKNOWN_BITSET"

template<size_t N>
class MyBitset : std::bitset<N> {
public:
  MyBitset(Item *parent = 0, std::string name = MY_BITSET_DEFAULT_NAME);

  ~MyBitset();

  void SetName(std::string name);
  std::string GetName();

  void SetParent(Item *parent);
  Item* GetParent();

  std::string GetDetails();

  void set(size_t __position, bool __val = true);
  bool test(size_t __position);
  void reset();
  unsigned long to_ulong();
  void operator|=(const std::bitset<N>& __rhs) _GLIBCXX_NOEXCEPT;

private:
  std::string createLogPrefix();

  void log(int logType, std::string logStr);

  std::string createString(long input, bool hex = true);

  Item *_parent;

  std::string _name;
  std::string _parentType;
  std::string _parentName;
};

#include "myBitset.tpp"

#endif
