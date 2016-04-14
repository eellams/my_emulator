/*    Tests that need to be performed
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

#ifndef _SYSTEM_HPP
#define _SYSTEM_HPP

#include <string>
#include <sstream>
#include <bitset>

#include <vector>
#include <utility>

#define WORD_WIDTH 8

//#define DATA_WIDTH 16
//#define ADDRESS_WIDTH 16

#define BUS_WIDTH 16
#define MEMORY_SIZE 1 << BUS_WIDTH

//#define CONTROL_WIDTH 4



#define REGISRER_NUMBER_WIDTH 3 // Must be the same as BITS_REG
#define REGISTER_NUMBER (1 << REGISRER_NUMBER_WIDTH)
#define REGISTER_WIDTH 16

//#define REG_WIDTH 5 // Must match BITS_REG

#define CONTROL_READ 0
#define CONTROL_WRITE 1
#define CONTROL_CLOCK 2 // Never actually used in this simulation
#define CONTROL_WHICH_BUS 3

// Breakdown of each instructiom
// Bit[7] | Bit[6] ] Bit[5] | Bit[4] | Bit[3] | Bit[2]   | Bit[1]   | Bit[0]
// ---------------------------------------------------------------------------
// Op[3]  | Op[2]  | Op[1]  | Op[0] | R/I flag| RegIm[2] | RegIm[1] | RegIm[0]


// This is pretty standard these days - don't touch!
#define BYTE_SIZE 8

// NOTE the compiled binary files will probably have wated space
//  i.e. a 9-bit word would occupy 2 bytes, as would a 10-16 bit word width
//  but, a 1-8 bit word width would only require 1 bit

#define BITS_OP 0xE0
#define BITS_IMM 0x1F

#define INSTR_ADD 0x00

struct Signal {
  std::string Name;
  long Value;
  void *Address;
};

#endif
