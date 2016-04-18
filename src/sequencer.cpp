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
  _numberOfClocks = 0;
  _state = STARTUP;
}
Sequencer::~Sequencer() {}

void Sequencer::Initialise() {
  _PC.SetName("Program Counter");
  _PC.SetWriteEnableP(&_ePC);
  _PC.SetInputP(_dataBusP->GetValueP());

  _CIR.SetName("Current Instruction Register");
  _CIR.SetWriteEnableP(&_eCIR);
  _CIR.SetInputP(_dataBusP->GetValueP());

  _ePC = _eCIR = false;
}

void Sequencer::IncrementPC() {
  // Increment PC
  long PCVal;

  PCVal = _PC.GetOutputP()->to_ulong(); // Get the value
  (*_PC.GetOutputP()) ^= PCVal; // XOR - sets the value to 0

  log(LOG_TYPE_INFO, "Incrementing " + _PC.GetFullName() + " from: " + createString(PCVal) + " to: " + createString(PCVal + 1));
  PCVal++; // Actually increment

  *(_PC.GetOutputP()) |= PCVal; // OR sets the bits as required
}

void Sequencer::Clock() {
  Update();

  log(LOG_TYPE_INFO, "Clock");

  // Actions taken depend on the current state
  switch (_state) {
    case STARTUP:
      log(LOG_TYPE_INFO, "Startup clock - do nothing");
      _state = FETCH;
      break;

    case FETCH:
      log(LOG_TYPE_INFO, "Fetch");
      _executeNumber = 0;
      // Set address bus to PC
      // Get memory, put on address Bus
      // Clock address bus into CIR
      // Increment PC

      // Set address bus to PC
      _addressBusP->SetValueP(_PC.GetOutputP());

      // Get memory at address, put on data bus
      _memoryP->Read();

      // Clock address bus into CIR
      _eCIR = true;

      _state = EXECUTE;
      break;

    case EXECUTE:
      log(LOG_TYPE_INFO, "Decoding and executing instruction");
      // Current instruction is on the data bus, and in CIR

      long data;
      long opcode;
      long imm;
      long regA, regB;

      // Get input, and bitmask into component bits

      // Get CIR, put on data bus
      //
      // TODO this isn't in the spirit of this project, the data bus is
      //  asynchronously loaded with CIR, only to be replaced in the below
      //  switch
      //  would make more sense to allow direct access to CIR in sequencer

      // Current instruction - to be decoded
      data = _CIR.GetOutputP()->to_ulong();

      opcode = (data & BITMASK_OP) >> (WORD_WIDTH - BITMASK_OP_WIDTH);
      imm = data & BITMASK_IMM;

      regA = (data & BITMASK_REG_A) >> (BITMASK_REG_B_WIDTH);
      regB = (data & BITMASK_REG_B);


      log(LOG_TYPE_DEBUG, "Decoded instruction: " + createString(opcode) + " and operand: " + createString(imm));
      log(LOG_TYPE_DEBUG, "Or Register A: " + createString(regA) + ", Register B: " + createString(regB));

      switch(opcode) {
        case INSTR_ADDI:
          // Add the imm of the data bus with the value of ACC,
          //  put value on databus,
          //  read databus into ACC

          log(LOG_TYPE_INFO, "ADDI command");
          _aluP->Add(); // Signal that we want the ALU to add

          _state = FETCH;
          break;

        case INSTR_ADD:
          log(LOG_TYPE_INFO, "ADD command");

          if (_executeNumber == 0) {
            log(LOG_TYPE_INFO, "First clock cycle");
            // Reset ACC
            // ACC := RegA

            _aluP->ResetACC();

            _registerFileP->SetOutput(REG_RESERVED_NUMBER + regA);
            _dataBusP->SetValueP(_registerFileP->GetOutputP());

            _aluP->Add();

            _state = EXECUTE;
          }
          else if (_executeNumber == 1){
            log(LOG_TYPE_INFO, "Second clock cycle");
            // ACC += RegB
            // Store in RegA

            _registerFileP->SetOutput(REG_RESERVED_NUMBER + regB);
            _dataBusP->SetValueP(_registerFileP->GetOutputP());

            _aluP->Add();

            _registerFileP->EnableRegister(REG_RESERVED_NUMBER + regA);

            _state = FETCH;
          }
          break;

        case INSTR_LOAD:
          log(LOG_TYPE_INFO, "LOAD command");
          // Set address bus to RegB
          // Read from memory
          // Clock into RegA

          // Set the address bus to the register number
          _registerFileP->SetOutput(REG_RESERVED_NUMBER + regB);
          _addressBusP->SetValueP(_registerFileP->GetOutputP());

          // Read from the adderss to data bus
          _memoryP->Read();

          // Clock into
          _registerFileP->EnableRegister(REG_RESERVED_NUMBER + regA);

          _state = FETCH;
          break;

        default:
          log(LOG_TYPE_ERROR, "Unknown instruction: " + createString(opcode));
          _state = FINISHED;
          break;
      }

      _executeNumber++;
      break;
  }

  _aluP->Clock();
  Update(); // Could change a bus value

  _memoryP->Clock();
  Update(); // Could change a bus value

  _registerFileP->Clock();
  Update(); // Could change a bus value

  _PC.Clock();
  Update();

  _CIR.Clock();
  Update();

  _ePC = _eCIR = false;

  _numberOfClocks += 1;

  LogSignals();
  _addressBusP->LogSignals();
  _dataBusP->LogSignals();

  _memoryP->LogSignals();
  _registerFileP->LogSignals();
  _aluP->LogSignals();

  Singleton<Logger>::GetInstance()->WriteSignals();
}

void Sequencer::SetRegisterFileP(RegisterFile *registerFileP) { _registerFileP = registerFileP; }
void Sequencer::SetMemoryP(Memory *memoryP) { _memoryP = memoryP; }
void Sequencer::SetALUP(ALU *aluP) { _aluP = aluP; }

bool Sequencer::Finished() {
  if (_state == FINISHED) return true;
  return false;
 }

void Sequencer::LogSignals() {
  std::vector<struct Signal> toSend;
  struct Signal toAdd;

  toAdd.Name = GetFullName() + std::string("Clock number: ");
  toAdd.Value = _numberOfClocks;
  toAdd.Address = 0;
  toSend.push_back(toAdd);

  toAdd.Name = GetFullName() + std::string("Sequencer State (next): ") + getStateName();
  toAdd.Value = static_cast<long>(_state);
  toAdd.Address = 0;

  toSend.push_back(toAdd);
  sendSignals(toSend);
}

// Should be called whenever a bus's value is changed
void Sequencer::Update() {
  log(LOG_TYPE_UPDATE, "Update");

  _PC.SetInputP(_dataBusP->GetValueP());
  _PC.Update();

  _CIR.SetInputP(_dataBusP->GetValueP());
  _CIR.Update();

  _registerFileP->Update();
  _memoryP->Update();
  _aluP->Update();
}

std::string Sequencer::getStateName() {
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
