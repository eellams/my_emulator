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

#ifndef _REGISTER_HPP
#define _REGISTER_HPP

#include "system.hpp"
#include "bussedItem.hpp"
#include "myBitset.hpp"

#include <string>

#define REGISTER_DEFAULT_NAME "UNKNOWN_REGISTER"
#define REGISTER_TYPE_NAME "REGISTER"

template<size_t N>
class Register : public BussedItem {
public:

  Register(std::string name = REGISTER_DEFAULT_NAME) : BussedItem(REGISTER_TYPE_NAME, name) {
    _output.SetParent(this);
    _output.SetName("Register Output");
  }
  ~Register() {  }

  // Clock in the input (if enabled)
  void Clock() {
    Update();
    if ((*_writeEnableP) == true) {
      log(LOG_TYPE_DEBUG, "Clocking in " + _inputP->GetFullName() + "value: " + createString(_inputP->to_ulong()));
      _output.SetValue(*_inputP);
      Update();
    }
  }

  void SetWriteEnableP(bool *value) {
    // TODO do we need to put a log here?
    _writeEnableP = value;
  }

  void SetInputP(MyBitset<N> *inputP) {
    _inputP = inputP;
  }

  // Return the pointer to the output
  //  useful for setting busses
  MyBitset<N>* GetOutputP() {
    return &_output;
  }

  // Nothing to update
  //  as the input is updated within RegisterFile::Update
  void Update() {
    log(LOG_TYPE_UPDATE, "Update [EMPTY]");
  }

  virtual void LogSignals() {
    std::vector<struct Signal> toSend;
    struct Signal toAdd;

    toAdd.Name = createLogPrefix() + std::string("Input");
    toAdd.Value = _inputP->to_ulong();
    toAdd.Address = static_cast<void*>(_inputP);

    toAdd.Name = createLogPrefix() + std::string("Contents");
    toAdd.Value = _output.to_ulong();
    toAdd.Address = static_cast<void*>(&_output);

    toSend.push_back(toAdd);
    sendSignals(toSend);
  }

private:
  MyBitset<N> *_inputP;
  MyBitset<N> _output;

  bool *_writeEnableP;

};

#endif
