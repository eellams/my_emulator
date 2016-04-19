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
  _read = _write = false;

  _output.SetParent(this);
  _output.SetName("Memory output");
}
Memory::~Memory() {}

bool Memory::LoadFromFile(std::string fileName, bool isBinary) {
  std::ifstream inputFile;
  char read[8]; //TODO this should vary
  MyBitset<BUS_WIDTH> readWord(this, "Reading");
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
      //if (!inputFile.eof()) {
      if (i<=filesize) {

        inputFile.read(read, numberCharactersPerOpcode);

        // Need to 'flip' the read data, such that the first byte becomes
        //  the last byte
        //  This is due to the files being read [1], [2], ..., [n]
        //   but would otherwise become [n], ..., [2], [1] through a memcpy
        //   I'm guessing it's some endian thing...

        for (int j=0; j<numberCharactersPerOpcode; j++) {
          int toShift = (numberCharactersPerOpcode - j - 1) * BYTE_SIZE;
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
  log(LOG_TYPE_DEBUG, "Clock");
  Update();

  MyBitset<CONTROL_BUS_WDTH> controlValue;

  controlValue = *(_controlBusP->GetValueP());

  if (controlValue.test(CONTROL_BUS_MEMORY_READ)) {
    _output.SetValue(_memory[_addressBusP->GetValueP()->to_ulong()]); // Copy value to output

    log(LOG_TYPE_INFO, "Reading from memory at address: " + createString(_addressBusP->GetValueP()->to_ulong()) + " value: " + createString(_output.to_ulong()));
    _dataBusP->SetValueP(&_output);
  }

  if (controlValue.test(CONTROL_BUS_MEMORY_WRITE)) {
    log(LOG_TYPE_INFO, "Writing to memory at address: " + createString(_addressBusP->GetValueP()->to_ulong()));
    _memory[_addressBusP->GetValueP()->to_ulong()] = *_dataBusP->GetValueP();
  }

  _read = _write = false;

  Update();
}

// No memory, nothing to update?
void Memory::Update() {
  log(LOG_TYPE_UPDATE, "Update [EMPTY]");
};

// Nothing to log?
void Memory::LogSignals() {
  std::vector<struct Signal> toSend;
  struct Signal toAdd;

  toAdd.Name = GetFullName  () + _output.GetName();
  toAdd.Value = _output.to_ulong();
  toAdd.Address = static_cast<void*>(&_output);

  toSend.push_back(toAdd);
  sendSignals(toSend);
}
