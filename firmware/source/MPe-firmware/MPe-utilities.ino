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

#ifdef TESTING
void testRandom()
{
  randomcurrent = random(0, 50) + random(0, 10) / 10.0;
  randomtemp1 = random(15, 150) + random(0, 10) / 10.0;
  randomtemp2 = random(15, 70) + random(0, 10) / 10.0;
  randomspeedms = random(0, 50);
}
#endif

int valueDelay(int value, int set, int step_up, int step_down, int timer_no) // step_up (or down) means value_unit per second ex. 10V = 10V/s, 10W = 10W/s
{
  unsigned int time = 0;

  if (timer_no == 1)
  {
    time = millis() - timer_value_delay_1;
    timer_value_delay_1 = millis();
  }
  else if (timer_no == 2)
  {
    time = millis() - timer_value_delay_2;
    timer_value_delay_2 = millis();
  }

  if ((set > value))
  {
    value += step_up / (1000 / time);
    if (value > set)
      value = set;
  }
  else if ((set < value))
  {
    value -= step_down / (1000 / time);
    if (value < set)
      value = set;
  }

  return value;
}

int mapConstrain(int value, int fromLow, int fromHigh, int toLow, int toHigh)
{
  value = constrain(value, fromLow, fromHigh);

  return map(value, fromLow, fromHigh, toLow, toHigh);
}

float Convert_mVtoBatVol(int mV)
{
  float ret = 0.0;
  ret = EEPROM.readInt(ADR_VOL_DIV) / 1000000.0 * mV;

  return ret;
}

void set_adc_offset()
{
  unsigned long time = millis() - timer_adc_offset;
  adc_will_save = 1;
  if (time < 5000)
  {
    adc_offset = (adc_offset + ((vRef_filtered.Current() * 500.0) - pin_current_mV_filtered.Current())) / 2.0;
    adc_will_save = 0;
  }

  if (adc_will_save && !adc_saved)
  {
    adc_saved = 1;
    EEPROM.updateFloat(ADR0CURRENT, adc_offset);
  }
}

float Convert_mVtoAmp(float mV)
{
  float ret = 0.0;
  float cur_dir = 1.0;
  if (EEPROM.readInt(ADR_CURDIR))
    cur_dir = -1.0;

  ret = cur_dir * ((vRef_filtered.Current() * 500.0) - eepromLoadFloat(ADR0CURRENT) - mV) / float(EEPROM.readInt(ADR_MVPERA));

  return ret;
}

int acuPercent()
{
  int acupercent = int(((EEPROM.readInt(ADR_BATCAP_AH) * 100.0) - mah_used) / (EEPROM.readInt(ADR_BATCAP_AH) * 100.0) * 100.0);

  if (acupercent < 0)
    acupercent = 0;

  return acupercent;
}

int numberCharges()
{
  return int(total_ah_used / (EEPROM.readInt(ADR_BATCAP_AH) / 10.0));
}

float whkm()
{
  float whkm = Wh_used_trp / trip;
  if (isnan(whkm) || isinf(whkm))
    whkm = 0.0;

  return whkm;
}

int whkm_dtg()
{
  int whkm = int(Wh_used_dtg / trip_dtg);
  if (isinf(whkm))
    whkm = 0;

  return whkm;
}

int distToGo()
{

  float dtg = 0.0;

  dtg = (EEPROM.readInt(ADR_BATCAP_WH) - Wh_used) / whkm_dtg();

  if (trip_dtg > 4.0)
  {
    last_dtg = dtg;
    trip_dtg = 0.0;
    Wh_used_dtg = 0.0;
    saveData();
  }

  if (last_dtg > 0 && trip_dtg < 1.0)
    dtg = last_dtg;

  if (isinf(dtg) || dtg > 999)
    dtg = 999;

  if (dtg < 3)
    dtg = 0;

  return dtg;
}

#ifdef MTG
float whhour()
{
  float whhour = Wh_used_trp / moving_time * 60.0;
  if (isnan(whhour) || isinf(whhour))
    whhour = 0;

  return whhour;
}

float whhour_mtg()
{
  float whhour = Wh_used_mtg / moving_time_mtg * 60.0;
  if (isnan(whhour) || isinf(whhour))
    whhour = 0;

  return whhour;
}

float minutesToGo()
{

  float mtg = 0.0;
  mtg = int((EEPROM.readInt(ADR_BATCAP_WH) - Wh_used) / (whhour_mtg() / 60.0));

  if (moving_time_mtg > 5.0)
  {
    last_mtg = mtg;
    moving_time_mtg = 0.0;
    Wh_used_mtg = 0.0;
    saveData();
  }

  if (last_mtg > 0.0 && moving_time_mtg < 1)
    mtg = last_mtg;

  if (isinf(mtg) || mtg > 999)
    mtg = 999;

  if (mtg < 0)
    mtg = 0;

  return mtg;
}
#endif

void resetCurrentAt0()
{
  timer_adc_offset = millis();
  adc_saved = 0;
}

void resetBattery()
{
#ifndef TESTING
  float vol = 0.0;
  float percent = 0.0;

  vol = getBatRawVoltage();
  int lvc = (EEPROM.readInt(ADR_LVC) / 10.0) + 2;
  percent = ((vol - lvc) / ((EEPROM.readInt(ADR_FULL_BATT_V) / 10.0) - lvc)) + 0.05;

  if (percent > 1.0)
    percent = 1.0;

  if (percent < 0.0)
    percent = 0.0;

  float batWh = float(EEPROM.readInt(ADR_BATCAP_WH));
  float batAh = float(EEPROM.readInt(ADR_BATCAP_AH) * 100.0);

  Wh_used = batWh - (batWh * percent);
  mah_used = batAh - (batAh * percent);
  saveData();
#endif

#ifdef TESTING
  Wh_used = 0.0;
  mah_used = 0.0;
  saveData();
#endif
}

void zeroTrip()
{
  trip = 0.0;
  trip_dtg = 0.0;
  vmax = 0.0;
  moving_time = 0.0;
  Wh_used_trp = 0.0;
  Wh_used_dtg = 0.0;
  last_dtg = 0;
  powermax = 0.0;
  imax = 0.0;

#ifdef MTG
  Wh_used_mtg = 0.0;
  moving_time_mtg = 0.0;
  last_mtg = 0.0;
#endif

  saveData();
}

int getCfgAddress(int address)
{
  if (address == 0 || address > 150)
    address = 200;

  address = 200 + address * 2;

  return address;
}
