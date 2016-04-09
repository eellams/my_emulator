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

#ifndef _SINGLETON_HPP
#define _SINGLETON_HPP

template<typename T>
class Singleton {
public:
  Singleton() { };
  ~Singleton() {};

  static T* GetInstance() {
    if (!_instance) _instance = new T();

    return _instance;
  }

protected:
  static T* _instance;

};

template<typename T>
T* Singleton<T>::_instance = 0;

 #endif
