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
#include "item.hpp"

#include <string>
#include <sstream>
#include <bitset>

#define MY_BITSET_DEFAULT_NAME "UNKNOWN_BITSET"

template<size_t N>
class MyBitset : public std::bitset<N> {
public:
  MyBitset(Item *parent = 0, std::string name = MY_BITSET_DEFAULT_NAME);

  ~MyBitset();

  // Set and get the name
  void SetName(std::string name);
  std::string GetName();

  // Set and get the parent Item
  void SetParent(Item *parent);
  Item* GetParent();

  // Set the value - either from a long, or another MyBitset
  void SetValue(MyBitset<N> value);
  void SetValue(long value);

  // For compatability with other classes
  std::string GetFullName();

  // Wrappers for std::bitset components
  void set(size_t __position, bool __val = true);
  bool test(size_t __position);
  void reset();
  unsigned long to_ulong();

private:
  // Create the log prefix
  std::string createLogPrefix();

  // Log the item
  void log(int logType, std::string logStr);

  // Cteate a string from an integer
  // TODO create string from another MyBitset?
  std::string createString(long input, bool hex = true);

  // Pointer to the item parent
  Item *_parent;

  // The name of this bitset
  std::string _name;
};

#include "myBitset.tpp"

#endif
