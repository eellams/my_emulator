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

enum unsigned char Commands {
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
}

#endif
