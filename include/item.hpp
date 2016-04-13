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

#ifndef _ITEM_HPP
#define _ITEM_HPP

#include <string>
#include <sstream>

#include "system.hpp"
#include "singleton.hpp"
#include "logger.hpp"
//#include "myBitset.hpp"

#define ITEM_DEFAULT_NAME "UNKNOWN_ITEM"
#define ITEM_TYPE_DEFAULT_NAME "ITEM"

template<size_t N> class MyBitset;

class Item {
public:
  Item(std::string typeName = ITEM_TYPE_DEFAULT_NAME,
    std::string name = ITEM_DEFAULT_NAME) {
      SetTypeName(typeName);
      SetName(name);
    }

  ~Item() {}

  void SetName(std::string name) { _name = name; }
  std::string GetName() { return _name; }

  void SetTypeName(std::string typeName) { _typeName = typeName; }
  std::string GetTypeName() { return _typeName; }

  void SetInput(MyBitset<BUS_WIDTH> **input) {
    std::ostringstream ss;
    std::string result;

    ss << std::hex << static_cast<void*>(input);
    result = ss.str();

    log(LOG_TYPE_DEBUG, "Setting input to pointer: " + result);
    _input = input;
  }

  MyBitset<BUS_WIDTH>** GetOutputP() { return _output; }

protected:
  std::string createLogPrefix() {
      std::string toReturn;
      toReturn = "[" + GetTypeName() + ": " + GetName() + "] ";
      return toReturn;
  }

  void log(int logType, std::string logStr) {
    Singleton<Logger>::GetInstance()->Log(logType, createLogPrefix() + logStr);
  };

  static std::string createString(long input, bool hex = true) {
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

  static std::string createString(void* input) {
    std::ostringstream ss;
    ss << std::hex << input;
    return ss.str();
  }

  // TODO setInput and setOutput?

  std::string _name;
  std::string _typeName;

  MyBitset<BUS_WIDTH> **_input;
  MyBitset<BUS_WIDTH> **_output;
};

 #endif