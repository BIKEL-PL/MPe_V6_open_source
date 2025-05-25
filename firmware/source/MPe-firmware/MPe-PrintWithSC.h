/*
 * This file is part of MPe-firmware.
 *
 * MPe-firmware is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MPe-firmware is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MPe-firmware.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Copyright (C) 2022 Marek Przybylak
 */

// Extends Print class with printsc functions (print with semicolon)
#ifndef PRINT_WITH_SC_H
#define PRINT_WITH_SC_H

#include <Arduino.h>

class PrintWithSC : public Print
{
  Print &base;

public:
  PrintWithSC(Print &p) : base(p) {}

  virtual size_t write(uint8_t b) override
  {
    return base.write(b);
  }

  size_t printsc(const String &s)
  {
    size_t n = base.print(s);
    return n + base.print(";");
  }
  size_t printsc(const char c[])
  {
    size_t n = base.print(c);
    return n + base.print(";");
  }
  size_t printsc(char c)
  {
    size_t n = base.print(c);
    return n + base.print(";");
  }
  size_t printsc(unsigned char b, int baseVal = DEC)
  {
    size_t n = base.print(b, baseVal);
    return n + base.print(";");
  }
  size_t printsc(int num, int baseVal = DEC)
  {
    size_t n = base.print(num, baseVal);
    return n + base.print(";");
  }
  size_t printsc(unsigned int num, int baseVal = DEC)
  {
    size_t n = base.print(num, baseVal);
    return n + base.print(";");
  }
  size_t printsc(long num, int baseVal = DEC)
  {
    size_t n = base.print(num, baseVal);
    return n + base.print(";");
  }
  size_t printsc(unsigned long num, int baseVal = DEC)
  {
    size_t n = base.print(num, baseVal);
    return n + base.print(";");
  }
  size_t printsc(double num, int digits = 2)
  {
    size_t n = base.print(num, digits);
    return n + base.print(";");
  }
  size_t printsc(const Printable &x)
  {
    size_t n = base.print(x);
    return n + base.print(";");
  }
};

#endif
