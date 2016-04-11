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

#include "bus.hpp"

Bus::Bus(std::string name) {
  SetName(name);
}

Bus::~Bus() {

}

void Bus::SetName(std::string name) {
  _name = name;
}

std::string Bus::GetName() {
  return _name;
}

void Bus::Set(Word value) {
  std::ostringstream valueSS;
  valueSS << (int)value.data;

  Singleton<Logger>::GetInstance()->Log(LOG_TYPE_DEBUG, "Setting value of bus '" + _name + "' (" + valueSS.str() + ")");
  _contents = value;
};

Word Bus::Get() {
  std::ostringstream contentsSS;
  contentsSS << (int)_contents.data;

  Singleton<Logger>::GetInstance()->Log(LOG_TYPE_DEBUG, "Getting value of bus '" + _name + "' (" + contentsSS.str() + ")");
  return _contents;
};
