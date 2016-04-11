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

class meh {
public:
  meh() {std::cout << "meh" << std::endl;};
  int x;
};

int main(int argc, char *argv[]) {
  std::string fileToExecute;
  bool isBinary = true;

  // Startup all of the singleton instances
  Logger *log = Singleton<Logger>::GetInstance();

  // Classes that we are going to use
  Bus<ADDRESS_WIDTH> addressBus;
  Bus<DATA_WIDTH> dataBus;
  Bus<CONTROL_WIDTH> controlBus;
  Sequencer<ADDRESS_WIDTH, DATA_WIDTH, CONTROL_WIDTH> sequencer;
  Memory<ADDRESS_WIDTH, DATA_WIDTH, CONTROL_WIDTH> memory;

  // Set bus names
  dataBus.SetName("Data Bus");
  addressBus.SetName("Address Bus");
  controlBus.SetName("Control Bus");

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

  // Setup our connections
  memory.SetDataBus(&dataBus);
  memory.SetAddressBus(&addressBus);
  memory.SetControlBus(&controlBus);

  sequencer.SetDataBus(&dataBus);
  sequencer.SetAddressBus(&addressBus);
  sequencer.SetControlBus(&controlBus);
  sequencer.SetupControlConnections(); // Sequencer controls control bus

  sequencer.FirstClock();

  log->Log(LOG_TYPE_INFO, "Program finished executing");

  return 1;
}
