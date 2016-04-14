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
#define REG_NUMBER 4 // The number of registers

// The 'address' of the registers
#define REG_ZERO 0
#define REG_ACC 1
#define REG_PC 2
#define REG_CIR 3

/*
#define REG_MAR 2
#define REG_MDR 3
#define REG_PC 4
#define REG_CIR 5
*/

#define REG_ZERO_NAME "Zero Reg"
#define REG_ACC_NAME "Accumulator"
#define REG_MAR_NAME "Memory Address Register"
#define REG_MDR_NAME "Memory Data Register"
#define REG_PC_NAME "Program Counter"
#define REG_CIR_NAME "Current Instruction Register"

#define REG_FILE_NAME "Register File"
#define REG_FILE_TYPE_NAME "REGISTER_FILE"

class RegisterFile : public BussedItem {
public:
  RegisterFile(std::string name = REG_FILE_NAME) : BussedItem(REG_FILE_TYPE_NAME, name) {
    _output.SetParent(this);
    _output.SetName("Output");
  }
  ~RegisterFile() {}

  void Initialise() {
    // Set the name, enable and input as required
    for (int i=0; i<REG_NUMBER; i++) {
      switch(i) {
        case REG_ZERO: _registers[i].SetName(REG_ZERO_NAME); break;
        case REG_ACC: _registers[i].SetName(REG_ACC_NAME); break;
        //case REG_MAR: _registers[i].SetName(REG_MAR_NAME); break;
        //case REG_MDR: _registers[i].SetName(REG_MDR_NAME); break;
        case REG_PC: _registers[i].SetName(REG_PC_NAME); break;
        case REG_CIR: _registers[i].SetName(REG_CIR_NAME); break;
      }
      _registers[i].SetWriteEnableP(&_regEnalbes[i]);
      _registers[i].SetInputP(_dataBusP->GetValueP());
      _regEnalbes[i] = false;
    }

    _addressBusP->SetValueP(_registers[REG_ZERO].GetOutputP());
    _dataBusP->SetValueP(_registers[REG_ZERO].GetOutputP());

    _currentOutput = 0;
    _incPC = false;
  }

  void SetOutput(size_t registerNumber) {
    log(LOG_TYPE_INFO, "Setting output to register: " + _registers[registerNumber].GetFullName() + " contents: " + createString(_registers[registerNumber].GetOutputP()->to_ulong()));
    _output.SetValue(*(_registers[registerNumber].GetOutputP()));
    _currentOutput = registerNumber;
  }
  MyBitset<BUS_WIDTH>* GetOutputP() {
    return &_output;
  }

  // Set the input and the enable flag as required
  //  note that won't do anything until Clock called
  void SetRegisterP(size_t regNumber, MyBitset<REGISTER_WIDTH> *value) {
    _registers[regNumber].SetInputP(value);
    //_regEnalbes[regNumber] = true;
  }

  void EnableRegister(size_t regNumber) {
    _regEnalbes[regNumber] = true;
  }

  // Signal that on the next clock cycle, we want to increment PC
  void IncPC() {
    _incPC = true;
  }

  // Should only be ever called once, in setting up the ALU
  Register<REGISTER_WIDTH>* GetACCP() {
    return &_registers[REG_ACC];
  }

  void ResetRegister(int registerNumber) {
    log(LOG_TYPE_INFO, "Asynchronously resetting register: " + _registers[registerNumber].GetFullName() );
    _registers[registerNumber].GetOutputP()->reset();
  }

  // Do all synchronous operations
  void Clock() {
    long PC; // Value used for incrementing PC register

    Update(); // Ensure all register inputs are up to date

    // If we need to increment the program counter
    if (_incPC) {
      PC = _registers[REG_PC].GetOutputP()->to_ulong(); // Get the value
      (*_registers[REG_PC].GetOutputP()) ^= PC; // XOR - sets the value to 0

      log(LOG_TYPE_INFO, "Incrementing PC from: " + createString(PC) + " to: " + createString(PC + 1));
      PC++; // Actually increment

      (*_registers[REG_PC].GetOutputP()) |= PC; // OR sets the bits as required

      // Assumed to only last one clock cycle
      _incPC = false;
    }

    // Ignore the zero register, which should never change
    //  which is why i=1
    for (int i=1; i<REG_NUMBER; i++) {

      // Ensure input is correct
      //  actually, this doesn't do anything right now
      //  but in theory it could!
      _registers[i].Update();

      // Clock in data as required
      _registers[i].Clock();

      // Reset the enable signal
      //  assumed to only last one clock cycle
      //  will have to be re-set as required
      _regEnalbes[i] = false;
    }
  }

  virtual void LogSignals() {
    std::vector<struct Signal> toSend;
    struct Signal toAdd;

    for (int i=0; i<REG_NUMBER; i++) {
      _registers[i].LogSignals();
    }
  }

  void Update() {
    log(LOG_TYPE_UPDATE, "Update");

    // Update the register inputs
    for (int i=0; i<REG_NUMBER; i++) {
      // Set the input to latest data bus value
      _registers[i].SetInputP(_dataBusP->GetValueP());

      // Update
      _registers[i].Update();
    }

    //_output = *(_registers[_currentOutput].GetOutputP());
    _output.SetValue(*(_registers[_currentOutput].GetOutputP()));
  }

private:
  bool _incPC;

  Register<REGISTER_WIDTH> _registers[REG_NUMBER];
  bool _regEnalbes[REG_NUMBER];

  MyBitset<REGISTER_WIDTH> _output;
  int _currentOutput;

  MyBitset<REGISTER_WIDTH> *_prevDataValue;
};

#endif
