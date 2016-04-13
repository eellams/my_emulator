/*    A simple emulator
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

#include "item.hpp"

Item::Item(std::string typeName, std::string name) {
    SetTypeName(typeName);
    SetName(name);
  }

Item::~Item() {}

void Item::SetName(std::string name) { _name = name; }
std::string Item::GetName() { return _name; }

void Item::SetTypeName(std::string typeName) { _typeName = typeName; }
std::string Item::GetTypeName() { return _typeName; }

void Item::SetInput(MyBitset<BUS_WIDTH> *input) {
  std::ostringstream ss;
  std::string result;

  ss << std::hex << static_cast<void*>(input);
  result = ss.str();

  log(LOG_TYPE_DEBUG, "Setting input to pointer: " + result);
  _input = input;
}

MyBitset<BUS_WIDTH>** Item::GetOutputP() { return &_output; }

std::string Item::createLogPrefix() {
    std::string toReturn;
    toReturn = "[" + GetTypeName() + ": " + GetName() + "] ";
    return toReturn;
}

void Item::log(int logType, std::string logStr) {
  Singleton<Logger>::GetInstance()->Log(logType, createLogPrefix() + logStr);
};

std::string Item::createString(long input, bool hex) {
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

std::string Item::createString(void* input) {
  std::ostringstream ss;
  ss << std::hex << input;
  return ss.str();
}
