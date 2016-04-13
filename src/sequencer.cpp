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

#include "sequencer.hpp"

Sequencer::Sequencer(std::string name) : BussedItem(SEQUENCER_TYPE_NAME, name) {
  _state = FETCH_PC;

  _zeroBitsData.SetParent(this);
  _zeroBitsData.SetName("_zeroBitsData");
  _zeroBitsAddress.SetParent(this);
  _zeroBitsAddress.SetName("_zeroBitsAddress");
  _PCAddress.SetParent(this);
  _PCAddress.SetName("_PCAddress");
  _controlBusValue.SetParent(this);
  _controlBusValue.SetName("_controlBusValue");

  _zeroBitsDataP = &_zeroBitsData;
  _zeroBitsAddressP = &_zeroBitsAddress;
  _PCAddressP = &_PCAddress;
  _controlBusValueP = &_controlBusValue;
};

Sequencer::~Sequencer() {};

void Sequencer::SetupControlConnections() {
  BussedItem::_controlBus->SetInput(&_controlBusValueP);
}

void Sequencer::Decode() { }
void Sequencer::Fetch() { }
void Sequencer::Store() { }

void Sequencer::GetNextInstruction() { }

void Sequencer::Initialise() {
  // TODO Initialise correctly, as and when registers available
  BussedItem::_dataBus->SetInput(&_zeroBitsDataP);
  BussedItem::_addressBus->SetInput(&_zeroBitsAddressP);

  _PCAddress |= REG_PC;
}

void Sequencer::Clock() {
  _controlBusValue.reset();

  switch (_state) {
    case FETCH_PC:
      log(LOG_TYPE_INFO, "Fetch");

      // Set address bus to PC
      //BussedItem<aN, dN, cN>::_addressBus->SetInput(&_PCAddress);
      _controlBusValue.set(CONTROL_READ);
      _controlBusValue.set(CONTROL_WHICH_BUS); // To address bus

      _registerFile->ReadFromRegister(REG_PC);

      _state = FETCH_IST;
      break;
    case FETCH_IST:
      // Store memory at address in CIR
      log(LOG_TYPE_INFO, "Fetch Instruction");
      _registerFile->WriteToRegister(REG_CIR);

      _state = EXECUTE;
      break;
    case EXECUTE:
      log(LOG_TYPE_INFO, "Execute");
      _state = FETCH_PC;
      break;
  }

  _memory->Clock();
  _registerFile->Clock();
}

void Sequencer::SetRegisterFile(RegisterFile *registerFile) { _registerFile = registerFile; }
void Sequencer::SetMemory(Memory *memory) { _memory = memory; }
