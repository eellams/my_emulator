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

// Reserved registers
// Now have no reserved registers - stored elsewhere
#define REG_RESERVED_NUMBER 0
#define REG_GENERAL_NUMBER 4 // 4 general purpose registers
#define REG_NUMBER REG_RESERVED_NUMBER + REG_GENERAL_NUMBER

// The 'address' of the registers
#define REG_ZERO 0

#define REG_ZERO_NAME "Zero Reg"
#define REG_GENERATION_NAME "General Register "

#define REG_FILE_NAME "Register File"
#define REG_FILE_TYPE_NAME "REGISTER_FILE"

class RegisterFile : public BussedItem {
public:
  RegisterFile(std::string name = REG_FILE_NAME);

  ~RegisterFile();

  void Initialise();

  void SetOutput(size_t registerNumber);

  MyBitset<BUS_WIDTH>* GetOutputP();

  // Set the input and the enable flag as required
  //  note that won't do anything until Clock called
  void SetRegisterP(size_t regNumber, MyBitset<REGISTER_WIDTH> *value);

  // Signal that on the next clock cycle, we want to load
  //  address bus into the register
  void EnableRegister(size_t regNumber);

  // Do all synchronous operations
  void Clock();

  virtual void LogSignals();

  void Update();

private:
  Register<REGISTER_WIDTH> _registers[REG_NUMBER];
  bool _regEnalbes[REG_NUMBER];

  MyBitset<REGISTER_WIDTH> _output;
  int _currentOutput;
};

#endif
