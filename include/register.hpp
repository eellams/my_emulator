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

  Register(std::string name = REGISTER_DEFAULT_NAME);
  ~Register();

  // Clock in the input (if enabled)
  void Clock();

  void SetWriteEnableP(bool *value);

  void SetInputP(MyBitset<N> *inputP);

  // Return the pointer to the output
  //  useful for setting busses
  MyBitset<N>* GetOutputP();

  // Nothing to update
  //  as the input is updated within RegisterFile::Update
  void Update();

  virtual void LogSignals();

private:
  MyBitset<N> *_inputP;
  MyBitset<N> _output;

  bool *_writeEnableP;

};

#include "register.tpp"

#endif
