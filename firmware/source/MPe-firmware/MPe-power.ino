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

float getBatVoltage()
{
#ifndef TESTING
  return Convert_mVtoBatVol(pin_voltage_mV_filtered.Current());
#endif
#ifdef TESTING
  float spd = 0.0;
  spd = map(acuPercent(), 0, 100, 39.0, 54.6);
  return spd;
#endif
}

float getBatRawVoltage()
{
  return Convert_mVtoBatVol(ads.readADC_SingleEnded(PIN_VOLTAGE) * 0.1875);
}

void checkVRef()
{
  vRef_filtered.Filter(ads.readADC_SingleEnded(PIN_V_REFERENCE) * 0.0001875);
  vRef = vRef_filtered.Current();
}

float getCurrent()
{
  float current = Convert_mVtoAmp(pin_current_mV_filtered.Current());
  if (current < 0.5 && current > -0.5)
    current = 0.00;

#ifndef TESTING
  return current;
#endif

#ifdef TESTING
  return randomcurrent;
#endif
}

void checkCurrentAndVoltage()
{
  unsigned int time = 0;
  time = (millis() - timer_30ms_loop);

  if (time > T30MS_LOOP)
  {
    timer_30ms_loop = millis();

    int pin_voltage_mV = (ads.readADC_SingleEnded(PIN_VOLTAGE) * 0.1875);
    pin_voltage_mV_filtered.Filter(pin_voltage_mV);

    float pin_current_mV = ads.readADC_SingleEnded(PIN_CURRENT) * 0.1875;
    pin_current_mV_filtered.Filter(pin_current_mV);

    float current = getCurrent();
    float pwr = getPower();
    float wh_to_add = 0.0;

#ifndef TESTING
    if (current > imax && millis() > 5000)
    {
      imax = current;
      if (pwr > powermax)
        powermax = pwr;
    }
#endif

#ifdef TESTING

    if (getPower() > powermax)
      powermax = getPower();

    if (getCurrent() > imax)
      imax = getCurrent();
#endif

    mah_used += current * time / 3600.0;

    if (mah_used < 0)
      mah_used = 0;

    // power and whused calculations

    wh_to_add = pwr * time / 3600000.0; // /1000.0/3600.0

    Wh_used += wh_to_add;

    if (Wh_used < 0.0)
      Wh_used = 0.0;

    if (pwr > 0.0)
    {
      total_ah_used += current * time / 3600000.0; // /1000.0/3600.0
      Wh_used_trp += wh_to_add;
      Wh_used_dtg += wh_to_add;
#ifdef MTG
      Wh_used_mtg += wh_to_add;
#endif
    }
  }
}

float getPower()
{
  float pwr = getBatVoltage() * getCurrent();

  return pwr;
}
