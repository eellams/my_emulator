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
  FINISHED = 0x05,
};

class Sequencer : public BussedItem {
public:
  Sequencer(std::string name = SEQUENCER_NAME);
  ~Sequencer();

  // Setupt the PC and CIR registers that are contained within this class
  void Initialise();

  // Increment the Program Counter by 1
  void IncrementPC();

  // The 'brains' of this emulator
  void Clock();

  // Setup pointers to the other classes required for operation
  void SetRegisterFileP(RegisterFile *registerFileP);
  void SetMemoryP(Memory *memoryP);
  void SetALUP(ALU *aluP);

  // Check if in the finished state
  bool Finished();

  void LogSignals();

  // Should be called whenever a bus's value is changed
  void Update();

private:
  // Get the name of the current state
  // TODO this needs to be updated
  std::string getStateName();

  States _state;

  // Pointers to other classes
  //  TODO, this really breaks encapsulation
  RegisterFile *_registerFileP;
  Memory *_memoryP;
  ALU *_aluP;

  // Registers and enables in this module
  Register<REGISTER_WIDTH> _PC;
  Register<REGISTER_WIDTH> _CIR;
  bool _ePC, _eCIR;

  // Used for keeping track of where we are in the emulation
  long _executeNumber;
  long _numberOfClocks;
};
 #endif
