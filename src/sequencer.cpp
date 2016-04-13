/*    A simple emulator
 *    Copyright (C) 2016  Eddie Ellams
 *
 *    This file is part of my_emulator.
 *
 *    My_emulator is free software: you can _controlBusValueredistribute it and/or modify
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
  _controlBusValue.SetParent(this);
  _controlBusValue.SetName("Control Bus Value");
};

Sequencer::~Sequencer() {};

void Sequencer::SetupControlConnections() {
  BussedItem::_controlBus->SetInput(&_controlBusValue);
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

      _state = FETCH_INSTRUCTION;
      break;
    case FETCH_INSTRUCTION:
      // Store memory at address in CIR
      log(LOG_TYPE_INFO, "Fetch Instruction");

      // Set address bus to PC
      //BussedItem<aN, dN, cN>::_addressBus->SetInput(&_PCAddress);
      _controlBusValue.set(CONTROL_READ);
      _controlBusValue.set(CONTROL_WHICH_BUS, 0); // To data bus

      _registerFile->WriteToRegister(REG_CIR);

      _state = STORE_INSTRUCTION;
      break;

    case STORE_INSTRUCTION:
      log(LOG_TYPE_INFO, "Store Instruction");

      _registerFile->WriteToRegister(REG_CIR);

      _state = FINISHED;
      break;

    case EXECUTE:
      log(LOG_TYPE_INFO, "Execute");
      _state = FETCH_PC;
      break;

    default:
      break;
  }

  _memory->Clock();
  _registerFile->Clock();

  Signals();
}

void Sequencer::Signals() {
  std::vector< std::pair<std::string, MyBitset<BUS_WIDTH>* > > signals;
  std::vector< std::pair<std::string, MyBitset<BUS_WIDTH>* > > signalsAddress = _addressBus->GetSignals();
  std::vector< std::pair<std::string, MyBitset<BUS_WIDTH>* > > signalsData = _dataBus->GetSignals();
  std::vector< std::pair<std::string, MyBitset<BUS_WIDTH>* > > signalsControl = _controlBus->GetSignals();

  while(signalsAddress.size()) { signals.push_back(signalsAddress.back()); signalsAddress.pop_back(); }
  while(signalsData.size()) { signals.push_back(signalsData.back()); signalsData.pop_back(); }
  while(signalsControl.size()) { signals.push_back(signalsControl.back()); signalsControl.pop_back(); }

  for (int i=0; i<signals.size(); i++) {
    if (signals[i].second) {
      Singleton<Logger>::GetInstance()->AddLogSignal(signals[i].first, signals[i].second->GetName(), signals[i].second->to_ulong());
    }
    else {
      Singleton<Logger>::GetInstance()->AddLogSignal(signals[i].first, "NOT SET", 0);
    }
  }
}

void Sequencer::SetRegisterFile(RegisterFile *registerFile) { _registerFile = registerFile; }
void Sequencer::SetMemory(Memory *memory) { _memory = memory; }
