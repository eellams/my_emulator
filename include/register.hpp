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
    _contents.SetParent(this);
    _contents.SetName("Register Contents");
    _contentsP = &_contents;
  }
  ~Register() {  }

  // Clock in the input (if enabled)
  void Clock() {
    MyBitset<N> input(this, "Register Input Buffer");
    if (*_writeEnable) {
      log(LOG_TYPE_DEBUG, "Clocking in " + _input->GetDetails() + "value: " + createString(_input->to_ulong()));
       //input = *_input;
      _contents.SetValue(*_input);

      log(LOG_TYPE_DEBUG, "Contents " + _contents.GetDetails() + "value: " + createString(_contents.to_ulong()));
    }
  }

  void SetWriteEnable(bool *value) {
    // TODO do we need to put a log here?
    _writeEnable = value;
  }

  MyBitset<N> GetContents() {
    log(LOG_TYPE_DEBUG, "Getting contents: (" + BussedItem::createString(_contents.to_ulong()) + ")");
    return _contents;
  }

  MyBitset<N>* GetContentsP() {
    log(LOG_TYPE_DEBUG, "Getting pointer to contents: " + BussedItem::createString(_contents.to_ulong()));
    return _contentsP;
  }

private:
  std::string _name;
  MyBitset<N> _contents;
  MyBitset<N> *_contentsP;

  MyBitset<N> **_inputP;


  bool *_writeEnable;

};

#endif
