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

#ifndef _LOG_HPP
#define _LOG_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <sstream>

#include "system.hpp"

#define MAX_TIME_STR 80

#define LOG_FILE_LOG "emulator.log"
#define LOG_FILE_SIGNAL "emulation.csv"

#define LOG_STRING_ERROR "ERROR:"
#define LOG_STRING_DEBUG "DEBUG:"
#define LOG_STRING_INFO "INFO:"
#define LOG_STRING_UPDATE "UPDATE:"

#define LOG_TYPE_INFO 0x00
#define LOG_TYPE_DEBUG 0x01
#define LOG_TYPE_ERROR 0x02
#define LOG_TYPE_UPDATE 0x03

class Logger {
public:
  Logger(std::string logFile = LOG_FILE_LOG, std::string signalFile = LOG_FILE_SIGNAL);
  ~Logger();

  bool Clock();

  void Log(char type, std::string toWrite);

  void SendSignals(std::vector<struct Signal> toSend) {
    for (int i=0; i<toSend.size(); i++) {
      _signals.push_back(toSend[i]);
    }
  }

  void WriteSignals() {
    Log(LOG_TYPE_DEBUG, "Writing signals");
    _signalFile << "New set\r\n";

    for(int i=0; i<_signals.size(); i++) {
      _signalFile << std::hex << _signals[i].Name << ": 0x" << _signals[i].Value << ": " << _signals[i].Address << "\r\n";
      _signalFile.flush();
    }

    _signals.clear();

    _signalFile << "End set\r\n";
  }

private:
  std::string createTimeString();
  std::string createFullLogPrefix(std::string prefix);

  void writeToLog(std::string);

  void log(std::string prefix, std::string toWrite);

  std::ofstream _logFile;
  std::ofstream _signalFile;

  std::vector<struct Signal> _signals;

  time_t _rawTime;
  struct tm *_timeInfo;
};

 #endif
