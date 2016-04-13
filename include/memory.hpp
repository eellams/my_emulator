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

#ifndef _MEMORY_HPP
#define _MEMORY_HPP

#include "system.hpp"
#include "myBitset.hpp"
#include "bussedItem.hpp"

#include <fstream>
#include <cstring>
#include <cmath>

#define MEMORY_NAME "Main Memory"
#define MEMORY_TYPE_NAME "MEMORY"

class Memory : public BussedItem {
public:
  Memory(std::string name = MEMORY_NAME) : BussedItem(MEMORY_TYPE_NAME, name) {}
  ~Memory() {}

  bool LoadFromFile(std::string fileName, bool isBinary) {
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
          //std::cout << read << std::endl;

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

  //void Update() {}
  void Clock() {}

private:
  MyBitset<BUS_WIDTH> _memory[MEMORY_SIZE];

  MyBitset<BUS_WIDTH> _outputBuffer;

};

 #endif
