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
#include "register.hpp"
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

  void Signed() {
    _signed = true;
  }

  void Clock() {
    log(LOG_TYPE_DEBUG, "Clock");

    unsigned long data;
    unsigned long imm;
    //unsigned long temp;
    MyBitset<BUS_WIDTH> temp;

    data = _dataBusP->GetValueP()->to_ulong();
    imm = data & BITMASK_IMM;


    if (_add) {
      log(LOG_TYPE_INFO, "Adding value: " + createString(imm) + " to Accumulator, value: " + createString(_ACCP->GetOutputP()->to_ulong()) ) ; //createString(_dataBusP->GetValueP()->to_ulong()));

      if (_signed) {
        log(LOG_TYPE_ERROR, "Signed flag provided, but not implemented" );

        temp.SetValue(imm);
        if (temp.test(BITMASK_IMM_WIDTH - 1)) {
          long negative = (long)((long)temp.to_ulong() - (1 << BITMASK_IMM_WIDTH));

          log(LOG_TYPE_DEBUG, "Number is negative, instead adding value: " + createString(negative) + " [" + createString(negative, false) + "]" );

          temp.SetValue( (long)((long)_ACCP->GetOutputP()->to_ulong() + negative) );
        }
        else {
          log(LOG_TYPE_DEBUG, "Number is positive");
          temp.SetValue(imm + _ACCP->GetOutputP()->to_ulong());
        }
      }
      else {
        temp.SetValue(imm + _ACCP->GetOutputP()->to_ulong());
      }

      _output.SetValue(temp.to_ulong());
      _dataBusP->SetValueP(&_output);

      _dataBusP->SetValueP(&_output);
      _add = false;
      _signed = false;
    }
  }

  // This class has no children, and has no memory
  //  so has nothing to update
  //  Update therefore is seldom called
  void Update() {
    log(LOG_TYPE_UPDATE, "Update [EMPTY]");
  };

  // Nothing to log?
  void LogSignals() {
    std::vector<struct Signal> toSend;
    struct Signal toAdd;

    toAdd.Name = createLogPrefix() + _output.GetName();
    toAdd.Value = _output.to_ulong();
    toAdd.Address = static_cast<void*>(&_output);

    toSend.push_back(toAdd);
    sendSignals(toSend);
  }

  void SetRegisterFileP(RegisterFile *value) {
    _registerFileP = value;
    _ACCP = value->GetACCP();
  }

private:
  RegisterFile *_registerFileP;
  Register<REGISTER_WIDTH>* _ACCP;

  MyBitset<BUS_WIDTH> _output;
  bool _add, _signed;
};
 #endif
