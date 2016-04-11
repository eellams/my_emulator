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

Memory::Memory() : BussedItem() {

}

Memory::~Memory() {

}

bool Memory::LoadFromFile(std::string fileName, bool isBinary) {
  std::ifstream inputFile;
  char read[BYTES_PER_WORD];
  Word readWord;
  int filesize;

  std::ostringstream filesizeSS;
  std::ostringstream requiredSS;

  // Open the file, at the end to check file size
  inputFile.open(fileName.c_str(), std::ios::binary | std::ios::ate);
  filesize = inputFile.tellg();

  filesizeSS << filesize;
  requiredSS << REQUIRED_FILE_SIZE;

  if (filesize < REQUIRED_FILE_SIZE) {
    Singleton<Logger>::GetInstance()->Log(LOG_TYPE_INFO, "Unexpected file size, read " + filesizeSS.str() + " bytes, expected " + requiredSS.str() + " bytes, will pad out the rest of data with 0x00");
  } else if (filesize > REQUIRED_FILE_SIZE) {
    Singleton<Logger>::GetInstance()->Log(LOG_TYPE_INFO, "Unexpected file size, read " + filesizeSS.str() + " bytes, expected " + requiredSS.str() + " bytes, some data will be lost!");
  }

  // Return to the top of the file
  inputFile.clear();
  inputFile.seekg(0, std::ios::beg);

  if (isBinary) {
    for (int i=0; i<MEMORY_SIZE; i++) {
      //inputFile.read((char*)&readWord.data, BYTES_PER_WORD);
      readWord.data = 0;
      if (!inputFile.eof()) {

        inputFile.read(read, BYTES_PER_WORD);

        // Need to 'flip' the read data, such that the first byte becomes
        //  the last byte
        //  This is due to the files being read [1], [2], ..., [n]
        //   but would otherwise become [n], ..., [2], [1] through a memcpy
        //   I'm guessing it's some endian thing...

        for (int j=0; j<BYTES_PER_WORD; j++) {
          int toShift = (BYTES_PER_WORD - j - 1) * BYTE_SIZE;
          if (toShift >= 0) readWord.data |= read[j] << toShift;
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

// TODO these two, when busses have been written
char Memory::Get(){ return 0; }; // Get word at address
void Memory::Set() {}; // Set word at address with data
