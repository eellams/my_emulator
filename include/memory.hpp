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

#include "bus.hpp"
#include "singleton.hpp"
#include "logger.hpp"

#include <fstream>
#include <sstream>
#include <cstring>

// This is pretty standard these days - don't touch!
#define BYTE_SIZE 8

#ifndef MEMORY_SIZE
  #define MEMORY_SIZE 8*8
#endif

#ifndef WORD_WIDTH
  #define WORD_WIDTH 8
#endif

// NOTE the compiled binary files will probably have wated space
//  i.e. a 9-bit word would occupy 2 bytes, as would a 10-16 bit word width
//  but, a 1-8 bit word width would only require 1 bit

#if WORD_WIDTH % 8 != 0
  #define BYTES_PER_WORD (WORD_WIDTH / BYTE_SIZE + 1)
#else
  #define BYTES_PER_WORD (WORD_WIDTH / BYTE_SIZE)
#endif

#define REQUIRED_FILE_SIZE MEMORY_SIZE * BYTES_PER_WORD

// Custom N-bit type
/*typedef struct {
    unsigned data: WORD_WIDTH;
} Word;*/

typedef struct {
  unsigned data : BYTES_PER_WORD * BYTE_SIZE;
  //char data[BYTES_PER_WORD];
  //bool data[WORD_WIDTH];
} Word;

class Memory {
public:
  Memory(Bus *dataBus, Bus *addressBus);
  ~Memory();

  bool LoadFromFile(std::string fileName, bool isBinary);

  char Get(); // Get word at address
  void Set(); // Set word at address with data

private:

  Word _memory[MEMORY_SIZE];

  Bus *_dataBus;
  Bus *_addressBus;
};

 #endif
