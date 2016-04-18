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
#include <utility>

#include "system.hpp"
#include "singleton.hpp"
#include "logger.hpp"

template<size_t N> class MyBitset;


#define ITEM_DEFAULT_NAME "UNKNOWN_ITEM"
#define ITEM_TYPE_DEFAULT_NAME "ITEM"

class Item {
public:
  Item(std::string typeName = ITEM_TYPE_DEFAULT_NAME,
    std::string name = ITEM_DEFAULT_NAME);

  ~Item();

  void SetName(std::string name);

  std::string GetFullName();

  void SetTypeName(std::string typeName);
  std::string GetTypeName();

  virtual void Update();

  virtual void LogSignals();

protected:
  void sendSignals(std::vector<struct Signal> toSend);

  std::string createLogPrefix();

  void log(int logType, std::string logStr);

  static std::string createString(long input, bool hex = true);

  static std::string createString(void* input);

  // TODO setInput and setOutput?

  std::string _name;
  std::string _typeName;
};

 #endif
