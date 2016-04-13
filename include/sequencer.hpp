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

#define REG_CONTROL_NAME "Sequencer Control Register"
#define REG_CIR_NAME "Current Instruction Register"
#define REG_MAR_NAME "Memory Address Register"
#define REG_PC_NAME "Program Counter"
#define REG_MDR_NAME "Memory Data Register"

#define SEQUENCER_NAME "Sequencer"
#define SEQUENCER_TYPE_NAME "SEQUENCER"

enum States{
  FETCH_PC,
  FETCH_INSTRUCTION,
  STORE_INSTRUCTION,
  EXECUTE,
  FINISHED
};

class Sequencer : public BussedItem {
public:
  Sequencer(std::string name);
  ~Sequencer();

  void SetupControlConnections();

  void Clock();

  void SetRegisterFile(RegisterFile *registerFile);
  void SetMemory(Memory *memory);

  void Signals();
  
  bool Finished() {
    if (_state == FINISHED) return true;
    return false;
   }

private:
  States _state;
  RegisterFile *_registerFile;
  Memory *_memory;

  MyBitset<BUS_WIDTH> _controlBusValue;
};
 #endif
