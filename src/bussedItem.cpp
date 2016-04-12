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


BussedItem::BussedItem(std::string typeName, std::string name) {
    SetTypeName(typeName);
    SetName(name);
  }

BussedItem::~BussedItem() {
}

void BussedItem::SetDataBus(Bus<DATA_WIDTH> *bus) {
  _dataBus = bus;
}
void BussedItem::SetAddressBus(Bus<ADDRESS_WIDTH> *bus) {
  _addressBus = bus;
}
void BussedItem::SetControlBus(Bus<CONTROL_WIDTH> *bus) {
  _controlBus = bus;
}

void BussedItem::SetName(std::string name) {
  if (_name != BUSSED_ITEM_DEFAULT_NAME && _name != "")
    log(LOG_TYPE_DEBUG, "Changing name from: '" + _name + "' to: '" + name + "'");
  _name = name;
}
std::string BussedItem::GetName() {
  return _name;
}

void BussedItem::SetTypeName(std::string typeName) {
  _typeName = typeName;
}
std::string BussedItem::GetTypeName() {
  return _typeName;
}

std::string BussedItem::createLogPrefix() {
    std::string toReturn;
    toReturn = "[" + GetTypeName() + ": " + GetName() + "] ";
    return toReturn;
}

void BussedItem::log(int logType, std::string logStr) {
  Singleton<Logger>::GetInstance()->Log(logType, createLogPrefix() + logStr);
}
