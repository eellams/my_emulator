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

class BussedItem : public Item {
public:
  BussedItem(std::string typeName = BUSSED_ITEM_DEFAULT_TYPE_NAME,
    std::string name = BUSSED_ITEM_DEFAULT_NAME);

  ~BussedItem();

  // Ser pointers to the busses
  void SetDataBusP(Bus<BUS_WIDTH> *bus);
  void SetAddressBusP(Bus<BUS_WIDTH> *bus);
  void SetControlBusP(Bus<CONTROL_BUS_WDTH> *bus);

  // Assume all BussedItems should be synchronous
  //  so give an error if no clock defined
  virtual void Clock();

// Will be the parent class to lots of other classes
//  so need to make this protected (inheritable)
protected:
  Bus<BUS_WIDTH> *_addressBusP;
  Bus<BUS_WIDTH> *_dataBusP;
  Bus<CONTROL_BUS_WDTH> *_controlBusP ;
};

 #endif
