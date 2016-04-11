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

#ifndef _BUS_HPP
#define _BUS_HPP

#include <bitset>
#include <string>
#include <sstream>
#include <stdexcept>

#include "system.hpp"
#include "singleton.hpp"
#include "logger.hpp"
#include "register.hpp"

#define BUS_DEFAULT_NAME "UNKNOWN_BUS"
#define BUS_PREFIX "BUS"

template<size_t N>
class Bus {
public:
  Bus(std::string name = BUS_DEFAULT_NAME) {
    SetName(name);
  }

  ~Bus() {};

  void SetName(std::string name) { _name = name; }
  std::string GetName() { return _name; }

  void SetInput(std::bitset<N> *input) {
    LOG(LOG_TYPE_DEBUG, createLogPrefix() + "setting input to: " + CreateString(input->to_ulong()));
    _input = input;
  }

  std::bitset<N> GetValue() {
    LOG(LOG_TYPE_DEBUG, createLogPrefix() + "getting value: " + CreateString(_input->to_ulong()));
    return std::bitset<N>(*_input);
  }

private:
  std::string createLogPrefix() {
      std::string toReturn;
      toReturn = "[" + std::string(BUS_PREFIX) + ":\"" + _name + "\"] ";
      return toReturn;
  }

  std::string _name;
  std::bitset<N> *_input;
};

 #endif
