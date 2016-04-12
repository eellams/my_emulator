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

#ifndef _REGISTER_FILE_HPP
#define _REGISTER_FILE_HPP

#include <string>

#include "system.hpp"
#include "bussedItem.hpp"
#include "register.hpp"

#define REG_NUM 1 << REG_WIDTH
#define REG_GEN_NAME "Registers"

// Reserved registers
#define REG_ZERO 0
#define REG_ACC 1
#define REG_MAR 2
#define REG_MDR 3
#define REG_PC 4
#define REG_CIR 5

#define REG_ZERO_NAME "Zero Reg"
#define REG_ACC_NAME "Accumulator"
#define REG_MAR_NAME "Memory Address Register"
#define REG_MDR_NAME "Memory Data Register"
#define REG_PC_NAME "Program Counter"
#define REG_CIR_NAME "Current Instruction Register"

class RegisterFile : public BussedItem {
public:
  RegisterFile();
  ~RegisterFile();


  void SetupRegisters();

  void Clock();

  void ReadFromRegister(int registerNumber);
  void WriteToRegister(int registerNumber);

private:
  Register<DATA_WIDTH> _reg[REGISTER_NUMBER];

  bool _registerEnables[REGISTER_NUMBER];
};

#endif
