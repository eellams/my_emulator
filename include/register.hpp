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

template<size_t N>
class Register {
public:
  Register(std::string name = REGISTER_DEFAULT_NAME) { SetName(name); }
  ~Register() {}

  void Clock() {}

  void SetValue(int value) {

    _contents = value;
  }

  void SetName(std::string name) {
    _name = name;
  }
  std::string GetName() { return _name; }

  std::bitset<N>* GetValuePointer() { return &_contents; }

private:
  std::string _name;
  std::bitset<N> _contents;

};

#endif
