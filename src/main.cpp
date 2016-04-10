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
#include "inputParser.hpp"
#include "singleton.hpp"

bool do_clock() {
  Singleton<Logger>::GetInstance()->Clock();
  return true;
}

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
  InputParser *iparser = Singleton<InputParser>::GetInstance();

  // Get the file we are reading
  std::cout << "File to read (leave blank for testprogram): ";
  std::cin >> std::noskipws >> fileToExecute;

  std::cin.clear();
  std::cin >> std::skipws;

  std::cout << std::endl << "File is binary? ";
  std::cin >> isBinary;

  if (fileToExecute == "") fileToExecute = "testprogram";
  log->Log(LOG_TYPE_INFO, "Opening program: '" + fileToExecute);

  iparser->ReadFile(fileToExecute, isBinary);

  do_clock();

  log->Log(LOG_TYPE_INFO, "Program finished executing");
  return 0;
}
