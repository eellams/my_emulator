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

#include <vector>
#include <iterator>

#include "system.hpp"
#include "singleton.hpp"
#include "logger.hpp"

#include "bussedItem.hpp"
#include "register.hpp"
#include "registerFile.hpp"
#include "memory.hpp"
#include "alu.hpp"

#define REG_CONTROL_NAME "Sequencer Control Register"
#define REG_CIR_NAME "Current Instruction Register"
#define REG_MAR_NAME "Memory Address Register"
#define REG_PC_NAME "Program Counter"
#define REG_MDR_NAME "Memory Data Register"

#define SEQUENCER_NAME "Sequencer"
#define SEQUENCER_TYPE_NAME "SEQUENCER"

enum States{
  FETCH_PC = 0x00,
  FETCH_INSTRUCTION = 0x01,
  EXECUTE = 0x02,
  FINISHED = 0x03
};

class Sequencer : public BussedItem {
public:
  Sequencer(std::string name = SEQUENCER_NAME) : BussedItem(SEQUENCER_TYPE_NAME, name) {}
  ~Sequencer() {}

  void Clock() {
    Update();

    log(LOG_TYPE_INFO, "Clock");
    switch (_state) {
      case FETCH_PC:
        // Set address bus to PC
        _addressBusP->SetValueP(_registerFileP->GetPCP());

        _state = FETCH_INSTRUCTION;
        break;

      case FETCH_INSTRUCTION:
        // Read at addreses bus, write to CIR
        _memoryP->Read();

        _registerFileP->SetCIRP(_dataBusP->GetValueP());

        _state = EXECUTE;
        break;
      case EXECUTE:
        // Current instruction is on the data bus, and in CIR
        long data;
        long opcode;
        long imm;

        data = _registerFileP->GetCIRP()->to_ulong();
        opcode = data & BITS_OP;
        imm = data & BITS_IMM;

        switch(opcode) {
          case INSTR_ADDI:
            // Add the imm of the data bus with the value of ACC,
            //  put value on databus,
            //  read databus into ACC

            log(LOG_TYPE_INFO, "Add command");
            _alu->Add();
            _registerFileP->SetACCP(_dataBusP->GetValueP());
            break;
          default:
            log(LOG_TYPE_ERROR, "Unknown instruction");
            break;
        }
        _registerFileP->IncPC();
        _state = FETCH_PC;
        break;
    }

    _alu->Clock();
    Update();

    _memoryP->Clock();
    Update();

    _registerFileP->Clock();
    Update();

    LogSignals();
    _addressBusP->LogSignals();
    _dataBusP->LogSignals();
    //_controlBusP->LogSignals();

    _memoryP->LogSignals();
    _registerFileP->LogSignals();
    Singleton<Logger>::GetInstance()->WriteSignals();
  }

  void SetRegisterFileP(RegisterFile *registerFileP) { _registerFileP = registerFileP; }
  void SetMemoryP(Memory *memoryP) { _memoryP = memoryP; }
  void SetALUP(ALU *alu) { _alu = alu; }

  bool Finished() {
    if (_state == FINISHED) return true;
    return false;
   }

   virtual void LogSignals() {
     std::vector<struct Signal> toSend;
     struct Signal toAdd;

     toAdd.Name = createLogPrefix() + std::string("Sequencer State");
     toAdd.Value = static_cast<long>(_state);
     toAdd.Address = 0;

     toSend.push_back(toAdd);
     sendSignals(toSend);
   }

   void Update() {
     log(LOG_TYPE_UPDATE, "Update");
     _registerFileP->Update();
     _memoryP->Update();
   }

private:
  States _state;
  RegisterFile *_registerFileP;
  Memory *_memoryP;
  ALU *_alu;

  MyBitset<BUS_WIDTH> _controlBusValue;
};
 #endif
