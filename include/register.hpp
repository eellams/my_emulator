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

#ifndef _REGISTER_HPP
#define _REGISTER_HPP

#include "system.hpp"
#include "singleton.hpp"
#include "logger.hpp"
#include <string>

#define REGISTER_DEFAULT_NAME "UNKNOWN_REGISTER"
#define REGISTER_PREFIX "REGISTER"

template<size_t N>
class Register {
public:
  Register(std::string name = REGISTER_DEFAULT_NAME) { SetName(name); }

  ~Register() {  }

  // Clock in the input (if enabled)
  void Clock() {
    if (*_writeEnable) {
      _contents = *_input;
      LOG(LOG_TYPE_DEBUG, createLogPrefix() + "clocking in value: " + CreateString(_input->to_ulong()));
    }
  }

  // Set the register's name (for logging)
  void SetName(std::string name) {
    if(name != REGISTER_DEFAULT_NAME && name != "")
      LOG(LOG_TYPE_DEBUG, createLogPrefix() + "renaming register to '" + name + "'");
    _name = name;
  }

  std::string GetName() { return _name; }

  void SetWriteEnable(bool *value) {
    // TODO do we need to put a log here?
    _writeEnable = value;
  }

  void SetInput(std::bitset<N> *value) {
    // TODO do we need a log here?
    _input = value;
  }

  std::bitset<N> GetContents() {
    LOG(LOG_TYPE_DEBUG, createLogPrefix() + "getting contents: " + CreateString(_contents.to_ulong()));
    return _contents;
  }

private:
  std::string createLogPrefix() {
      std::string toReturn;
      toReturn = "[" + std::string(REGISTER_PREFIX) + ":\"" + _name + "\"] ";
      return toReturn;
  }

  std::string _name;
  std::bitset<N> _contents;
  std::bitset<N> *_input;

  bool *_writeEnable;

};

#endif
