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

void checkIfStopped()
{
  int time = 2500;
  if ((EEPROM.readInt(ADR_MOT_MAG) > 1))
    time = 1000;

  if ((millis() - timer_stopped > time) && !stopped)
  {
    stopped = true;
    speed_filtered.SetCurrent(0.0);
    saveData();
  }
}

float avgSpeed()
{

  float avg = 0.0;
  avg = trip / moving_time * 60.0;

  if (isinf(avg) || isnan(avg))
    avg = 0.0;

  return avg;
}

void mot_rot()
{
  rotation_impulses++;
}

void speed_one()
{
  if (!speed_impulse) // debouncing
    speedOneimpulseTime = millis() - timer_speed_one;

  speed_impulse = true;

  timer_speed_one = millis();
}

void speed_one_check()
{
  if (millis() - timer_30ms_loop > T30MS_LOOP)
  {
    if (speed_impulse)
    {

      stopped = false;

      detachInterrupt(PIN_DSPEED);
      speed_impulse = false;
      attachInterrupt(digitalPinToInterrupt(PIN_DSPEED), speed_one, FALLING);

      float spd = 0.0;
      spd = EEPROM.readInt(ADR_PERIMETER) * 3.4344 / speedOneimpulseTime; // experimental: 3.4344 instead of 3.6   because it seems like in real life readings are ~5% higher, so we decreasing perimeter about 5%
      speed_filtered.Filter(spd);
      timer_stopped = millis();
    }
  }
}

void speed_multi(int time)
{
  unsigned int rot_imp = 0;
  float rpm = 0.0;
  float rps = 0.0;
  float spd = 0.0;
  detachInterrupt(PIN_DSPEED);
  rot_imp = rotation_impulses;
  rotation_impulses = 0;
  attachInterrupt(digitalPinToInterrupt(PIN_DSPEED), mot_rot, FALLING);

  rps = (rot_imp * (1000.0 / time)) / (EEPROM.readInt(ADR_MOT_MAG) / 2);

  rpm = rps * 60.0;
  spd = (rpm / 1000.0) / (EEPROM.readInt(ADR_GEAR_RATIO) / 10.0) * 60.0 * (EEPROM.readInt(ADR_PERIMETER) / 1050.0); // experimental: 1050.0 becouse it seems like in real life readings are ~5% higher, so we decreasing perimeter about 5%
  speed_filtered.Filter(spd);

  if (spd > 1.0)
  {
    stopped = false;
    timer_stopped = millis();
  }
}

void countSpeed()
{
  int time = millis() - timer_speed;

  if (time >= SPEED_COUNT_TIME)
  {
    timer_speed = millis();

    if ((EEPROM.readInt(ADR_MOT_MAG) > 1))
      speed_multi(time);

    speed = speed_filtered.Current();

    if (EEPROM.readInt(ADR_KPHMPH))
      speed = speed * 0.621;

    if (speed > vmax)
      vmax = speed;

    float add_distance = (speed / 3600.0) * (time / 1000.0);

    dist += add_distance;
    if (dist > 50000)
      dist = 0;

    trip += add_distance;
    trip_dtg += add_distance;

    if (!stopped)
    {
      moving_time += time / 60000.0;

#ifdef MTG
      moving_time_mtg += time / 60000.0;
      total_mh += time / 3600000.0;
#endif
    }
  }
}
