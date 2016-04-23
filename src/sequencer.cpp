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

  _controlBusValue.SetName("Control Bus Value");
  _controlBusValue.SetParent(this);
  _controlBusValue.reset();

  _controlBusP->SetValueP(&_controlBusValue);

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
      SetControlBit(CONTROL_BUS_MEMORY_READ);

      // Clock address bus into CIR
      _eCIR = true;

      _state = EXECUTE;
      break;

    case EXECUTE:
      log(LOG_TYPE_INFO, "Decoding and executing instruction");
      // Current instruction is on the data bus, and in CIR

      long data;
      long opcode;
      long imm, shortImm;
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
      shortImm = (data & BITMASK_IMM_SHORT) >> (BITMASK_REG_B_WIDTH);

      // Bitmask, and shift to remove redundant zeros
      regA = (data & BITMASK_REG_A) >> (BITMASK_REG_B_WIDTH);
      regB = (data & BITMASK_REG_B);


      log(LOG_TYPE_DEBUG, "Decoded instruction: " + createString(opcode) + " and operand: " + createString(imm));
      log(LOG_TYPE_DEBUG, "Or Register A: " + createString(regA) + ", Register B: " + createString(regB));
      log(LOG_TYPE_DEBUG, "Or ShortImm: " + createString(shortImm) + ", Register B: " + createString(regB));

      switch(opcode) {
        case INSTR_ADDI:
          // RegA := RegA + imm
          log(LOG_TYPE_INFO, "ADDI, " + createString(shortImm, false) + ", " + createString(regB, false) + " command");

          // Reset ACC
          // Add RegB to ACC
          // Clock
          // Add imm to ACC
          // Store in RegB

          if (_executeNumber == 0) {
            log(LOG_TYPE_INFO, "First ADDI clock cycle");
            // Reset ACC
            SetControlBit(CONTROL_BUS_ALU_RESET_ACC);

            // Set data bus to RegB
            _registerFileP->SetOutput(REG_RESERVED_NUMBER + regB);
            ///_dataBusP->SetValueP(_registerFileP->GetOutputP());

            // Add RegA to ACC
            SetControlBit(CONTROL_BUS_ALU_ADD);

            _state = EXECUTE;
          }
          else if (_executeNumber == 1) {
            log(LOG_TYPE_INFO, "Second ADDI clock cycle");
            // Set data bus to CIR
            _dataBusP->SetValueP(_CIR.GetOutputP());

            // Set flags
            //  Only add the 3 bits below instruction
            SetControlBit(CONTROL_BUS_ALU_IMM_SHORT);
            SetControlBit(CONTROL_BUS_ALU_ADD);
            SetControlBit(CONTROL_BUS_ALU_SIGNED);

            // Store in RegB
            _registerFileP->EnableRegister(REG_RESERVED_NUMBER + regB);

            IncrementPC();
            _state = FETCH;
          }
          break;

        case INSTR_ADD:
          log(LOG_TYPE_INFO, "ADD, " + createString(regA, false) + ", " + createString(regB, false) + " command");

          if (_executeNumber == 0) {
            log(LOG_TYPE_INFO, "ADD First clock cycle");
            // ACC := 0 (reset ACC)
            // ACC := ACC + RegA (ACC := RegA)

            // Reset ACC
            SetControlBit(CONTROL_BUS_ALU_RESET_ACC);

            // Set the data bus
            _registerFileP->SetOutput(REG_RESERVED_NUMBER + regA);
            ///_dataBusP->SetValueP(_registerFileP->GetOutputP());

            // Set control bus flags
            SetControlBit(CONTROL_BUS_ALU_ADD);

            // Another execute clock cycle to go
            _state = EXECUTE;
          }
          else if (_executeNumber == 1){
            log(LOG_TYPE_INFO, "ADD Second clock cycle");
            // ACC := ACC + RegB
            // RegA := ACC [Store in RegA]

            // Set the data bus
            _registerFileP->SetOutput(REG_RESERVED_NUMBER + regB);
            ///_dataBusP->SetValueP(_registerFileP->GetOutputP());

            // Set control bus flags
            SetControlBit(CONTROL_BUS_ALU_ADD);
            SetControlBit(CONTROL_BUS_ALU_SIGNED);

            // Clock into RegA
            _registerFileP->EnableRegister(REG_RESERVED_NUMBER + regA);

            // Move on to the next command
            IncrementPC();
            _state = FETCH;
          }
          break;

          case INSTR_LOAD:
            log(LOG_TYPE_INFO, "LOAD, " + createString(regA, false) + ", " + createString(regB, false) + " command");
            // Store RegA with memory at address in RegB

            // Clear ACC
            // Set ACC to RegB
            // Clock
            // Set address bus to ACC
            // Set read flag
            // Clock into RegA

            if (_executeNumber == 0) {
              // ACC := 0
              // ACC := ACC + RegB

              // Clear ACC
              SetControlBit(CONTROL_BUS_ALU_RESET_ACC);

              // Add RegB to ACC
              _registerFileP->SetOutput(REG_RESERVED_NUMBER + regB);
              ///_dataBusP->SetValueP(_registerFileP->GetOutputP());

              SetControlBit(CONTROL_BUS_ALU_ADD);

              _state = EXECUTE;
            }
            else if (_executeNumber == 1) {
              // RegA := Mem[ACC]

              // Setup address bus
              //  TODO is this method actually required?
              _addressBusP->SetValueP(_aluP->GetACCP());

              // Setup control flags
              SetControlBit(CONTROL_BUS_MEMORY_READ);

              // Clock data bus into regA
              //  note that memory automatically puts read data onto data bus
              _registerFileP->EnableRegister(REG_RESERVED_NUMBER + regA);


              IncrementPC();
              _state = FETCH;
            }
            break;

        case INSTR_STORE:
          log(LOG_TYPE_INFO, "STORE, " + createString(regA, false) + ", " + createString(regB, false) + " command");

          // Reset ACC
          // Add RegB to ACC
          // Clock
          // Set address bus to ACC
          // Set data bus to RegA
          // Write to memory

          if (_executeNumber == 0) {
            // ACC := 0
            // ACC := ACC + RegB

            // Clear ACC
            SetControlBit(CONTROL_BUS_ALU_RESET_ACC);

            // Add RegB to ACC
            _registerFileP->SetOutput(REG_RESERVED_NUMBER + regB);
            ///_dataBusP->SetValueP(_registerFileP->GetOutputP());

            SetControlBit(CONTROL_BUS_ALU_ADD);

            _state = EXECUTE;
          }
          else if (_executeNumber == 1) {
            // Mem[ACC] := RegA

            // Set address bus to ACC
            _addressBusP->SetValueP(_aluP->GetACCP());

            // Set data bus to RegA
            _registerFileP->SetOutput(REG_RESERVED_NUMBER + regA);
            ///_dataBusP->SetValueP(_registerFileP->GetOutputP());

            // Write
            SetControlBit(CONTROL_BUS_MEMORY_WRITE);

            IncrementPC();
            _state = FETCH;
          }
          break;

        case INSTR_NAND:
          log(LOG_TYPE_INFO, "NAND, " + createString(regA, false) + ", " + createString(regB, false) + " command");
          // Set RegA to RegA NAND RegB (Reg = ~(RegA & RegB) )

          // Reset ACC
          // Set data bus to RegA
          // Add data bus to ACC (i.e. set ACC to RegA)
          // Clock
          // Set data bus to RegB
          // Send NAND flag to ACC
          // Store in RegA

          if (_executeNumber == 0) {
            log(LOG_TYPE_INFO, "First NAND clock cycle");

            // ACC := 0
            // ACC := ALU + RegA

            // Reset ACC
            SetControlBit(CONTROL_BUS_ALU_RESET_ACC);

            // Set data bus to RegA
            _registerFileP->SetOutput(REG_RESERVED_NUMBER + regA);
            ///_dataBusP->SetValueP(_registerFileP->GetOutputP());

            // Add data bus [RegA] to ALU
            SetControlBit(CONTROL_BUS_ALU_ADD);
            _state = EXECUTE;
          }
          else if (_executeNumber == 1) {
            log(LOG_TYPE_INFO, "Second JMPI clock cycle");

            // ACC := ~(ACC & RegB)
            // RegA := ACC

            // Set data bus to RegB
            _registerFileP->SetOutput(REG_RESERVED_NUMBER + regB);
            ///_dataBusP->SetValueP(_registerFileP->GetOutputP());

            // Send NAND flag to ACC
            //  Acts to NAND ACC [RegB] with data bus [RegA]
            SetControlBit(CONTROL_BUS_ALU_NAND);

            // Store in RegA
            _registerFileP->EnableRegister(REG_RESERVED_NUMBER + regA);

            // Not touching PC, so increment as normal
            IncrementPC();
            _state = FETCH;
          }
          break;

        case INSTR_JMPI:
          log(LOG_TYPE_INFO, "JMPI, " + createString(imm, false) + " command");
          // Reset ACC
          // Set data bus to PC
          // Set ADD flag to ALU
          // Clock
          // Set data bus to CIR
          // Set ADD, SIGNED and IMM flag to ALU
          // Clock [ACC] into PC

          if (_executeNumber == 0) {
            log(LOG_TYPE_INFO, "First JMPI clock cycle");

            // ACC := 0
            // ACC := ACC + PC

            // Reset ACC
            SetControlBit(CONTROL_BUS_ALU_RESET_ACC);

            // Set data bus to PC
            _dataBusP->SetValueP(_PC.GetOutputP());

            // Set flags for ALU
            SetControlBit(CONTROL_BUS_ALU_ADD);

            _state = EXECUTE;
          }

          else {
            log(LOG_TYPE_INFO, "Second JMPI clock cycle");

            // ACC := ACC + imm
            // PC := ACC

            // Set data bus to CIR
            _dataBusP->SetValueP(_CIR.GetOutputP());

            // Set ADD and IMM flag to ALU
            SetControlBit(CONTROL_BUS_ALU_ADD);
            SetControlBit(CONTROL_BUS_ALU_IMM);
            SetControlBit(CONTROL_BUS_ALU_SIGNED);

            // Enable PC (clock in databus value)
            _ePC = true;

            // Don't increase PC, as we have already changed it?
            _state = FETCH;
          }
          break;

        case INSTR_BZ:
          log(LOG_TYPE_INFO, "BZ, " + createString(imm, false) + " command");
          // If zero flag is not set, add IMM to PC
          // IMM can be signed

          if ( _aluP->GetZeroFlag() && (_executeNumber == 0) ) {
            log(LOG_TYPE_DEBUG, "Zero flag was set - jumping");
            log(LOG_TYPE_DEBUG, "First BZ clock cycle");

            // ACC := 0
            // ACC := ACC + PC

            // Reset ACC
            SetControlBit(CONTROL_BUS_ALU_RESET_ACC);

            // Add PC to ACC
            _dataBusP->SetValueP(_PC.GetOutputP());

            SetControlBit(CONTROL_BUS_ALU_ADD);
            _state = EXECUTE;
          }
          else if (_executeNumber == 1) {
            log(LOG_TYPE_DEBUG, "Second BZ clock cycle");

            // ACC := ACC + imm
            // PC := ACC

            // Add IMM to ACC (which contains PC)
            _dataBusP->SetValueP(_CIR.GetOutputP());

            SetControlBit(CONTROL_BUS_ALU_ADD);
            SetControlBit(CONTROL_BUS_ALU_IMM);
            SetControlBit(CONTROL_BUS_ALU_SIGNED);

            // Clock into program Counter
            _ePC = true;

            _state = FETCH;
          }
          else if (_executeNumber == 0) {
            // Flag was not zero, carry on execution
            log(LOG_TYPE_INFO, "Zero flag was not set");

            // Do nothing
            // Only increment PC

            IncrementPC();
            _state = FETCH;
          }
          break;

        case INSTR_FINISH:
          log(LOG_TYPE_INFO, "FINISH command");
          _state = FINISHED;
          break;

        default:
          log(LOG_TYPE_ERROR, "Unknown instruction: " + createString(opcode));
          _state = FINISHED;
          break;
      }

      _executeNumber++;
      break;

    case FINISHED:
      log(LOG_TYPE_ERROR, "Clocking after finish state entered");
      break;
  }

  _aluP->Clock();
  Update(); // Could change a bus value

  _memoryP->Clock();
  Update(); // Could change a bus value

  _registerFileP->Clock();
  Update(); // Could change a bus value

  _PC.Clock();
  _CIR.Clock();
  Update();

  // Reset values that were set this clock cycle
  //  so they have no effect of later clock cycles
  _controlBusValue.reset();
  _ePC = _eCIR = false;

  // Used for debugging / emulation information
  _numberOfClocks += 1;

  // Log all of the signals
  //  as they have already changed for this clock cycle
  LogSignals();
  _addressBusP->LogSignals();
  _dataBusP->LogSignals();

  _memoryP->LogSignals();
  _registerFileP->LogSignals();
  _aluP->LogSignals();

  Singleton<Logger>::GetInstance()->WriteSignals();
}

// Only ever called during initialisation from the main function
void Sequencer::SetRegisterFileP(RegisterFile *registerFileP) { _registerFileP = registerFileP; }
void Sequencer::SetMemoryP(Memory *memoryP) { _memoryP = memoryP; }
void Sequencer::SetALUP(ALU *aluP) { _aluP = aluP; }

// Check if CIR points to FINISHED command
bool Sequencer::Finished() {
  if (_state == FINISHED) return true;
  return false;
 }

// Debugging
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

  toAdd.Name = GetFullName() + _PC.GetFullName();
  toAdd.Value = _PC.GetOutputP()->to_ulong();
  toAdd.Address = static_cast<void*>(_PC.GetOutputP());
  toSend.push_back(toAdd);

  toAdd.Name = GetFullName() + _CIR.GetFullName();
  toAdd.Value = _CIR.GetOutputP()->to_ulong();
  toAdd.Address = static_cast<void*>(_CIR.GetOutputP());
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
  case FINISHED: toReturn = std::string("Finished"); break;
}

return toReturn;
}
