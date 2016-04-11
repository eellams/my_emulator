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

#include "system.hpp"
#include "singleton.hpp"
#include "logger.hpp"

#define BUS_DEFAULT_NAME "UNKNOWN_BUS"

template<size_t N>
class Bus {
public:
  Bus() {}
  ~Bus() {};

  void SetName(std::string name) { _name = name; }
  std::string GetName() { return _name; }

  void Set(std::bitset<N> *value) {
    std::ostringstream valueSS;
    valueSS << (int)value->data;

    Singleton<Logger>::GetInstance()->Log(LOG_TYPE_DEBUG, "Setting value of bus '" + _name + "' (" + valueSS.str() + ")");
    _contents = value;
  };

  std::bitset<N>* Get() {
    std::ostringstream contentsSS;
    contentsSS << (int)_contents->data;

    Singleton<Logger>::GetInstance()->Log(LOG_TYPE_DEBUG, "Getting value of bus '" + _name + "' (" + contentsSS.str() + ")");
    return _contents;
  };

private:
  std::string _name;
  std::bitset<N> *_contents;
};

 #endif
