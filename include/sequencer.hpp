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

template<size_t aN, size_t dN, size_t cN>
class Sequencer : public BussedItem<aN, dN, cN> {
public:
  Sequencer() : BussedItem<aN, dN, cN>() {
    _controlReg.SetName(REG_CONTROL_NAME);

    _PC.SetName(REG_PC_NAME);
    _CIR.SetName(REG_CIR_NAME);
    _MAR.SetName(REG_MAR_NAME);
    _MDR.SetName(REG_MDR_NAME);
  };
  ~Sequencer() {};

  void SetupControlConnections() {
    BussedItem<aN, dN, cN>::_controlBus->Set(&_controlReg);
  }

  void FirstClock() {
    BussedItem<aN, dN, cN>::_addressBus->Set(&_PC);
    
  }

  void Clock() {

  }

private:
  Register<cN> _controlReg; // Value on the control bus

  Register<dN> _PC; // Address of current instruction
  Register<aN> _CIR; // Value of current instruction
  Register<aN> _MAR; // Memory address register
  Register<dN> _MDR; // Memory data register
};
 #endif
