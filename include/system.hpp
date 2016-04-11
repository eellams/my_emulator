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

#define WORD_WIDTH 8
#define MEMORY_SIZE WORD_WIDTH * WORD_WIDTH

// Breakdown of each instructiom
// Bit[7] | Bit[6] ] Bit[5] | Bit[4] | Bit[3] | Bit[2]   | Bit[1]   | Bit[0]
// ---------------------------------------------------------------------------
// Op[3]  | Op[2]  | Op[1]  | Op[0] | R/I flag| RegIm[2] | RegIm[1] | RegIm[0]

#define OPCODE_BITS 4 // Top 4 bits are opcode
#define REG_IMM_FLAG 1 // 1 flag
#define REG_IMM 3 // Bottom 3 bits Reg or Imm

// This is pretty standard these days - don't touch!
#define BYTE_SIZE 8

// NOTE the compiled binary files will probably have wated space
//  i.e. a 9-bit word would occupy 2 bytes, as would a 10-16 bit word width
//  but, a 1-8 bit word width would only require 1 bit

#if WORD_WIDTH % 8 != 0
  #define BYTES_PER_WORD (WORD_WIDTH / BYTE_SIZE + 1)
#else
  #define BYTES_PER_WORD (WORD_WIDTH / BYTE_SIZE)
#endif

#define REQUIRED_FILE_SIZE MEMORY_SIZE * BYTES_PER_WORD

enum Commands {
  JMP = 0x01,
  ADD = 0x02,
  OP3=0x03,
  OP4=0x04,
  OP5=0x05,
  OP6=0x06,
  OP7=0x07,
  OP8=0x08,
  OP9=0x09,
  OP10=0x0A,
  OP11=0x0B,
  OP12=0x0C,
  OP13=0x0D,
  OP14=0x0E,
  OP15=0x0F
};

typedef struct {
  unsigned data : BYTES_PER_WORD * BYTE_SIZE;
} Word;

#endif
