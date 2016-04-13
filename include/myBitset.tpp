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

#include "myBitset.hpp"

template<size_t N>
MyBitset<N>::MyBitset(Item *parent, std::string name) : std::bitset<N>() {
  SetParent(parent);
  SetName(name);
 }

template<size_t N>
MyBitset<N>::~MyBitset() {};

template<size_t N>
void MyBitset<N>::SetName(std::string name) { _name = name; }

template<size_t N>
std::string MyBitset<N>::GetName() { return _name; }

template<size_t N>
void MyBitset<N>::SetParent(Item *parent) { _parent = parent; }

template<size_t N>
Item* MyBitset<N>::GetParent() { return _parent; }

template<size_t N>
std::string MyBitset<N>::GetDetails() {
  std::string toReturn;
  toReturn += createLogPrefix();
  return toReturn;
}

template<size_t N>
void MyBitset<N>::set(size_t __position, bool __val) {
  log(LOG_TYPE_DEBUG, "Setting bit: " + createString(__position) + " to: " + createString((long)__val) );
  std::bitset<N>::set(__position, __val);
}

template<size_t N>
bool MyBitset<N>::test(size_t __position) {
  bool toReturn = std::bitset<N>::test(__position);
  log(LOG_TYPE_DEBUG, "Testting bit: " + createString((long)  __position) + " (" + createString((long)toReturn) + ")" );
  return toReturn;
}

template<size_t N>
void MyBitset<N>::reset() {
  log(LOG_TYPE_DEBUG, "Resetting all bits to 0");
  std::bitset<N>::reset();
}

template<size_t N>
unsigned long MyBitset<N>::to_ulong() {
  log(LOG_TYPE_DEBUG, "Returning value as long: " + createString(std::bitset<N>::to_ulong()) );
  return std::bitset<N>::to_ulong();
}

template<size_t N>
void MyBitset<N>::operator|=(const std::bitset<N>& __rhs) _GLIBCXX_NOEXCEPT { std::bitset<N>::operator|=(__rhs); }

template<size_t N>
std::string MyBitset<N>::createLogPrefix() {
  std::string toReturn;
  if (_parent) {
    toReturn = "[" + _parent->GetTypeName() + ": " + _parent->GetName() + "] " + \
      "[MYBITSET: " + GetName() + "] ";
  } else {
    toReturn = std::string("[NO_PARENT]") + "[MYBITSET: " + GetName() + "] ";
  }

  return toReturn;
}

template<size_t N>
void MyBitset<N>::log(int logType, std::string logStr) {
  // TODO
  //Singleton<Logger>::GetInstance()->Log(logType, createLogPrefix() + logStr);
}

template<size_t N>
std::string MyBitset<N>::createString(long input, bool hex) {
  std::ostringstream ss;
  if (hex) {
    ss << std::hex << input;
    return "0x" + ss.str();
  }
  else {
    ss << input;
    return ss.str();
  }
}
