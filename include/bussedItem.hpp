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
#include "item.hpp"
#include "bus.hpp"
#include "myBitset.hpp"

#define BUSSED_ITEM_DEFAULT_NAME "UNKNOWN_ITEM"
#define BUSSED_ITEM_DEFAULT_TYPE_NAME "UNKNOWN_TYPE"

// Forward definiton, avoids cyclic dependencies with "myBitset.hpp"
//template<size_t N> class MyBitset;

class BussedItem : public Item {
public:
  BussedItem(std::string typeName = BUSSED_ITEM_DEFAULT_TYPE_NAME,
    std::string name = BUSSED_ITEM_DEFAULT_NAME);
  ~BussedItem();

  void SetDataBus(Bus<BUS_WIDTH> *bus);
  void SetAddressBus(Bus<BUS_WIDTH> *bus);
  void SetControlBus(Bus<BUS_WIDTH> *bus);

protected:

  // TODO setInput and setOutput?

  Bus<BUS_WIDTH> *_addressBus;
  Bus<BUS_WIDTH> *_dataBus;
  Bus<BUS_WIDTH> *_controlBus;
};

 #endif
