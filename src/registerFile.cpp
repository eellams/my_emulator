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

#include "registerFile.hpp"

RegisterFile::RegisterFile(std::string name) : BussedItem(REG_FILE_TYPE_NAME, name) {
  _output.SetParent(this);
  _output.SetName("Output");
}
RegisterFile::~RegisterFile() {}

void RegisterFile::Initialise() {
  // Set the name, enable and input as required
  for (int i=0; i<REG_NUMBER; i++) {
    switch(i) {
      case REG_ZERO: _registers[i].SetName(REG_ZERO_NAME); break;
      default: _registers[i].SetName(std::string(REG_GENERATION_NAME) + createString(i - REG_RESERVED_NUMBER)); break;
    }
    _registers[i].SetWriteEnableP(&_regEnalbes[i]);
    _registers[i].SetInputP(_dataBusP->GetValueP());
    _regEnalbes[i] = false;
  }

  _addressBusP->SetValueP(_registers[REG_ZERO].GetOutputP());
  _dataBusP->SetValueP(_registers[REG_ZERO].GetOutputP());

  _currentOutput = 0;
}

void RegisterFile::SetOutput(size_t registerNumber) {
  log(LOG_TYPE_INFO, "Setting output to register: " + _registers[registerNumber].GetFullName() + " contents: " + createString(_registers[registerNumber].GetOutputP()->to_ulong()));
  _output.SetValue(*(_registers[registerNumber].GetOutputP()));
  _currentOutput = registerNumber;
}
MyBitset<BUS_WIDTH>* RegisterFile::GetOutputP() {
  return &_output;
}

// Set the input and the enable flag as required
//  note that won't do anything until Clock called
void RegisterFile::SetRegisterP(size_t regNumber, MyBitset<REGISTER_WIDTH> *value) {
  log(LOG_TYPE_DEBUG, "Setting this output to register: " + _registers[regNumber].GetFullName() + " contents: " + createString(_registers[regNumber].GetOutputP()->to_ulong()));
  _registers[regNumber].SetInputP(value);
}

// Signal that on the next clock cycle, we want to load
//  address bus into the register
void RegisterFile::EnableRegister(size_t regNumber) {
  _regEnalbes[regNumber] = true;
}

// Do all synchronous operations
void RegisterFile::Clock() {
  long PC; // Value used for incrementing PC register

  Update(); // Ensure all register inputs are up to date

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

void RegisterFile::LogSignals() {
  std::vector<struct Signal> toSend;
  struct Signal toAdd;

  for (int i=0; i<REG_NUMBER; i++) {
    _registers[i].LogSignals();
  }
}

void RegisterFile::Update() {
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
