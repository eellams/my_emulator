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
#include "bus.hpp"
#include "singleton.hpp"
#include "logger.hpp"
#include "bussedItem.hpp"

#include <bitset>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cmath>

template<size_t aN, size_t dN>
class Memory : public BussedItem<aN, dN> {
public:
  Memory() : BussedItem<aN, dN>() {};
  ~Memory() {};

  bool LoadFromFile(std::string fileName, bool isBinary) {
    std::ifstream inputFile;
    char read[(size_t)std::ceil(log2(dN))];
    std::bitset<dN> readWord;
    int filesize, memorySize, expectedFileSize;

    std::ostringstream filesizeSS;
    std::ostringstream expectedSS;

    // Open the file, at the end to check file size
    inputFile.open(fileName.c_str(), std::ios::binary | std::ios::ate);
    filesize = inputFile.tellg();

    memorySize = std::pow(2,dN);

    if (dN % BYTE_SIZE != 0) expectedFileSize = memorySize * (dN / BYTE_SIZE + 1);
    else expectedFileSize = memorySize * (dN / BYTE_SIZE);

    filesizeSS << filesize;
    expectedSS << expectedFileSize;

    if (filesize < expectedFileSize) {
      Singleton<Logger>::GetInstance()->Log(LOG_TYPE_INFO, "Unexpected file size, read " + filesizeSS.str() + " bytes, expected " + expectedSS.str() + " bytes, will pad out the rest of data with 0x00");
    } else if (filesize > expectedFileSize) {
      Singleton<Logger>::GetInstance()->Log(LOG_TYPE_INFO, "Unexpected file size, read " + filesizeSS.str() + " bytes, expected " + expectedSS.str() + " bytes, some data will be lost!");
    }

    // Return to the top of the file
    inputFile.clear();
    inputFile.seekg(0, std::ios::beg);

    if (isBinary) {
      for (int i=0; i<expectedFileSize; i++) {
        //inputFile.read((char*)&readWord.data, BYTES_PER_WORD);
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
      }
    } else {
      Singleton<Logger>::GetInstance()->Log(LOG_TYPE_ERROR, "Non-binary files not implemented yet");
      inputFile.close();

      return false;
    }

    /*for (int i=0; i<MEMORY_SIZE; i++) {
      std::cout << "Memory address: " << i << " value: " << (int)_memory[i].data << std::endl;
    }*/

    Singleton<Logger>::GetInstance()->Log(LOG_TYPE_INFO, "Successfully loaded the file into memory");

    inputFile.close();
    return true;
  }

  void Clock() {
    // TODO clock in if the flags permit
  }

private:
  std::bitset<dN> _memory[2^aN];

  bool *_writeEnable; // TODO correct the flags
};

 #endif
