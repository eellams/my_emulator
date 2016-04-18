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

#include "alu.hpp"

ALU::ALU(std::string name) : BussedItem(ALU_TYPE_NAME, ALU_NAME) {
  _ACC.SetParent(this);
  _ACC.SetName("Accumulator");
}

ALU::~ALU() {};

// Adds data bus to accumulator
void ALU::Add() {
  _add = true;
}

void ALU::Signed() {
  _signed = true;
}

void ALU::Clock() {
  log(LOG_TYPE_DEBUG, "Clock");

  unsigned long data;
  unsigned long imm;
  //unsigned long temp;
  MyBitset<BUS_WIDTH> temp;

  data = _dataBusP->GetValueP()->to_ulong();
  imm = data & BITMASK_IMM;


  if (_add) {
    log(LOG_TYPE_INFO, "Adding value: " + createString(imm) + " to Accumulator, value: " + createString(_ACC.to_ulong()) ) ; //createString(_dataBusP->GetValueP()->to_ulong()));

    if (_signed) {
      log(LOG_TYPE_ERROR, "Signed flag" );

      temp.SetValue(imm);
      if (temp.test(BITMASK_IMM_WIDTH - 1)) {
        long negative = (long)((long)temp.to_ulong() - (1 << BITMASK_IMM_WIDTH));

        log(LOG_TYPE_DEBUG, "Number is negative, instead adding value: " + createString(negative) + " [" + createString(negative, false) + "]" );

        temp.SetValue( (long)((long)_ACC.to_ulong() + negative) );
      }
      else {
        log(LOG_TYPE_DEBUG, "Number is positive");
        temp.SetValue(imm + _ACC.to_ulong());
      }
    }
    else {
      temp.SetValue(imm + _ACC.to_ulong());
    }

    _ACC.SetValue(temp.to_ulong());
    _dataBusP->SetValueP(&_ACC);

    _dataBusP->SetValueP(&_ACC);
    _add = false;
    _signed = false;
  }
}

// This class has no children, and has no memory
//  so has nothing to update
//  Update therefore is seldom called
void ALU::Update() {
  log(LOG_TYPE_UPDATE, "Update [EMPTY]");
};

// Nothing to log?
void ALU::LogSignals() {
  std::vector<struct Signal> toSend;
  struct Signal toAdd;

  toAdd.Name = GetFullName() + _ACC.GetName();
  toAdd.Value = _ACC.to_ulong();
  toAdd.Address = static_cast<void*>(&_ACC);

  toSend.push_back(toAdd);
  sendSignals(toSend);
}

void ALU::SetRegisterFileP(RegisterFile *value) {
  _registerFileP = value;
}

void ALU::ResetACC() {
  _ACC.reset();
}

MyBitset<BUS_WIDTH>* ALU::GetACCP() {
  return &_ACC;
}
