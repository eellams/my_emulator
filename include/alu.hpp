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

#ifndef _ALU_HPP
#define _ALU_HPP

#include "bussedItem.hpp"
#include "registerFile.hpp"
#include "myBitset.hpp"

#define ALU_NAME "ALU"
#define ALU_TYPE_NAME "ALU"

class ALU : public BussedItem {
public:
  ALU(std::string name = ALU_NAME) : BussedItem(ALU_TYPE_NAME, ALU_NAME) {
    _output.SetParent(this);
    _output.SetName("Output");
  }
  ~ALU() {};

  // Adds data bus to accumulator
  void Add() {
    _add = true;
  }

  void Clock() {
    log(LOG_TYPE_DEBUG, "Clock");
    Update();

    long data;
    long imm;
    long temp;

    data = _dataBusP->GetValueP()->to_ulong();
    imm = data & BITS_IMM;


    if (_add) {
      log(LOG_TYPE_INFO, "Adding value: " + createString(imm));
      temp = _output.to_ulong() + imm;

      _output ^= _output;
      _output |= temp;

      _dataBusP->SetValueP(&_output);
      _add = false;
    }

    Update();
  }

  void SetRegisterFileP(RegisterFile *value) { _registerFileP = value; }

private:
  RegisterFile *_registerFileP;

  MyBitset<BUS_WIDTH> _output;
  bool _add;
};
 #endif
