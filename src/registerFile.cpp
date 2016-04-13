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

#include "registerFile.hpp"

RegisterFile::RegisterFile(std::string name) : BussedItem(REG_FILE_TYPE_NAME, name) {}

RegisterFile::~RegisterFile() {}


void RegisterFile::SetupRegisters() {
  std::string name;

  log(LOG_TYPE_DEBUG, "Setting up registers");

  // Setup general registers
  for (int i=0; i<REGISTER_NUMBER; i++) {
    if (i == REG_ZERO) name = std::string(REG_ZERO_NAME);
    else if (i == REG_ACC) name = std::string(REG_ACC_NAME);
    else if (i == REG_MAR) name = std::string(REG_MAR_NAME);
    else if (i == REG_MDR) name = std::string(REG_MDR_NAME);
    else if (i == REG_PC) name = std::string(REG_PC_NAME);
    else if (i == REG_CIR) name = std::string(REG_CIR_NAME);
    else name = std::string(REG_GEN_NAME) + BussedItem::createString(i, false);

    _reg[i].SetName(name);

    _reg[i].SetWriteEnable(&_registerEnables[i]);
    _registerEnables[i] = false;
    _reg[i].SetInput( *_dataBus->GetValue() );
  }
}

void RegisterFile::Clock() {
  log(LOG_TYPE_DEBUG, "Register file clock");

  // Clock all the registers
  for (int i=0; i<REGISTER_NUMBER; i++) {
    _reg[i].Clock();
    _registerEnables[i] = false;
  }
}

void RegisterFile::ReadFromRegister(int registerNumber) {
  MyBitset<BUS_WIDTH> **control;
  control = BussedItem::_controlBus->GetValue();

  if ((*control)->test(CONTROL_WHICH_BUS)) {
    // Address bus
    log(LOG_TYPE_INFO, "Reading to address bus: " + createString(_reg[registerNumber].GetContentsP()->to_ulong() ));
    _addressBus->SetInput( _reg[registerNumber].GetContentsP() );
  }
  else {
    // Data bus
    log(LOG_TYPE_INFO, "Reading to address bus: " + createString(_reg[registerNumber].GetContentsP()->to_ulong() ));
    _dataBus->SetInput( _reg[registerNumber].GetContentsP() );
  }
}

void RegisterFile::WriteToRegister(int registerNumber) {
  _registerEnables[registerNumber] = true;
}
