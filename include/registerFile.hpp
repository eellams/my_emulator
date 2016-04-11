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

#ifndef _REGISTER_FILE_HPP
#define _REGISTER_FILE_HPP

#include "system.hpp"
#include "singleton.hpp"
#include "logger.hpp"
#include "bussedItem.hpp"

#define REG_NUM 1 << OPERAND_BITS

template<size_t aN, size_t dN, size_t cN>
class RegisterFile : BussedItem<aN, dN, cN> {
public:
  RegisterFile() {
    for (int i=0; i<REG_NUM; i++) {
      _generalReg[i].SetWriteEnable(&_registerEnables[i]);
      // TODO name the registers
    }
  }

  ~RegisterFile() {}

  void Clock() {}

private:
  Register<dN> _generalReg[REG_NUM];
  bool _registerEnables[REG_NUM];

  //std::bitset<dN> _input;
  Register<dN> *_output;
};

#endif
