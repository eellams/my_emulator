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
#define BUS_WIDTH 8
#define REGISTER_WIDTH 8
#define MEMORY_SIZE 1 << BUS_WIDTH
#define CONTROL_BUS_WDTH 8

#define CONTROL_BUS_MEMORY_WRITE 0
#define CONTROL_BUS_MEMORY_READ 1
#define CONTROL_BUS_ALU_RESET_ACC 2
#define CONTROL_BUS_ALU_ADD 3
#define CONTROL_BUS_ALU_SIGNED 4
#define CONTROL_BUS_ALU_IMM 5
#define CONTROL_BUS_ALU_NAND 6
#define CONTROL_BUS_ALU_IMM_SHORT 7

// Breakdown of each instructions
//
// Type A: (e.g. JMPI, BZ)
// Bit[7] | Bit[6] ] Bit[5] | Bit[4] | Bit[3] | Bit[2] | Bit[1] | Bit[0]
// ---------------------------------------------------------------------
// Op[2]  | Op[1]  | Op[0]  | Imm[4] | Imm[3] | Imm[2] | Imm[1] | Imm[0]
//
// Type B: (other instructions)
// Bit[7] | Bit[6] ] Bit[5] | Bit[4] | Bit[3] | Bit[2]   | Bit[1]  | Bit[0]
// -------------------------------------------------------------------------
// Op[2]  | Op[1]  | Op[0]  | Flag   | RegA[1] | RegA[0] | RegB[1] | RegB[0]
//
// Type C: (ADDI)
// Bit[7] | Bit[6] ] Bit[5] | Bit[4] | Bit[3] | Bit[2]   | Bit[1]  | Bit[0]
// -------------------------------------------------------------------------
// Op[2]  | Op[1]  | Op[0]  | Imm[2]  | Imm[1] | Imm[0] | RegB[1] | RegB[0]


// NOTE the compiled binary files will probably have wated space
//  i.e. a 9-bit word would occupy 2 bytes, as would a 10-16 bit word width
//  but, a 1-8 bit word width would only require 1 bit

// Bitmasks for items in memory
#define BITMASK_OP 0xE0 // Top 3 bits
#define BITMASK_OP_WIDTH 3

#define BITMASK_IMM 0x1F // Remaining 5 bits
#define BITMASK_IMM_WIDTH 5

#define BITMASK_IMM_SHORT 0x1C // 3 bits between command and RegB
#define BITMASK_IMM_SHORT_WIDTH 3

#define BITMASK_REG_A 0x0C
#define BITMASK_REG_A_WIDTH 2 // 2 bits of register
#define BITMASK_REG_B 0x03
#define BITMASK_REG_B_WIDTH 2

// ADD regB, 0 acts to store ACC in regB

//#define INSTR_ADDI   0x00   // Add immediate to accumulator
#define INSTR_ADDI   0x00   // Add immediate to RegB
#define INSTR_ADD    0x01   // Add RegB to RegA (RegA += RegB)
#define INSTR_LOAD   0x02   // Load address at RegB into RegB
#define INSTR_STORE  0x03   // Store RegA at address in RegB
#define INSTR_NAND   0x04   // Set RegA to RegA NAND RegB (Reg = ~(RegA & RegB) )
#define INSTR_JMPI   0x05   // Add immediate to PC
#define INSTR_BZ     0x06   // Jump if ACC is zero by imm
#define INSTR_FINISH 0x07   // End the emulation


// Used for logging
struct Signal {
  std::string Name;
  long Value;
  void *Address;
};

#endif
