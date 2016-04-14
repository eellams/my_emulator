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

#include <iostream>
#include <fstream>
#include <string>

#include "logger.hpp"
#include "singleton.hpp"
#include "bus.hpp"
#include "memory.hpp"
#include "sequencer.hpp"
#include "register.hpp"
#include "registerFile.hpp"
#include "alu.hpp"

int main(int argc, char *argv[]) {
  std::string fileToExecute;
  bool isBinary = true;

  // Startup all of the singleton instances
  Logger *log = Singleton<Logger>::GetInstance();

  MyBitset<BUS_WIDTH> zeros(0, "ZEROS");

  Bus<BUS_WIDTH> addressBus("Address Bus");
  Bus<BUS_WIDTH> dataBus("Data Bus");
  Bus<BUS_WIDTH> controlBus("Control Bus");

  RegisterFile registerFile("RegisterFile");
  Sequencer sequencer("Sequencer");
  Memory memory("Main Memory");
  ALU alu("ALU");

  // Get the file we are reading
  std::cout << "File to read (leave blank for testprogram): ";
  std::cin >> std::noskipws >> fileToExecute;

  std::cin.clear();
  std::cin >> std::skipws;

  std::cout << std::endl << "File is binary? ";
  std::cin >> isBinary;

  if (fileToExecute == "") fileToExecute = "testprogram";
  log->Log(LOG_TYPE_INFO, "Opening program: '" + fileToExecute + "'");

  if (!memory.LoadFromFile(fileToExecute, isBinary)) {
    log->Log(LOG_TYPE_ERROR, "Something went wrong reading the file");
    return 0;
  }

  dataBus.SetValueP(&zeros);
  addressBus.SetValueP(&zeros);
  controlBus.SetValueP(&zeros);

  registerFile.SetDataBusP(&dataBus);
  registerFile.SetAddressBusP(&addressBus);
  registerFile.SetControlBusP(&controlBus);
  registerFile.SetupRegisters();

  alu.SetDataBusP(&dataBus);
  alu.SetAddressBusP(&addressBus);
  alu.SetControlBusP(&controlBus);
  alu.SetRegisterFileP(&registerFile);

  // Setup our connections
  memory.SetDataBusP(&dataBus);
  memory.SetAddressBusP(&addressBus);
  memory.SetControlBusP(&controlBus);

  sequencer.SetDataBusP(&dataBus);
  sequencer.SetAddressBusP(&addressBus);
  sequencer.SetControlBusP(&controlBus);
  sequencer.SetRegisterFileP(&registerFile);
  sequencer.SetALUP(&alu);
  sequencer.SetMemoryP(&memory);

  //while(!sequencer.Finished()) sequencer.Clock();
  for (int i=0; i<10; i++) {
    sequencer.Clock();
  }

  log->Log(LOG_TYPE_INFO, "Program finished executing");

  return 1;
}
