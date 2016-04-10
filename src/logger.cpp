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

#include "logger.hpp"

Logger::Logger(std::string logFile, std::string signalFile) {

  // Set up the time
  time(&_rawTime);
  _timeInfo = localtime(&_rawTime);

  std::cout << "Starting logger with log file: '" << logFile << "' ";
  std::cout << "and signal file: '" << signalFile << "'"<< std::endl;

}

Logger::~Logger() {
  delete _timeInfo;
  _logfile.close();
}

bool Logger::Clock() {
  Log(LOG_TYPE_INFO, "Logger clock");
  return true;
}

void Logger::Log(char type, std::string toWrite) {
  switch(type) {
    case LOG_TYPE_ERROR:
      log(LOG_STRING_ERROR, toWrite);
      break;
    case LOG_TYPE_DEBUG:
    log(LOG_STRING_DEBUG, toWrite);
      break;
    case LOG_TYPE_INFO:
      log(LOG_STRING_INFO, toWrite);
      break;
  }
}

std::string Logger::createTimeString() {
  char buffer[MAX_TIME_STR];

  strftime(buffer,80,"%d-%m-%Y %I:%M:%S", _timeInfo);
  std::string str(buffer);

  str = "[" + str + "]";

  return str;
}

std::string Logger::createFullLogPrefix(std::string prefix) {
  std::string toReturn;
  toReturn += prefix;

  toReturn += createTimeString();

  return toReturn;
}

void Logger::log(std::string prefix, std::string toWrite) {
  std::string logString;

  logString += createFullLogPrefix(prefix);
  logString += ": " + toWrite;

  std::cout << logString << std::endl;
}