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
#include <cstring>
#include <cmath>

template<size_t aN, size_t dN, size_t cN>
class Memory : public BussedItem<aN, dN, cN> {
public:
  Memory() : BussedItem<aN, dN, cN>() {};
  ~Memory() {};

  bool LoadFromFile(std::string fileName, bool isBinary) {
    std::ifstream inputFile;
    char read[(size_t)std::ceil(log2(dN))];
    std::bitset<dN> readWord;
    int filesize, expectedFileSize;

    // Open the file, at the end to check file size
    inputFile.open(fileName.c_str(), std::ios::binary | std::ios::ate);
    filesize = inputFile.tellg();

    // Memory size = ( 1 << aN )
    if (dN % BYTE_SIZE != 0) expectedFileSize = (1 << aN) * (dN / BYTE_SIZE + 1);
    else expectedFileSize = (1 << aN) * (dN / BYTE_SIZE);

    if (filesize < expectedFileSize) {
      Singleton<Logger>::GetInstance()->Log(LOG_TYPE_INFO, "Unexpected file size, read " + CreateString(filesize) + " bytes, expected " + CreateString(expectedFileSize) + " bytes, will pad out the rest of data with 0x00");
    } else if (filesize > expectedFileSize) {
      Singleton<Logger>::GetInstance()->Log(LOG_TYPE_INFO, "Unexpected file size, read " + CreateString(filesize) + " bytes, expected " + CreateString(expectedFileSize) + " bytes, some data will be lost!");
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

    Singleton<Logger>::GetInstance()->Log(LOG_TYPE_INFO, "Successfully loaded the file into memory");

    inputFile.close();
    return true;
  }

  void Clock() {
    std::bitset<aN> address;
    std::bitset<dN> data;
    std::bitset<cN> control;

    address = BussedItem<aN, dN, cN>::_addressBus->GetValue();
    data = BussedItem<aN, dN, cN>::_dataBus->GetValue();
    control = BussedItem<aN, dN, cN>::_controlBus->GetValue();

    if (control.test(CONTROL_READ) && !control.test(CONTROL_WRITE)) {
      std::cout << "READ" << std::endl;
    }
    else if (!control.test(CONTROL_READ) && control.test(CONTROL_WRITE)) {
      std::cout << "WRITE" << std::endl;
    }
  }

private:
  std::bitset<dN> _memory[1 << aN];
};

 #endif
