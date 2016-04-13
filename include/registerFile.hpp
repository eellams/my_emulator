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

#ifndef _REGISTER_FILE_HPP
#define _REGISTER_FILE_HPP

#include <string>

#include "system.hpp"
#include "bussedItem.hpp"
#include "register.hpp"

#define REG_NUM 1 << REG_WIDTH
#define REG_GEN_NAME "Registers" // Prefix used for generating names

// Reserved registers
#define REG_ZERO 0
#define REG_ACC 1
#define REG_MAR 2
#define REG_MDR 3
#define REG_PC 4
#define REG_CIR 5

#define REG_ZERO_NAME "Zero Reg"
#define REG_ACC_NAME "Accumulator"
#define REG_MAR_NAME "Memory Address Register"
#define REG_MDR_NAME "Memory Data Register"
#define REG_PC_NAME "Program Counter"
#define REG_CIR_NAME "Current Instruction Register"

#define REG_FILE_NAME "Register File"
#define REG_FILE_TYPE_NAME "REGISTER_FILE"

class RegisterFile : public BussedItem {
public:
  RegisterFile(std::string name = REG_FILE_NAME) : BussedItem(REG_FILE_TYPE_NAME, name) {
    _eACC = _ePC = _eCIR = false;
  }

  ~RegisterFile() {}

  void SetupRegisters() {
    _ACC.SetName("Accumulator");
    _PC.SetName("Program Counter");
    _CIR.SetName("Current Instruction Register");

    _ACC.SetWriteEnableP(&_eACC);
    SetACCP(_dataBusP->GetValueP());

    _PC.SetWriteEnableP(&_ePC);
    SetPCP(_dataBusP->GetValueP());

    _CIR.SetWriteEnableP(&_eCIR);
    SetCIRP(_dataBusP->GetValueP());
  }

  MyBitset<REGISTER_WIDTH>* GetACCP() { return _ACC.GetOutputP(); }
  void SetACCP(MyBitset<REGISTER_WIDTH> *value) {
    _ACC.SetInputP(value);
    _eACC = true;
  }

  MyBitset<REGISTER_WIDTH>* GetPCP() { return _PC.GetOutputP(); }
  void SetPCP(MyBitset<REGISTER_WIDTH> *value) {
    _PC.SetInputP(value);
    _ePC = true;
  }

  MyBitset<REGISTER_WIDTH>* GetCIRP() { return _CIR.GetOutputP(); }
  void SetCIRP(MyBitset<REGISTER_WIDTH> *value) {
    _CIR.SetInputP(value);
    _eCIR = true;
  }

  void Clock() {
    Update();
    
    _ACC.Clock();
    _PC.Clock();
    _CIR.Clock();

    _eACC = false;
    _ePC = false;
    _eCIR = false;
  }

  void Update() {
    log(LOG_TYPE_DEBUG, "Updating values");
    SetACCP(_dataBusP->GetValueP());
    SetPCP(_dataBusP->GetValueP());
    SetCIRP(_dataBusP->GetValueP());
  }

private:
  Register<REGISTER_WIDTH> _ACC, _PC, _CIR;

  bool _eACC, _ePC, _eCIR;
};

#endif
