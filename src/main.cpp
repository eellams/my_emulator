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
  log->SetLogLevel(LOG_TYPE_DEBUG);
  //log->SetLogLevel(LOG_TYPE_INFO);

  // Busses
  Bus<BUS_WIDTH> addressBus("Address Bus");
  Bus<BUS_WIDTH> dataBus("Data Bus");
  Bus<CONTROL_BUS_WDTH> controlBus("Control Bus");

  // Component parts - will be controlled by sequencer
  RegisterFile registerFile("RegisterFile");
  Sequencer sequencer("Sequencer");
  Memory memory("Main Memory");
  ALU alu("ALU");

  // Get the file we are reading
  std::cout << "File to read (leave blank for testprogram): ";
  std::cin >> std::noskipws >> fileToExecute;

  if (fileToExecute == "") fileToExecute = "testprogram";
  log->Log(LOG_TYPE_INFO, "Opening program: '" + fileToExecute + "'");

  if (!memory.LoadFromFile(fileToExecute)) {
    log->Log(LOG_TYPE_ERROR, "Something went wrong reading the file");
    return 0;
  }

  // Setup connections and Initialise
  registerFile.SetDataBusP(&dataBus);
  registerFile.SetAddressBusP(&addressBus);
  registerFile.SetControlBusP(&controlBus);
  registerFile.Initialise();

  alu.SetDataBusP(&dataBus);
  alu.SetAddressBusP(&addressBus);
  alu.SetControlBusP(&controlBus);
  alu.SetRegisterFileP(&registerFile);

  memory.SetDataBusP(&dataBus);
  memory.SetAddressBusP(&addressBus);
  memory.SetControlBusP(&controlBus);

  // Sequencer controls the program
  sequencer.SetDataBusP(&dataBus);
  sequencer.SetAddressBusP(&addressBus);
  sequencer.SetControlBusP(&controlBus);
  sequencer.SetRegisterFileP(&registerFile);
  sequencer.Initialise();
  sequencer.SetALUP(&alu);
  sequencer.SetMemoryP(&memory);

  //sequencer.LogSignals();
  //log->WriteSignals();

  for (int i=0; i<10; i++) {
    sequencer.Clock();
  }

  /*while (!sequencer.Finished()) {
    sequencer.Clock();
  }*/

  log->Log(LOG_TYPE_INFO, "Program finished executing");

  return 1;
}
