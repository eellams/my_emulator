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

#include "memory.hpp"

Memory::Memory(std::string name) : BussedItem(MEMORY_TYPE_NAME, name) {
  _outputBuffer.SetParent(this);
  _outputBuffer.SetName("Memory Output Buffer");

  _output = &_outputBuffer;
};

Memory::~Memory() {};

bool Memory::LoadFromFile(std::string fileName, bool isBinary) {
  std::ifstream inputFile;
  char read[(size_t)std::ceil( 1 << BUS_WIDTH )];
  MyBitset<BUS_WIDTH> readWord(this, "MEMORY_CELL");
  int filesize, expectedFileSize, numberCharactersPerOpcode;

  // Open the file, at the end to check file size
  inputFile.open(fileName.c_str(), std::ios::binary | std::ios::ate);
  filesize = inputFile.tellg();

  if (WORD_WIDTH % BYTE_SIZE != 0) numberCharactersPerOpcode = (WORD_WIDTH / BYTE_SIZE + 1);
  else numberCharactersPerOpcode = (WORD_WIDTH / BYTE_SIZE);

  expectedFileSize = numberCharactersPerOpcode * MEMORY_SIZE;

  if (filesize < expectedFileSize) {
    log(LOG_TYPE_INFO, "Unexpected file size, read " + BussedItem::createString(filesize) +
      " bytes, expected " + BussedItem::createString(expectedFileSize) + " bytes, will pad out the rest of data with 0x00");
  } else if (filesize > expectedFileSize) {
    log(LOG_TYPE_INFO, "Unexpected file size, read " + BussedItem::createString(filesize) +
      " bytes, expected " + BussedItem::createString(expectedFileSize) + " bytes, some data will be lost!");
  }

  // Return to the top of the file
  inputFile.clear();
  inputFile.seekg(0, std::ios::beg);

  if (isBinary) {
    for (int i=0; i<expectedFileSize; i++) {
      readWord = 0;
      if (!inputFile.eof()) {

        inputFile.read(read, sizeof(read));

        // Need to 'flip' the read data, such that the first byte becomes
        //  the last byte
        //  This is due to the files being read [1], [2], ..., [n]
        //   but would otherwise become [n], ..., [2], [1] through a memcpy
        //   I'm guessing it's some endian thing...

        for (int j=0; j<sizeof(read); j++) {
          int toShift = (sizeof(read) - j - 1) * BYTE_SIZE;
          if (toShift >= 0) readWord |= read[j] << toShift;
        }
      }
      _memory[i] = readWord;
      _memory[i].SetParent(this);
      _memory[i].SetName("Memory Cell: " + createString(i));
    }
  } else {
    log(LOG_TYPE_ERROR, "Non-binary files not implemented yet");
    inputFile.close();

    return false;
  }

  log(LOG_TYPE_INFO, "Successfully loaded the file into memory");

  inputFile.close();
  return true;
}

void Memory::Clock() {
  //TODO work this out
  log(LOG_TYPE_DEBUG, "Clock - not fully implemented");
  MyBitset<BUS_WIDTH> **address;
  MyBitset<BUS_WIDTH> **data;
  MyBitset<BUS_WIDTH> **control;

  address = BussedItem::_addressBus->GetValue();
  data = BussedItem::_dataBus->GetValue();
  control = BussedItem::_controlBus->GetValue();

  if ((*address)->to_ulong() < MEMORY_SIZE){
    if ((*control)->test(CONTROL_READ) && !(*control)->test(CONTROL_WRITE)) {
      // Change the output regardless
      _outputBuffer = _memory[(*address)->to_ulong()];

      // Read to one of the two busses depending on the control flag
      if ((*control)->test(CONTROL_WHICH_BUS)) {
        // Address bus
        log(LOG_TYPE_INFO, "Reading memory address: " + createString((*address)->to_ulong()) + " value: " + createString(_outputBuffer.to_ulong()) + " to address bus");
        _addressBus->SetInput(_output);
      }
      else {
        // Data bus
        log(LOG_TYPE_INFO, "Reading memory address: " + createString((*address)->to_ulong()) + " value: " + createString(_outputBuffer.to_ulong()) + " to data bus");
        _dataBus->SetInput(_output);
      }
    }

    else if (!(*control)->test(CONTROL_READ) && (*control)->test(CONTROL_WRITE)) {
      log(LOG_TYPE_ERROR, "Writing to memory not implemented yet");
      // TODO implement writing
    }
    else if ((*control)->test(CONTROL_READ) && (*control)->test(CONTROL_WRITE)) {
      log(LOG_TYPE_ERROR, "Both reading and writing to memory: " + createString((*address)->to_ulong()));
    }
    else log(LOG_TYPE_INFO, "Nothing to do here");
  }
  else {
    log(LOG_TYPE_ERROR, "Memory address out of range");
  }

}
