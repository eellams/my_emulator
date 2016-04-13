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

#ifndef _BUS_HPP
#define _BUS_HPP

#include <string>
#include <sstream>
#include <stdexcept>

#include "system.hpp"
#include "singleton.hpp"
#include "logger.hpp"
#include "item.hpp"
#include "myBitset.hpp"

//template<size_t N> class MyBitset;

#define BUS_DEFAULT_NAME "UNKNOWN_BUS"
#define BUS_PREFIX "BUS"

// TODO contention checking?
//  i.e. if input changed more than once per clock cycle

template<size_t N>
class Bus : public Item {
public:
  Bus(std::string name = BUS_DEFAULT_NAME) : Item(BUS_PREFIX, name) {
    SetName(name);
  }
  ~Bus() {};

  MyBitset<N>** GetValue() { return &_input; }

  void SetInput(MyBitset<BUS_WIDTH> *input) {
    log(LOG_TYPE_DEBUG, "Setting bus input to: " + input->GetDetails() + "value: " + createString(input->to_ulong()) );
    Item::SetInput(input);
  }
};

 #endif
