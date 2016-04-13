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

#include <iostream>

#include "singleton.hpp"
#include "logger.hpp"
//#include "register.hpp"
#include "bus.hpp"
#include "myBitset.hpp"

#define TEST_BIT_SIZE 8

/*void testRegWrite(Register<TEST_BIT_SIZE>& reg, std::bitset<TEST_BIT_SIZE>& input, bool enabled) {
  std::bitset<TEST_BIT_SIZE> output, testing;

  // Initial value
  testing = reg.GetContents();

  // Set the value on the input
  reg.SetInput(&input);

  // Check if changed before the clock signal
  if (reg.GetContents() != testing) {
    std::cout << "Register doesn't wait for clock signal" << std::endl;
  }

  // Clock in data
  reg.Clock();

  output = reg.GetContents();

  if (enabled && output != input) {
    std::cout << "Register did not clock in data as expected" << std::endl;
  } else if (!enabled && output != testing) {
    std::cout << "Register clocked in data without correct enable flag" << std::endl;
  }
}*/

void testRegisters() {
  /*std::cout << "Testing register class..." << std::endl;

  std::bitset<TEST_BIT_SIZE> input = 0xff;
  std::bitset<TEST_BIT_SIZE> output, testing;
  bool regEnable = false;
  Register<TEST_BIT_SIZE> testRegister;

  testRegister.SetName("Test Register");

  // Test regEnable
  testRegister.SetWriteEnable(&regEnable);

  std::cout << "Testing register when disabled..." << std::endl;
  testRegWrite(testRegister, input, regEnable);

  // Change values, and check for the other regEnable
  regEnable = true;
  input = 0xaa;
  std::cout << "Testing register when enabled..." << std::endl;
  testRegWrite(testRegister, input, regEnable);*/
}

void testBusses() {
  std::cout << "Testing busses..." << std::endl;

  Bus<TEST_BIT_SIZE> testBus("test Bus");
  MyBitset<TEST_BIT_SIZE> input, output;

  // Set up the bus
  testBus.SetValueP(&input);

  // Set the input value
  input.SetValue(0xff);

  output = *testBus.GetValueP();

  if (input != output) {
    std::cout << "Busses not working correctly" << std::endl;
  }

  // See what happens if we change the input
  input.SetValue(0x88);
  output = *testBus.GetValueP();
  if (output != 0x88) {
    std::cout << "Bus pointers not working properly" << std::endl;
  }
}

int main(int argc, char *argv[]) {
  Logger *log = Singleton<Logger>::GetInstance();

  testRegisters();
  testBusses();

  return 0;
}
