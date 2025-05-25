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

void enable_watchdog()
{
  cli();
  wdt_reset();
  WDTCSR |= B00011000;
  // WDTCSR =  B01001100;   //0.25s
  // WDTCSR =  B01001101;   //0.5s
  WDTCSR = B01001110; // 1.0s
  sei();
}

// WATCHDOG Interrupt
ISR(WDT_vect)
{
  EEPROM.updateInt(ADR_WATCHDOGRESET, 1);
  saveData();
}
