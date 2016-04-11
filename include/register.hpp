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
#include "bus.hpp"
#include "singleton.hpp"
#include "logger.hpp"

class Register : public BussedItem {
public:
  Register() : BussedItem() {}
  ~Register() {}



  void Clock() {}

/*private:
  Word _value;
  bool *_isEnabled;*/
};

class GeneralRegisterDeMux : public BussedItem {
public:
  GeneralRegisterDeMux() : BussedItem() {

  }
  ~GeneralRegisterDeMux() {}

private:
  // The bottom bits of the CIR indicate the register to write to
  //  TODO should this be a bus rather than a weird link?
  Register *_CIR;

  //Register _registers[8]; // TODO this shouldn't be hard-coded
  //bool _registerEnable[8];
};

#endif
