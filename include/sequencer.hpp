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

#define SEQUENCER_TYPE_NAME "SEQUENCER"
#define SEQUENCER_NAME "Sequencer"

enum States{
  STARTUP = 0x00,
  FETCH = 0x01,
  EXECUTE = 0x02,
  EXECUTE_LDI = 0x03,
  FINISHED = 0x04,
};

class Sequencer : public BussedItem {
public:
  Sequencer(std::string name = SEQUENCER_NAME) : BussedItem(SEQUENCER_TYPE_NAME, name) {
    _numberOfClocks = 0;
    _state = STARTUP;
  }
  ~Sequencer() {}

  void Clock() {
    Update();

    log(LOG_TYPE_INFO, "Clock");

    // Actions taken depend on the current state
    switch (_state) {
      case STARTUP:
        log(LOG_TYPE_INFO, "Startup clock - do nothing");
        _state = FETCH;
        break;

      case FETCH:
        // Set address bus to PC
        // Get memory, put on address Bus
        // Clock address bus into CIR
        // Increment PC

        // Set address bus to PC
        _registerFileP->SetOutput(REG_PC);
        _addressBusP->SetValueP(_registerFileP->GetOutputP());

        // Get memory at address, put on data bus
        _memoryP->Read();

        // Clock address bus into CIR
        _registerFileP->EnableRegister(REG_CIR);
        _state = FINISHED;

        // Increment PC
        _registerFileP->IncPC();

        _state = EXECUTE;
        break;

      case EXECUTE:
        log(LOG_TYPE_INFO, "Decoding and executing instruction");
        // Current instruction is on the data bus, and in CIR
        long data;
        long opcode;
        long imm;

        // Get input, and bitmask into component bits

        // Get CIR, put on data bus
        _registerFileP->SetOutput(REG_CIR);
        _dataBusP->SetValueP(_registerFileP->GetOutputP());

        data = _dataBusP->GetValueP()->to_ulong();

        opcode = (data & BITMASK_OP) >> (WORD_WIDTH - BITMASK_OP_WIDTH);
        imm = data & BITMASK_IMM;

        log(LOG_TYPE_DEBUG, "Decoded instruction: " + createString(opcode) + " and operand: " + createString(imm));

        switch(opcode) {
          case INSTR_ADDI:
            // Add the imm of the data bus with the value of ACC,
            //  put value on databus,
            //  read databus into ACC

            log(LOG_TYPE_INFO, "ADDI command");
            _aluP->Add(); // Signal that we want the ALU to add

            // Clock into ACC
            _registerFileP->EnableRegister(REG_ACC);

            _state = FETCH;
            break;

          case INSTR_JMPI:
            log(LOG_TYPE_INFO, "JMPI command");

            // Decoded instructiom
            // Reset ACC
            // Clock in operand to ACC

            _registerFileP->ResetRegister(REG_ACC);
            _aluP->Add();
            _aluP->Signed();

            _registerFileP->EnableRegister(REG_ACC);

            _state = EXECUTE_LDI;

            break;

          default:
            log(LOG_TYPE_ERROR, "Unknown instruction");
            _state = FINISHED;
            break;
        }

        break;

        case EXECUTE_LDI:
          log(LOG_TYPE_INFO, "Executing second clock of LDI instruction");
          // Set data bus to PC
          // Add data bus to ACC
          // Clock in data bus to PC

          _registerFileP->SetOutput(REG_PC);
          _dataBusP->SetValueP(_registerFileP->GetOutputP());

          _aluP->Add();

          _registerFileP->EnableRegister(REG_PC);

          _state = FETCH;
          break;
    }

    _aluP->Clock();
    Update(); // Could change a bus value

    _memoryP->Clock();
    Update(); // Could change a bus value

    _registerFileP->Clock();
    Update(); // Could change a bus value

    _numberOfClocks += 1;

    LogSignals();
    _addressBusP->LogSignals();
    _dataBusP->LogSignals();

    _memoryP->LogSignals();
    _registerFileP->LogSignals();
    _aluP->LogSignals();

    Singleton<Logger>::GetInstance()->WriteSignals();
  }

  void SetRegisterFileP(RegisterFile *registerFileP) { _registerFileP = registerFileP; }
  void SetMemoryP(Memory *memoryP) { _memoryP = memoryP; }
  void SetALUP(ALU *aluP) { _aluP = aluP; }

  bool Finished() {
    if (_state == FINISHED) return true;
    return false;
   }

   void LogSignals() {
     std::vector<struct Signal> toSend;
     struct Signal toAdd;

     toAdd.Name = createLogPrefix() + std::string("Clock number: ");
     toAdd.Value = _numberOfClocks;
     toAdd.Address = 0;
     toSend.push_back(toAdd);

     toAdd.Name = createLogPrefix() + std::string("Sequencer State (next): ") + getStateName();
     toAdd.Value = static_cast<long>(_state);
     toAdd.Address = 0;

     toSend.push_back(toAdd);
     sendSignals(toSend);
   }

   // Should be called whenever a bus's value is changed
   void Update() {
     log(LOG_TYPE_UPDATE, "Update");

     _registerFileP->Update();
     _memoryP->Update();
     _aluP->Update();
   }

private:
  std::string getStateName() {
    std::string toReturn;

    switch (_state) {
      case STARTUP: toReturn = std::string("Startup"); break;
      case FETCH: toReturn = std::string("Fetch"); break;
      case EXECUTE: toReturn = std::string("Execute"); break;
      case EXECUTE_LDI: toReturn = std::string("Execute LDI"); break;
      case FINISHED: toReturn = std::string("Finished"); break;
    }

    return toReturn;
  }

  States _state;
  RegisterFile *_registerFileP;
  Memory *_memoryP;
  ALU *_aluP;

  MyBitset<BUS_WIDTH> _controlBusValue;
  long _numberOfClocks;
};
 #endif
