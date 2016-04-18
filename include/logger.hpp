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
#define LOG_FILE_SIGNAL "emulation_signals.log"
#define LOG_FILE_SIGNAL_CSV "emulation_signals.csv"

#define LOG_STRING_ERROR "ERROR:"
#define LOG_STRING_DEBUG "DEBUG:"
#define LOG_STRING_INFO "INFO:"
#define LOG_STRING_UPDATE "UPDATE:"

#define LOG_TYPE_ERROR 0
#define LOG_TYPE_INFO 1
#define LOG_TYPE_DEBUG 2
#define LOG_TYPE_UPDATE 3

class Logger {
public:
  Logger(std::string logFile = LOG_FILE_LOG, std::string signalFile = LOG_FILE_SIGNAL, std::string signalFileCSV = LOG_FILE_SIGNAL_CSV);
  ~Logger();

  // Set the log level - all above level number
  void SetLogLevel(int level) { _level = level; }

  // Log a string
  void Log(char type, std::string toWrite);

  // Add signals to be written
  void SendSignals(std::vector<struct Signal> toSend);

  // Write all the queued signal to file
  //  should always be sent in the same order, so will also be written in the
  //  same order
  void WriteSignals();

private:
  // Create a time string (not too useful for this application)
  std::string createTimeString();

  // Creates a log prefix
  std::string createFullLogPrefix(std::string prefix);

  // Write to log fie
  void writeToLog(std::string);

  // Wrie a log string
  void log(std::string prefix, std::string toWrite);

  std::ofstream _logFile;
  std::ofstream _signalFile;
  std::ofstream _signalFileCSV;

  std::vector<struct Signal> _signals;

  int _level;
  bool _firstTimeWriteSignals;

  time_t _rawTime;
  struct tm *_timeInfo;
};

 #endif
