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

#ifndef _MEMORY_HPP
#define _MEMORY_HPP

#include "system.hpp"
#include "bus.hpp"
#include "singleton.hpp"
#include "logger.hpp"

#include <fstream>
#include <sstream>
#include <cstring>

class Memory {
public:
  Memory();
  ~Memory();

  bool LoadFromFile(std::string fileName, bool isBinary);

  char Get(); // Get word at address
  void Set(); // Set word at address with data

  void SetDataBus(Bus *bus);
  void SetAddressBus(Bus *bus);

private:
  Word _memory[MEMORY_SIZE];

  Bus *_dataBus;
  Bus *_addressBus;
};

 #endif
