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

#include "bussedItem.hpp"

BussedItem::BussedItem(std::string typeName, std::string name) : Item(typeName, name) {

}

BussedItem::~BussedItem() {}

void BussedItem::SetDataBusP(Bus<BUS_WIDTH> *bus) {
  log(LOG_TYPE_DEBUG, "Setting data bus to: " + bus->GetFullName() + ", address: " + createString(static_cast<void*>(bus)));
  _dataBusP = bus;
}

void BussedItem::SetAddressBusP(Bus<BUS_WIDTH> *bus) {
  log(LOG_TYPE_DEBUG, "Setting address bus to: " + bus->GetFullName() + ", address: " + createString(static_cast<void*>(bus)));
  _addressBusP = bus;
}

void BussedItem::SetControlBusP(Bus<BUS_WIDTH> *bus) {
  log(LOG_TYPE_DEBUG, "Setting control bus to: " + bus->GetFullName() + ", address: " + createString(static_cast<void*>(bus)));
  _controlBusP = bus;
}

// Assume all BussedItems should be synchronous
void BussedItem::Clock() {
  log(LOG_TYPE_ERROR, "Unimplemented Clock function");
}
