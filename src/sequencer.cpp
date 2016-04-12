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

Sequencer::Sequencer() : BussedItem(SEQUENCER_TYPE_NAME, SEQUENCER_NAME) {
  _state = FETCH_PC;
};

Sequencer::~Sequencer() {};

void Sequencer::SetupControlConnections() {
  BussedItem::_controlBus->SetInput(&_controlBusValue);
}

void Sequencer::Decode() { }
void Sequencer::Fetch() { }
void Sequencer::Store() { }

void Sequencer::GetNextInstruction() { }

void Sequencer::Initialise() {
  // TODO Initialise correctly, as and when registers available
  BussedItem::_dataBus->SetInput(&_zeroBitsData);
  BussedItem::_addressBus->SetInput(&_zeroBitsAddress);

  _PCAddress |= REG_PC;
}

void Sequencer::Clock() {
  //std::bitset<aN> value;

  _controlBusValue.reset();

  switch (_state) {
    case FETCH_PC:
      log(LOG_TYPE_DEBUG, "Fetch");

      // Set address bus to PC
      //BussedItem<aN, dN, cN>::_addressBus->SetInput(&_PCAddress);
      _controlBusValue.set(CONTROL_READ);
      _controlBusValue.set(CONTROL_WHICH_BUS); // To address bus

      _registerFile->ReadFromRegister(REG_PC);

      _state = FETCH_IST;
      break;
    case FETCH_IST:
      // Store memory at address in CIR
      log(LOG_TYPE_DEBUG, "Fetch Instruction");
      _registerFile->WriteToRegister(REG_CIR);

      _state = EXECUTE;
      break;
    case EXECUTE:
      log(LOG_TYPE_DEBUG, "Execute");
      _state = FETCH_PC;
      break;
  }

  _memory->Clock();
  _registerFile->Clock();
}

void Sequencer::SetRegisterFile(RegisterFile *registerFile) { _registerFile = registerFile; }
void Sequencer::SetMemory(Memory *memory) { _memory = memory; }
