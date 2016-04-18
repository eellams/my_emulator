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

#ifndef _ALU_HPP
#define _ALU_HPP

#include "bussedItem.hpp"
#include "registerFile.hpp"
#include "register.hpp"
#include "myBitset.hpp"

#define ALU_NAME "ALU"
#define ALU_TYPE_NAME "ALU"

class ALU : public BussedItem {
public:
  ALU(std::string name = ALU_NAME);
  ~ALU();

  // Adds data bus to accumulator
  void Add();

  // Send the flag that the numbers to be added are signed
  void Signed();

  void Clock();

  // This class has no children, and has no memory
  //  so has nothing to update
  //  Update therefore is seldom called
  void Update();

  // Nothing to log?
  void LogSignals();

  // Set the pointer to the register file class
  void SetRegisterFileP(RegisterFile *value);

  // Reset the accumulator to zeros
  void ResetACC();

  // Get a pointer to the ACC register
  MyBitset<BUS_WIDTH>* GetACCP();

private:
  RegisterFile *_registerFileP;

  MyBitset<BUS_WIDTH> _ACC;
  bool _add, _signed;
};
 #endif
