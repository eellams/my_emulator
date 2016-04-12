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

#ifndef _BUSSED_ITEM_HPP
#define _BUSSED_ITEM_HPP

#include <string>
#include <sstream>

#include "system.hpp"
#include "bus.hpp"

#define BUSSED_ITEM_DEFAULT_NAME "UNKNOWN_ITEM"
#define BUSSED_ITEM_DEFAULT_TYPE_NAME "UNKNOWN_TYPE"

// Forward definiton, avoids cyclic dependencies with "myBitset.hpp"
template<size_t N> class MyBitset;

class BussedItem {
public:
  BussedItem(std::string typeName = BUSSED_ITEM_DEFAULT_TYPE_NAME,
    std::string name = BUSSED_ITEM_DEFAULT_NAME);
  ~BussedItem();

  void SetDataBus(Bus<DATA_WIDTH> *bus);
  void SetAddressBus(Bus<ADDRESS_WIDTH> *bus);
  void SetControlBus(Bus<CONTROL_WIDTH> *bus);

  void SetName(std::string name);
  std::string GetName();

  void SetTypeName(std::string typeName);
  std::string GetTypeName();

  void SetInput(MyBitset<DATA_WIDTH> *input) { _input = input; }
  MyBitset<DATA_WIDTH>* GetOutputP() { return _output; }

protected:
  std::string createLogPrefix();
  void log(int logType, std::string logStr);

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

  // TODO setInput and setOutput?

  std::string _name;
  std::string _typeName;

  MyBitset<DATA_WIDTH> *_input;
  MyBitset<DATA_WIDTH> *_output;

  Bus<ADDRESS_WIDTH> *_addressBus;
  Bus<DATA_WIDTH> *_dataBus;
  Bus<CONTROL_WIDTH> *_controlBus;
};

 #endif
