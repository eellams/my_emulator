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

#ifndef _BUSSED_ITEM_HPP
#define _BUSSED_ITEM_HPP

#include <string>
#include <sstream>

#include "system.hpp"
#include "bus.hpp"

#define BUS_DEFAULT_NAME "UNKNOWN_BUS"

// As the width of data and address bus could differ
template<size_t aN, size_t dN, size_t cN>
class BussedItem {
public:
  BussedItem() {}
  ~BussedItem() {}

  void SetDataBus(Bus<dN> *bus) { _dataBus = bus; }
  void SetAddressBus(Bus<aN> *bus) { _addressBus = bus; }
  void SetControlBus(Bus<cN> *bus) { _controlBus = bus; }

protected:
  Bus<aN> *_addressBus;
  Bus<dN> *_dataBus;
  Bus<cN> *_controlBus;
};

 #endif
