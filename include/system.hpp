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

// This is pretty standard these days - don't touch!
#define BYTE_SIZE 8
// The size of a word in this system
//  currently set to 1 byte
#define WORD_WIDTH 8

// For simplicity of being able to copy bus <=> register, these are the same
#define BUS_WIDTH 16
#define REGISTER_WIDTH 16
#define MEMORY_SIZE 1 << BUS_WIDTH
#define CONTROL_BUS_WDTH 16

#define CONTROL_BUS_MEMORY_WRITE 0
#define CONTROL_BUS_MEMORY_READ 1

// Breakdown of each instructions
//
// Type A: (will end with an I, e.g. ADDI, JMPI)
// Bit[7] | Bit[6] ] Bit[5] | Bit[4] | Bit[3] | Bit[2] | Bit[1] | Bit[0]
// ---------------------------------------------------------------------
// Op[2]  | Op[1]  | Op[0]  | Imm[4] | Imm[3] | Imm[2] | Imm[1] | Imm[0]
//
// Type B: (other instructions)
// Bit[7] | Bit[6] ] Bit[5] | Bit[4] | Bit[3] | Bit[2]   | Bit[1]  | Bit[0]
// -------------------------------------------------------------------------
// Op[2]  | Op[1]  | Op[0]  | Flag   | RegA[1] | RegA[0] | RegB[1] | RegB[0]


// NOTE the compiled binary files will probably have wated space
//  i.e. a 9-bit word would occupy 2 bytes, as would a 10-16 bit word width
//  but, a 1-8 bit word width would only require 1 bit

// Bitmasks for items in memory
#define BITMASK_OP 0xE0 // Top 3 bits
#define BITMASK_OP_WIDTH 3

#define BITMASK_IMM 0x1F // Remaining 5 bits
#define BITMASK_IMM_WIDTH 5

#define BITMASK_REG_A 0x0C
#define BITMASK_REG_A_WIDTH 2
#define BITMASK_REG_B 0x03
#define BITMASK_REG_B_WIDTH 2

#define INSTR_ADDI 0x00 // Add immediate to accumulator
#define INSTR_ADD 0x01 // RegA += RegB
#define INSTR_LOAD 0x02 // Load RegA to address of RegB

// Used for logging
struct Signal {
  std::string Name;
  long Value;
  void *Address;
};

#endif
