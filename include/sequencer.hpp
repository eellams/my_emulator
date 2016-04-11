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

#ifndef _SEQUENCER_HPP
#define _SEQUENCER_HPP

#include "system.hpp"
#include "singleton.hpp"
#include "logger.hpp"
#include "bussedItem.hpp"
#include "register.hpp"

#define REG_CONTROL_NAME "Sequencer Control Register"
#define REG_CIR_NAME "Current Instruction Register"
#define REG_MAR_NAME "Memory Address Register"
#define REG_PC_NAME "Program Counter"
#define REG_MDR_NAME "Memory Data Register"

enum States{
  FETCH,
  EXECUTE
};

template<size_t aN, size_t dN, size_t cN>
class Sequencer : public BussedItem<aN, dN, cN> {
public:
  Sequencer() : BussedItem<aN, dN, cN>() {
    _controlReg.SetName(REG_CONTROL_NAME);

    _PC.SetName(REG_PC_NAME);
    _CIR.SetName(REG_CIR_NAME);
    _MAR.SetName(REG_MAR_NAME);
    _MDR.SetName(REG_MDR_NAME);

    _state = FETCH;
  };
  ~Sequencer() {};

  void SetupControlConnections() {
    BussedItem<aN, dN, cN>::_controlBus->Set(&_controlReg);
  }

  void Decode() {
    std::bitset<dN> *word;

    word = _CIR.GetValuePointer();

    for (int i=0; i<OPCODE_BITS; i++) {
      _opcode.set(OPCODE_BITS - 1 - i, word->test(word->size() - 1 - i));
    }

    Singleton<Logger>::GetInstance()->Log(LOG_TYPE_DEBUG, "Decoded Opcode " + CreateString(_opcode.to_ulong()));
  }

  void Fetch() {
    BussedItem<aN, dN, cN>::_addressBus->Set(_PC);

    _controlReg->SetBit(CONTROL_WRITE);

  }

  void Store() {

  }

  void Initialise() {
    BussedItem<aN, dN, cN>::_addressBus->Set(&_PC);
  }

  void Clock() {
    switch (_state) {
      case FETCH:
        Singleton<Logger>::GetInstance()->Log(LOG_TYPE_DEBUG, "Fetch");
        _state = EXECUTE;
        break;
      case EXECUTE:
        Singleton<Logger>::GetInstance()->Log(LOG_TYPE_DEBUG, "Execute");
        _state = FETCH;
        break;
    }
  }

private:
  States _state;

  std::bitset<OPCODE_BITS> _opcode;
  std::bitset<OPERAND_BITS> _operand; // TODO this is not used

  Register<cN> _controlReg; // Value on the control bus
};
 #endif
