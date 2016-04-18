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

template<size_t N>
Bus<N>::Bus(std::string name) : Item(BUS_PREFIX, name) {

}

template<size_t N>
Bus<N>::~Bus() {

};

template<size_t N>
MyBitset<N>* Bus<N>::GetValueP() {
  return _valueP;
}

template<size_t N>
void Bus<N>::SetValueP(MyBitset<N> *value) {
  log(LOG_TYPE_INFO, "Setting to: " + value->GetFullName() + "value: " + createString(value->to_ulong()) );
  _valueP = value;
}

template<size_t N>
void Bus<N>::LogSignals() {
  std::vector<struct Signal> toSend;
  struct Signal toAdd;

  toAdd.Name = GetFullName() + std::string("Bus value");
  toAdd.Value = _valueP->to_ulong();
  toAdd.Address = static_cast<void*>(_valueP);

  toSend.push_back(toAdd);
  sendSignals(toSend);
}
