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

Logger::Logger(std::string logFile, std::string signalFile, std::string signalFileCSV) {

  _firstTimeWriteSignals = true;

  // Set up the time
  time(&_rawTime);
  _timeInfo = localtime(&_rawTime);

  std::cout << "Starting logger with log file: '" << logFile << "' ";
  std::cout << "and signal file: '" << signalFile << "'"<< std::endl;

  _logFile.open(logFile.c_str());
  _signalFile.open(signalFile.c_str());
  _signalFileCSV.open(signalFileCSV.c_str());

  SetLogLevel(LOG_TYPE_INFO);

}

Logger::~Logger() {
  delete _timeInfo;

  _logFile.close();
  _signalFile.close();
  _signalFileCSV.close();
}

void Logger::Log(char type, std::string toWrite) {
  switch(type) {
    case LOG_TYPE_ERROR:
      if (_level >= LOG_TYPE_ERROR) log(LOG_STRING_ERROR, toWrite);
      break;
    case LOG_TYPE_DEBUG:
      if (_level >= LOG_TYPE_DEBUG) log(LOG_STRING_DEBUG, toWrite);
      break;
    case LOG_TYPE_INFO:
      if (_level >= LOG_TYPE_INFO) log(LOG_STRING_INFO, toWrite);
      break;

    case LOG_TYPE_UPDATE:
      if (_level >= LOG_TYPE_UPDATE) log(LOG_STRING_UPDATE, toWrite);
      break;
  }
}

void Logger::SendSignals(std::vector<struct Signal> toSend) {
  for (size_t i=0; i<toSend.size(); i++) {
    _signals.push_back(toSend[i]);
  }
}

void Logger::WriteSignals() {
  Log(LOG_TYPE_DEBUG, "Writing signals");
  _signalFile << "New set\r\n";

  for(size_t i=0; i<_signals.size(); i++) {
    _signalFile << std::hex << _signals[i].Name << ": 0x" << _signals[i].Value << ": " << _signals[i].Address << "\r\n";
    _signalFile.flush();
  }

  _signalFile << "End set\r\n";

  if (_firstTimeWriteSignals) {
    for (size_t i=0; i<_signals.size(); i++) {
      _signalFileCSV << _signals[i].Name;
      if (i != _signals.size() - 1) {
        _signalFileCSV << ",";
      } else {
        _signalFileCSV << "\r\n";
      }
      _signalFileCSV.flush();
    }
    _firstTimeWriteSignals = false;
  }

  for (size_t i=0; i<_signals.size(); i++) {
    _signalFileCSV << _signals[i].Value;
    if (i != _signals.size() - 1) {
      _signalFileCSV << ",";
    } else {
      _signalFileCSV << "\r\n";
    }
    _signalFileCSV.flush();
  }

  _signals.clear();
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

void Logger::writeToLog(std::string toWrite) {
  // Write to the log file, if possible
  if (_logFile.is_open()) {
    _logFile.write(toWrite.c_str(), toWrite.size());

    // Flush the output, keeping the log up to date
    //  note that this is not the most efficidnt way to do this
    //  but this program isn't time critical, so should be okay
    _logFile.flush();
  } else {
    Log(LOG_TYPE_ERROR, "Couldn't write to log file!");
  }
}

void Logger::log(std::string prefix, std::string toWrite) {
  std::string logString;

  logString += createFullLogPrefix(prefix);
  logString += ": " + toWrite + "\r\n";

  writeToLog(logString);
  std::cout << logString;
}
