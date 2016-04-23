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

  _zeroFlag = true;
}

ALU::~ALU() {};

void ALU::Clock() {
  log(LOG_TYPE_DEBUG, "Clock");

  // Used as local sotoring for values on the bus
  unsigned long dataBusValue = 0;
  unsigned long imm = 0;
  unsigned long shortImm = 0;
  long toAdd = 0;

  MyBitset<CONTROL_BUS_WDTH> controlValue;
  MyBitset<BUS_WIDTH> temp; // Used in calculating a negative number

  // Decode, so we can remove the databus value
  dataBusValue = _dataBusP->GetValueP()->to_ulong();
  imm = dataBusValue & BITMASK_IMM;
  shortImm = (dataBusValue & BITMASK_IMM_SHORT) >> (BITMASK_REG_B_WIDTH);

  controlValue = *(_controlBusP->GetValueP());

  // Reset the accumulator before we do anything else
  if (controlValue.test(CONTROL_BUS_ALU_RESET_ACC)) {
    log(LOG_TYPE_INFO, "Reseting " + _ACC.GetFullName() + " to zeros");
    _ACC.reset();
  }

  if (controlValue.test(CONTROL_BUS_ALU_IMM)) {
    log(LOG_TYPE_DEBUG, "Immediate initial value: " + createString(imm));
    if (controlValue.test(CONTROL_BUS_ALU_SIGNED)) {
      // Signed IMM
      log(LOG_TYPE_DEBUG, "Signed Immediate");
      if (imm & (1 << (BITMASK_IMM_WIDTH - 1))) {
        // A negative number
        log(LOG_TYPE_DEBUG, "Negative signed Immediate");
        toAdd = imm - (1 << BITMASK_IMM_WIDTH);
      }
      else toAdd = imm;
    }
    else toAdd = imm;
    log(LOG_TYPE_DEBUG, "Immediate value: " + createString(toAdd));
  }

  else if (controlValue.test(CONTROL_BUS_ALU_IMM_SHORT)) {
    // Short IMM
    log(LOG_TYPE_DEBUG, "Short immediate");
    log(LOG_TYPE_DEBUG, "Initial short immediate value: " + createString(shortImm));

    if (controlValue.test(CONTROL_BUS_ALU_SIGNED)) {
      // Signed short IMM
      log(LOG_TYPE_DEBUG, "Signed short immediate");

      if (shortImm & (1 << (BITMASK_IMM_SHORT_WIDTH - 1))) {
        // Negative signed short imm
        log(LOG_TYPE_DEBUG, "Negative signed short immediate");
        toAdd = shortImm - (1 << BITMASK_IMM_SHORT_WIDTH);
      }
      else toAdd = shortImm;
    }
    else toAdd = shortImm;

    log (LOG_TYPE_DEBUG, "Short immediate value: " + createString(toAdd) + " [" + createString(toAdd, false) + "]");
  }

  else {
    log(LOG_TYPE_DEBUG, "A full-bus value initial value: " + createString(dataBusValue));

    if(controlValue.test(CONTROL_BUS_ALU_SIGNED)) {
      log(LOG_TYPE_DEBUG, "Signed value");

      if (dataBusValue & (1 << (BUS_WIDTH - 1))) {
        log(LOG_TYPE_DEBUG, "Negative value");
        toAdd = dataBusValue - (1 << BUS_WIDTH);
      }
      else toAdd = dataBusValue;
    }
    else toAdd = dataBusValue;
    log(LOG_TYPE_DEBUG, "Full bus value: " + createString(toAdd) + " [" + createString(toAdd, false) + "]");
  }


  if (controlValue.test(CONTROL_BUS_ALU_ADD)) {
    log(LOG_TYPE_INFO, "Adding value: " + createString(toAdd) + " to ACC: " + createString(_ACC.to_ulong()));
    _ACC.SetValue(_ACC.to_ulong() + toAdd);
    _dataBusP->SetValueP(&_ACC);
  }

  else if (controlValue.test(CONTROL_BUS_ALU_NAND)) {
    log(LOG_TYPE_INFO, "Nanding value: " + createString(toAdd) + " to ACC: " + createString(_ACC.to_ulong()));
    _ACC.SetValue( ~(toAdd & _ACC.to_ulong()) );
    log(LOG_TYPE_INFO, "Producing: " + createString(_ACC.to_ulong()));
    _dataBusP->SetValueP(&_ACC);
  }

  if (_ACC.to_ulong() == 0) {
    log(LOG_TYPE_INFO, "Zero flag set");
    _zeroFlag = true;
  }
  else if (_zeroFlag) {
    log(LOG_TYPE_DEBUG, "Unsetting zero flag");
    _zeroFlag = false;
  }
}

// Update this and children
void ALU::Update() {
  log(LOG_TYPE_UPDATE, "Update [EMPTY]");
};

// Log signals
void ALU::LogSignals() {
  std::vector<struct Signal> toSend;
  struct Signal toAdd;

  toAdd.Name = GetFullName() + _ACC.GetName();
  toAdd.Value = _ACC.to_ulong();
  toAdd.Address = static_cast<void*>(&_ACC);

  toSend.push_back(toAdd);
  sendSignals(toSend);
}
