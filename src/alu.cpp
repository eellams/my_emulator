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

void ALU::Clock() {
  log(LOG_TYPE_DEBUG, "Clock");

  // Used as local sotoring for values on the bus
  unsigned long dataBusValue;
  unsigned long imm;

  MyBitset<CONTROL_BUS_WDTH> controlValue;
  MyBitset<BUS_WIDTH> temp; // Used in calculating a negative number

  // Decode, so we can remove the databus value
  dataBusValue = _dataBusP->GetValueP()->to_ulong();
  imm = dataBusValue & BITMASK_IMM;

  controlValue = *(_controlBusP->GetValueP());

  // Reset the accumulator before we do anything else
  if (controlValue.test(CONTROL_BUS_ALU_RESET_ACC)) {
    log(LOG_TYPE_INFO, "Reseting " + _ACC.GetFullName() + " to zeros");
    _ACC.reset();
  }

  // If the add flag is present, add the data to the accumulator
  //  (not including the op-code - see above)
  if (controlValue.test(CONTROL_BUS_ALU_ADD)) {
    // Add to ACC
    log(LOG_TYPE_INFO, "Adding value: " + createString(imm) + " to Accumulator, value: " + createString(_ACC.to_ulong()) );

    // If the signed flag is specified
    //  convert the a negative
    if (controlValue.test(CONTROL_BUS_ALU_SIGNED)) {
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
      // Not signed - just add the number to the ACC
      temp.SetValue(imm + _ACC.to_ulong());
    }

    _ACC.SetValue(temp.to_ulong());
    _dataBusP->SetValueP(&_ACC);
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
