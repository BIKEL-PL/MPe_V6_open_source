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

float convertCtoF(float temp)
{
  return (temp * 1.8) + 32.0;
}

int Convert_VtoTempLM35(float voltage)
{
  float temp = 0.0;
  temp = voltage * 100.0;
  return int(temp);
}

int Convert_VtoTempThermistor(float v_in, int type) //[V]
{
  float coef_A = A_10K;
  float coef_B = B_10K;
  float coef_C = C_10K;

  if (type == 2 || type == 4)
  {
    coef_A = A_KTY83;
    coef_B = B_KTY83;
    coef_C = C_KTY83;
  }

  float Rs = ((TEMPRESVAL * (vRef - 0.67)) / v_in) - TEMPRESVAL; // resistance of thermistor

  if (type == 3 || type == 4)
    Rs = TEMPRESVAL / ((((vRef / v_in) - 1.0) * (TEMPRESVAL / TEMPRESVAL2)) - 1.0); // Resistance of thermistor Mxus3k Turbo (common ground with hall)

  // T = 1/(a + b×ln(Rs) + c×(ln(Rs))3 )  http://www.useasydocs.com/theory/ntc.htm

  float lnRs = log(Rs);

  float temp = (1 / (coef_A + (coef_B * lnRs) + (coef_C * (lnRs * lnRs * lnRs)))) - 273.15;

  return int(temp);
}

int getTemp_1()
{
#ifndef TESTING
  return getTemp(pin_temp1_V_filtered.Current(), EEPROM.readInt(ADR_TEMPTYPE1));
#endif
#ifdef TESTING
  return randomtemp1;
#endif
}

int getTemp_2()
{
#ifndef TESTING
  return getTemp(pin_temp2_V_filtered.Current(), EEPROM.readInt(ADR_TEMPTYPE2));
#endif
#ifdef TESTING
  return randomtemp2;
#endif
}

int getTemp(float tempPinV, int type)
{
  int temp = 0;

  if (type == 0)
    temp = Convert_VtoTempLM35(tempPinV);
  else
    temp = Convert_VtoTempThermistor(tempPinV, type);

  if (EEPROM.readInt(ADR_TEMPCTEMPF))
    return convertCtoF(temp);
  else
    return temp;
}

void checkTemp()
{
  float pin_temp_V = vRef / 1024.0 * analogRead(PIN_LM35_TEMP_T1);
  pin_temp1_V_filtered.Filter(pin_temp_V);

  pin_temp_V = vRef / 1024.0 * analogRead(PIN_LM35_TEMP_T2);
  pin_temp2_V_filtered.Filter(pin_temp_V);

  if ((getTemp_1() > int(EEPROM.readInt(ADR_OVHT1)) || getTemp_2() > int(EEPROM.readInt(ADR_OVHT2))) && temperatureOK)
  {
    temperatureOK = false;
  }

  if (!temperatureOK)
  {
    if (getTemp_1() < int(EEPROM.readInt(ADR_OVHT1)) - 10 && getTemp_2() < int(EEPROM.readInt(ADR_OVHT2)) - 10)
      temperatureOK = true;
  }
}
