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

void saveData()
{
  EEPROM.updateFloat(ADRDIST, dist);
  EEPROM.updateFloat(ADRWHUSED, Wh_used);
  EEPROM.updateFloat(ADRVMAX, vmax);
  EEPROM.updateFloat(ADRMVTIME, moving_time);
  EEPROM.updateFloat(ADRTRIP, trip);
  EEPROM.updateFloat(ADRWHUSEDKM, Wh_used_trp);
  EEPROM.updateFloat(ADRPMAX, powermax);
  EEPROM.updateFloat(ADRIMAX, imax);
  EEPROM.updateFloat(ADRMAHUSED, mah_used);
  EEPROM.updateFloat(ADRWHUSEDKMDTG, Wh_used_dtg);
  EEPROM.updateFloat(ADRTRIPDTG, trip_dtg);
  EEPROM.updateFloat(ADRTOTMAHUSED, total_ah_used);

  if (getCurrent() < 1.5)
    EEPROM.updateFloat(ADRLASTVOLTAGE, getBatRawVoltage());
#ifdef MTG
  EEPROM.updateFloat(ADRWHUSEDMTG, Wh_used_mtg);
  EEPROM.updateFloat(ADRMVTIMEMTG, moving_time_mtg);
  EEPROM.updateFloat(ADRTOTMH, total_mh);
#endif
}

#ifdef INITIAL
void initialConfigSave()
{
  EEPROM.updateInt(ADRFIRSTTIME, FIRST_TIME_CODE);

  // used in savaData(); and loadData();
  EEPROM.updateFloat(ADRDIST, 0.0);
  EEPROM.updateFloat(ADRWHUSED, 0.0);
  EEPROM.updateFloat(ADRVMAX, 0.0);
  EEPROM.updateFloat(ADRMVTIME, 0.0);
  EEPROM.updateFloat(ADRTRIP, 0.0);
  EEPROM.updateFloat(ADRWHUSEDKM, 0.0);
  EEPROM.updateFloat(ADRPMAX, 0.0);
  EEPROM.updateFloat(ADRIMAX, 0.0);
  EEPROM.updateFloat(ADRMAHUSED, 0.0);
  EEPROM.updateFloat(ADRWHUSEDKMDTG, 0.0);
  EEPROM.updateFloat(ADRTRIPDTG, 0.0);
  EEPROM.updateFloat(ADRTOTMAHUSED, 0.0);
  EEPROM.updateFloat(ADRLASTVOLTAGE, 95.0);
  EEPROM.updateFloat(ADRWHUSEDMTG, 0.0);
  EEPROM.updateFloat(ADRMVTIMEMTG, 0.0);
  EEPROM.updateFloat(ADRTOTMH, 0.0);

  EEPROM.updateFloat(ADR0CURRENT, 0.0);
  EEPROM.updateInt(ADR_WATCHDOGRESET, 0);

#ifdef INIT_3000W

  EEPROM.updateInt(ADR_ASSISTMODE, 3);
  EEPROM.updateInt(ADR_BATCAP_AH, 192);
  EEPROM.updateInt(ADR_BATCAP_WH, 1114);
  EEPROM.updateInt(ADR_LVC, 390);
  EEPROM.updateInt(ADR_FULL_BATT_V, 540);
  EEPROM.updateInt(ADR_MVPERA, 10);
  EEPROM.updateInt(ADR_CURDIR, 1);
  EEPROM.updateInt(ADR_VOL_DIV, 33058);
  EEPROM.updateInt(ADR_CUR_SENSOR_OK, 0);
  EEPROM.updateInt(ADR_CUR_PROT, 2);

  EEPROM.updateInt(ADR_TOT_MIN, 85);
  EEPROM.updateInt(ADR_TOT_MAX, 350);
  EEPROM.updateInt(ADR_TIN_MIN, 90);
  EEPROM.updateInt(ADR_TIN_MAX, 360);
  EEPROM.updateInt(ADR_THR_RESET, 1);
  EEPROM.updateInt(ADR_THR_SAFE_VOLTAGE, 370);

  EEPROM.updateInt(ADR_KPHMPH, 0);
  EEPROM.updateInt(ADR_PERIMETER, 2163);
  EEPROM.updateInt(ADR_MOT_MAG, 46);
  EEPROM.updateInt(ADR_GEAR_RATIO, 10);

  EEPROM.updateInt(ADR_BT_BUTTONS, 0);
  EEPROM.updateInt(ADR_EBRAKEHILO, 0);

  EEPROM.updateInt(ADR_TEMPCTEMPF, 0);
  EEPROM.updateInt(ADR_TEMPTYPE1, 3);
  EEPROM.updateInt(ADR_TEMPTYPE2, 1);
  EEPROM.updateInt(ADR_OVHT1, 140);
  EEPROM.updateInt(ADR_OVHT2, 60);

  EEPROM.updateInt(ADR_POWERKP, 150);
  EEPROM.updateInt(ADR_POWERKI, 80);
  EEPROM.updateInt(ADR_POWERKD, 50);
  EEPROM.updateInt(ADR_P_LOW, 0);
  EEPROM.updateInt(ADR_I_LOW, 0);
  EEPROM.updateInt(ADR_D_LOW, 0);
  EEPROM.updateInt(ADR_LOW_THRESHOLD, 0);

  EEPROM.updateInt(ADR_SPEEDFACTORMIN, 1);

  EEPROM.updateInt(ADR_PIDPWMMAX, 200);

  EEPROM.updateInt(ADR_SPEEDFACTOR_RAMP_UP, 40);
  EEPROM.updateInt(ADR_CRUISE_CONTROL_POWER_MIN, 240);
  EEPROM.updateInt(ADR_CRUISE_CONTROL_POWER_MAX, 1300);
  EEPROM.updateInt(ADR_CRUISE_CONTROL_POWER_RAMP_UP, 300);
  EEPROM.updateInt(ADR_CRUISE_CONTROL_MAX_SPEED, 40);

  EEPROM.updateInt(ADR_AUTOLEGAL, 1);
  EEPROM.updateInt(ADR_LEGALLIMIT_ON_OFF, 1);
  EEPROM.updateInt(ADR_LEGALLIMIT_SPEED, 25);
  EEPROM.updateInt(ADR_LEGALLIMIT_POWER, 250);
  EEPROM.updateInt(ADR_PASMAGNETS, 12);
  EEPROM.updateInt(ADR_PWR_LIM_PAS_1, 100);
  EEPROM.updateInt(ADR_PWR_LIM_PAS_2, 180);
  EEPROM.updateInt(ADR_PWR_LIM_PAS_3, 250);
  EEPROM.updateInt(ADR_PWR_LIM_PAS_4, 350);
  EEPROM.updateInt(ADR_PWR_LIM_PAS_5, 600);
  EEPROM.updateInt(ADR_SPD_LIM_PAS_1, 20);
  EEPROM.updateInt(ADR_SPD_LIM_PAS_2, 25);
  EEPROM.updateInt(ADR_SPD_LIM_PAS_3, 25);
  EEPROM.updateInt(ADR_SPD_LIM_PAS_4, 30);
  EEPROM.updateInt(ADR_SPD_LIM_PAS_5, 38);
  EEPROM.updateInt(ADR_MIN_SPD_PAS_1, 0);
  EEPROM.updateInt(ADR_MIN_SPD_PAS_2, 0);
  EEPROM.updateInt(ADR_MIN_SPD_PAS_3, 0);
  EEPROM.updateInt(ADR_MIN_SPD_PAS_4, 0);
  EEPROM.updateInt(ADR_MIN_SPD_PAS_5, 0);
  EEPROM.updateInt(ADR_CAD_MIN_PAS_1, 0);
  EEPROM.updateInt(ADR_CAD_MIN_PAS_2, 0);
  EEPROM.updateInt(ADR_CAD_MIN_PAS_3, 0);
  EEPROM.updateInt(ADR_CAD_MIN_PAS_4, 0);
  EEPROM.updateInt(ADR_CAD_MIN_PAS_5, 0);
  EEPROM.updateInt(ADR_CAD_MAX_PAS_1, 10);
  EEPROM.updateInt(ADR_CAD_MAX_PAS_2, 10);
  EEPROM.updateInt(ADR_CAD_MAX_PAS_3, 10);
  EEPROM.updateInt(ADR_CAD_MAX_PAS_4, 10);
  EEPROM.updateInt(ADR_CAD_MAX_PAS_5, 10);
  EEPROM.updateInt(ADR_RAMP_UP_PAS_1, 300);
  EEPROM.updateInt(ADR_RAMP_UP_PAS_2, 300);
  EEPROM.updateInt(ADR_RAMP_UP_PAS_3, 300);
  EEPROM.updateInt(ADR_RAMP_UP_PAS_4, 400);
  EEPROM.updateInt(ADR_RAMP_UP_PAS_5, 500);

  EEPROM.updateInt(ADR_BOOST_POWER_PAS_1, 500);
  EEPROM.updateInt(ADR_BOOST_POWER_PAS_2, 500);
  EEPROM.updateInt(ADR_BOOST_POWER_PAS_3, 750);
  EEPROM.updateInt(ADR_BOOST_POWER_PAS_4, 800);
  EEPROM.updateInt(ADR_BOOST_POWER_PAS_5, 1000);
  EEPROM.updateInt(ADR_BOOST_TIME_PAS_1, 3500);
  EEPROM.updateInt(ADR_BOOST_TIME_PAS_2, 3500);
  EEPROM.updateInt(ADR_BOOST_TIME_PAS_3, 3500);
  EEPROM.updateInt(ADR_BOOST_TIME_PAS_4, 3500);
  EEPROM.updateInt(ADR_BOOST_TIME_PAS_5, 3500);
  EEPROM.updateInt(ADR_BOOST_SPEED_PAS_1, 10);
  EEPROM.updateInt(ADR_BOOST_SPEED_PAS_2, 10);
  EEPROM.updateInt(ADR_BOOST_SPEED_PAS_3, 10);
  EEPROM.updateInt(ADR_BOOST_SPEED_PAS_4, 22);
  EEPROM.updateInt(ADR_BOOST_SPEED_PAS_5, 30);
  EEPROM.updateInt(ADR_BOOST_RAMP_UP_PAS, 5000);
  EEPROM.updateInt(ADR_CADENCE_COUNT_TIME, 250);
  EEPROM.updateInt(ADR_ENABLE_TORQUE_SENSOR, 0);
  EEPROM.updateInt(ADR_STARTUP_WEIGHT_ON_PEDAL, 180);
  EEPROM.updateInt(ADR_TORQUE_SENSOR_ADC_MIN, 315);
  EEPROM.updateInt(ADR_TORQUE_SENSOR_ADC_MAX, 620);
  EEPROM.updateInt(ADR_TORQUE_SENSOR_KGF_MAX, 600);

  EEPROM.updateInt(ADR_PWR_LIM_THR_1, 1000);
  EEPROM.updateInt(ADR_PWR_LIM_THR_2, 70);
  EEPROM.updateInt(ADR_PWR_LIM_THR_3, 90);
  EEPROM.updateInt(ADR_PWR_LIM_THR_4, 100);
  EEPROM.updateInt(ADR_PWR_LIM_THR_5, 100);
  EEPROM.updateInt(ADR_MODE_THR_1, 1);
  EEPROM.updateInt(ADR_MODE_THR_2, 0);
  EEPROM.updateInt(ADR_MODE_THR_3, 0);
  EEPROM.updateInt(ADR_MODE_THR_4, 0);
  EEPROM.updateInt(ADR_MODE_THR_5, 0);
  EEPROM.updateInt(ADR_RAMP_UP_THR_1, 1000);
  EEPROM.updateInt(ADR_RAMP_UP_THR_2, 3000);
  EEPROM.updateInt(ADR_RAMP_UP_THR_3, 3000);
  EEPROM.updateInt(ADR_RAMP_UP_THR_4, 3000);
  EEPROM.updateInt(ADR_RAMP_UP_THR_5, 3000);

#endif

#ifdef INIT_10000W

  EEPROM.updateInt(ADR_ASSISTMODE, 3);
  EEPROM.updateInt(ADR_BATCAP_AH, 440);
  EEPROM.updateInt(ADR_BATCAP_WH, 3190);
  EEPROM.updateInt(ADR_LVC, 600);
  EEPROM.updateInt(ADR_FULL_BATT_V, 835);
  EEPROM.updateInt(ADR_MVPERA, 10);
  EEPROM.updateInt(ADR_CURDIR, 1);
  EEPROM.updateInt(ADR_VOL_DIV, 33058);
  EEPROM.updateInt(ADR_CUR_SENSOR_OK, 0);
  EEPROM.updateInt(ADR_CUR_PROT, 20);

  EEPROM.updateInt(ADR_TOT_MIN, 85);
  EEPROM.updateInt(ADR_TOT_MAX, 350);
  EEPROM.updateInt(ADR_TIN_MIN, 90);
  EEPROM.updateInt(ADR_TIN_MAX, 360);
  EEPROM.updateInt(ADR_THR_RESET, 1);
  EEPROM.updateInt(ADR_THR_SAFE_VOLTAGE, 370);

  EEPROM.updateInt(ADR_KPHMPH, 0);
  EEPROM.updateInt(ADR_PERIMETER, 2050);
  EEPROM.updateInt(ADR_MOT_MAG, 32);
  EEPROM.updateInt(ADR_GEAR_RATIO, 10);

  EEPROM.updateInt(ADR_BT_BUTTONS, 0);
  EEPROM.updateInt(ADR_EBRAKEHILO, 1);

  EEPROM.updateInt(ADR_TEMPCTEMPF, 0);
  EEPROM.updateInt(ADR_TEMPTYPE1, 4);
  EEPROM.updateInt(ADR_TEMPTYPE2, 0);
  EEPROM.updateInt(ADR_OVHT1, 140);
  EEPROM.updateInt(ADR_OVHT2, 60);

  EEPROM.updateInt(ADR_POWERKP, 190);
  EEPROM.updateInt(ADR_POWERKI, 100);
  EEPROM.updateInt(ADR_POWERKD, 80);
  EEPROM.updateInt(ADR_P_LOW, 0);
  EEPROM.updateInt(ADR_I_LOW, 0);
  EEPROM.updateInt(ADR_D_LOW, 0);
  EEPROM.updateInt(ADR_LOW_THRESHOLD, 0);

  EEPROM.updateInt(ADR_SPEEDFACTORMIN, 1);

  EEPROM.updateInt(ADR_PIDPWMMAX, 70);

  EEPROM.updateInt(ADR_SPEEDFACTOR_RAMP_UP, 30);
  EEPROM.updateInt(ADR_CRUISE_CONTROL_POWER_MIN, 240);
  EEPROM.updateInt(ADR_CRUISE_CONTROL_POWER_MAX, 2000);
  EEPROM.updateInt(ADR_CRUISE_CONTROL_POWER_RAMP_UP, 260);
  EEPROM.updateInt(ADR_CRUISE_CONTROL_MAX_SPEED, 40);

  EEPROM.updateInt(ADR_AUTOLEGAL, 1);
  EEPROM.updateInt(ADR_LEGALLIMIT_ON_OFF, 0);
  EEPROM.updateInt(ADR_LEGALLIMIT_SPEED, 25);
  EEPROM.updateInt(ADR_LEGALLIMIT_POWER, 250);
  EEPROM.updateInt(ADR_PASMAGNETS, 12);
  EEPROM.updateInt(ADR_PWR_LIM_PAS_1, 500);
  EEPROM.updateInt(ADR_PWR_LIM_PAS_2, 700);
  EEPROM.updateInt(ADR_PWR_LIM_PAS_3, 900);
  EEPROM.updateInt(ADR_PWR_LIM_PAS_4, 1100);
  EEPROM.updateInt(ADR_PWR_LIM_PAS_5, 1100);
  EEPROM.updateInt(ADR_SPD_LIM_PAS_1, 20);
  EEPROM.updateInt(ADR_SPD_LIM_PAS_2, 25);
  EEPROM.updateInt(ADR_SPD_LIM_PAS_3, 30);
  EEPROM.updateInt(ADR_SPD_LIM_PAS_4, 35);
  EEPROM.updateInt(ADR_SPD_LIM_PAS_5, 35);
  EEPROM.updateInt(ADR_MIN_SPD_PAS_1, 0);
  EEPROM.updateInt(ADR_MIN_SPD_PAS_2, 0);
  EEPROM.updateInt(ADR_MIN_SPD_PAS_3, 0);
  EEPROM.updateInt(ADR_MIN_SPD_PAS_4, 0);
  EEPROM.updateInt(ADR_MIN_SPD_PAS_5, 0);
  EEPROM.updateInt(ADR_CAD_MIN_PAS_1, 0);
  EEPROM.updateInt(ADR_CAD_MIN_PAS_2, 0);
  EEPROM.updateInt(ADR_CAD_MIN_PAS_3, 0);
  EEPROM.updateInt(ADR_CAD_MIN_PAS_4, 0);
  EEPROM.updateInt(ADR_CAD_MIN_PAS_5, 0);
  EEPROM.updateInt(ADR_CAD_MAX_PAS_1, 10);
  EEPROM.updateInt(ADR_CAD_MAX_PAS_2, 10);
  EEPROM.updateInt(ADR_CAD_MAX_PAS_3, 10);
  EEPROM.updateInt(ADR_CAD_MAX_PAS_4, 10);
  EEPROM.updateInt(ADR_CAD_MAX_PAS_5, 10);
  EEPROM.updateInt(ADR_RAMP_UP_PAS_1, 200);
  EEPROM.updateInt(ADR_RAMP_UP_PAS_2, 200);
  EEPROM.updateInt(ADR_RAMP_UP_PAS_3, 400);
  EEPROM.updateInt(ADR_RAMP_UP_PAS_4, 500);
  EEPROM.updateInt(ADR_RAMP_UP_PAS_5, 500);

  EEPROM.updateInt(ADR_BOOST_POWER_PAS_1, 2000);
  EEPROM.updateInt(ADR_BOOST_POWER_PAS_2, 2000);
  EEPROM.updateInt(ADR_BOOST_POWER_PAS_3, 2000);
  EEPROM.updateInt(ADR_BOOST_POWER_PAS_4, 2000);
  EEPROM.updateInt(ADR_BOOST_POWER_PAS_5, 2000);
  EEPROM.updateInt(ADR_BOOST_TIME_PAS_1, 3500);
  EEPROM.updateInt(ADR_BOOST_TIME_PAS_2, 3500);
  EEPROM.updateInt(ADR_BOOST_TIME_PAS_3, 3500);
  EEPROM.updateInt(ADR_BOOST_TIME_PAS_4, 3500);
  EEPROM.updateInt(ADR_BOOST_TIME_PAS_5, 3500);
  EEPROM.updateInt(ADR_BOOST_SPEED_PAS_1, 10);
  EEPROM.updateInt(ADR_BOOST_SPEED_PAS_2, 10);
  EEPROM.updateInt(ADR_BOOST_SPEED_PAS_3, 22);
  EEPROM.updateInt(ADR_BOOST_SPEED_PAS_4, 27);
  EEPROM.updateInt(ADR_BOOST_SPEED_PAS_5, 27);
  EEPROM.updateInt(ADR_BOOST_RAMP_UP_PAS, 5000);
  EEPROM.updateInt(ADR_CADENCE_COUNT_TIME, 250);
  EEPROM.updateInt(ADR_ENABLE_TORQUE_SENSOR, 0);
  EEPROM.updateInt(ADR_STARTUP_WEIGHT_ON_PEDAL, 180);
  EEPROM.updateInt(ADR_TORQUE_SENSOR_ADC_MIN, 315);
  EEPROM.updateInt(ADR_TORQUE_SENSOR_ADC_MAX, 620);
  EEPROM.updateInt(ADR_TORQUE_SENSOR_KGF_MAX, 600);

  EEPROM.updateInt(ADR_PWR_LIM_THR_1, 1000);
  EEPROM.updateInt(ADR_PWR_LIM_THR_2, 35);
  EEPROM.updateInt(ADR_PWR_LIM_THR_3, 35);
  EEPROM.updateInt(ADR_PWR_LIM_THR_4, 50);
  EEPROM.updateInt(ADR_PWR_LIM_THR_5, 100);
  EEPROM.updateInt(ADR_MODE_THR_1, 1);
  EEPROM.updateInt(ADR_MODE_THR_2, 0);
  EEPROM.updateInt(ADR_MODE_THR_3, 0);
  EEPROM.updateInt(ADR_MODE_THR_4, 0);
  EEPROM.updateInt(ADR_MODE_THR_5, 0);
  EEPROM.updateInt(ADR_RAMP_UP_THR_1, 1000);
  EEPROM.updateInt(ADR_RAMP_UP_THR_2, 500);
  EEPROM.updateInt(ADR_RAMP_UP_THR_3, 500);
  EEPROM.updateInt(ADR_RAMP_UP_THR_4, 2000);
  EEPROM.updateInt(ADR_RAMP_UP_THR_5, 2000);

#endif

#ifdef INIT_6000W

  EEPROM.updateInt(ADR_ASSISTMODE, 5);
  EEPROM.updateInt(ADR_BATCAP_AH, 400);
  EEPROM.updateInt(ADR_BATCAP_WH, 2890);
  EEPROM.updateInt(ADR_LVC, 600);
  EEPROM.updateInt(ADR_FULL_BATT_V, 830);
  EEPROM.updateInt(ADR_MVPERA, 10);
  EEPROM.updateInt(ADR_CURDIR, 0);
  EEPROM.updateInt(ADR_VOL_DIV, 33187);
  EEPROM.updateInt(ADR_CUR_SENSOR_OK, 0);
  EEPROM.updateInt(ADR_CUR_PROT, 30);

  EEPROM.updateInt(ADR_TOT_MIN, 85);
  EEPROM.updateInt(ADR_TOT_MAX, 350);
  EEPROM.updateInt(ADR_TIN_MIN, 90);
  EEPROM.updateInt(ADR_TIN_MAX, 370);
  EEPROM.updateInt(ADR_THR_RESET, 1);
  EEPROM.updateInt(ADR_THR_SAFE_VOLTAGE, 370);

  EEPROM.updateInt(ADR_KPHMPH, 0);
  EEPROM.updateInt(ADR_PERIMETER, 1890);
  EEPROM.updateInt(ADR_MOT_MAG, 46);
  EEPROM.updateInt(ADR_GEAR_RATIO, 10);

  EEPROM.updateInt(ADR_BT_BUTTONS, 0);
  EEPROM.updateInt(ADR_EBRAKEHILO, 1);

  EEPROM.updateInt(ADR_TEMPCTEMPF, 0);
  EEPROM.updateInt(ADR_TEMPTYPE1, 3);
  EEPROM.updateInt(ADR_TEMPTYPE2, 1);
  EEPROM.updateInt(ADR_OVHT1, 120);
  EEPROM.updateInt(ADR_OVHT2, 60);

  EEPROM.updateInt(ADR_POWERKP, 200);
  EEPROM.updateInt(ADR_POWERKI, 70);
  EEPROM.updateInt(ADR_POWERKD, 140);
  EEPROM.updateInt(ADR_P_LOW, 50);
  EEPROM.updateInt(ADR_I_LOW, 30);
  EEPROM.updateInt(ADR_D_LOW, 50);
  EEPROM.updateInt(ADR_LOW_THRESHOLD, 300);

  EEPROM.updateInt(ADR_SPEEDFACTORMIN, 1);

  EEPROM.updateInt(ADR_PIDPWMMAX, 200);

  EEPROM.updateInt(ADR_SPEEDFACTOR_RAMP_UP, 50);
  EEPROM.updateInt(ADR_CRUISE_CONTROL_POWER_MIN, 240);
  EEPROM.updateInt(ADR_CRUISE_CONTROL_POWER_MAX, 1300);
  EEPROM.updateInt(ADR_CRUISE_CONTROL_POWER_RAMP_UP, 300);
  EEPROM.updateInt(ADR_CRUISE_CONTROL_MAX_SPEED, 40);

  EEPROM.updateInt(ADR_AUTOLEGAL, 1);
  EEPROM.updateInt(ADR_LEGALLIMIT_ON_OFF, 0);
  EEPROM.updateInt(ADR_LEGALLIMIT_SPEED, 28);
  EEPROM.updateInt(ADR_LEGALLIMIT_POWER, 350);
  EEPROM.updateInt(ADR_PASMAGNETS, 36);
  EEPROM.updateInt(ADR_PWR_LIM_PAS_1, 5);
  EEPROM.updateInt(ADR_PWR_LIM_PAS_2, 10);
  EEPROM.updateInt(ADR_PWR_LIM_PAS_3, 15);
  EEPROM.updateInt(ADR_PWR_LIM_PAS_4, 20);
  EEPROM.updateInt(ADR_PWR_LIM_PAS_5, 1100);
  EEPROM.updateInt(ADR_SPD_LIM_PAS_1, 55);
  EEPROM.updateInt(ADR_SPD_LIM_PAS_2, 55);
  EEPROM.updateInt(ADR_SPD_LIM_PAS_3, 55);
  EEPROM.updateInt(ADR_SPD_LIM_PAS_4, 55);
  EEPROM.updateInt(ADR_SPD_LIM_PAS_5, 37);
  EEPROM.updateInt(ADR_MIN_SPD_PAS_1, 0);
  EEPROM.updateInt(ADR_MIN_SPD_PAS_2, 0);
  EEPROM.updateInt(ADR_MIN_SPD_PAS_3, 0);
  EEPROM.updateInt(ADR_MIN_SPD_PAS_4, 0);
  EEPROM.updateInt(ADR_MIN_SPD_PAS_5, 3);
  EEPROM.updateInt(ADR_CAD_MIN_PAS_1, 0);
  EEPROM.updateInt(ADR_CAD_MIN_PAS_2, 0);
  EEPROM.updateInt(ADR_CAD_MIN_PAS_3, 0);
  EEPROM.updateInt(ADR_CAD_MIN_PAS_4, 0);
  EEPROM.updateInt(ADR_CAD_MIN_PAS_5, 0);
  EEPROM.updateInt(ADR_CAD_MAX_PAS_1, 10);
  EEPROM.updateInt(ADR_CAD_MAX_PAS_2, 10);
  EEPROM.updateInt(ADR_CAD_MAX_PAS_3, 10);
  EEPROM.updateInt(ADR_CAD_MAX_PAS_4, 10);
  EEPROM.updateInt(ADR_CAD_MAX_PAS_5, 10);
  EEPROM.updateInt(ADR_RAMP_UP_PAS_1, 350);
  EEPROM.updateInt(ADR_RAMP_UP_PAS_2, 350);
  EEPROM.updateInt(ADR_RAMP_UP_PAS_3, 350);
  EEPROM.updateInt(ADR_RAMP_UP_PAS_4, 350);
  EEPROM.updateInt(ADR_RAMP_UP_PAS_5, 350);

  EEPROM.updateInt(ADR_BOOST_POWER_PAS_1, 500);
  EEPROM.updateInt(ADR_BOOST_POWER_PAS_2, 500);
  EEPROM.updateInt(ADR_BOOST_POWER_PAS_3, 800);
  EEPROM.updateInt(ADR_BOOST_POWER_PAS_4, 1100);
  EEPROM.updateInt(ADR_BOOST_POWER_PAS_5, 1000);
  EEPROM.updateInt(ADR_BOOST_TIME_PAS_1, 1500);
  EEPROM.updateInt(ADR_BOOST_TIME_PAS_2, 2000);
  EEPROM.updateInt(ADR_BOOST_TIME_PAS_3, 2000);
  EEPROM.updateInt(ADR_BOOST_TIME_PAS_4, 2000);
  EEPROM.updateInt(ADR_BOOST_TIME_PAS_5, 1500);
  EEPROM.updateInt(ADR_BOOST_SPEED_PAS_1, 20);
  EEPROM.updateInt(ADR_BOOST_SPEED_PAS_2, 25);
  EEPROM.updateInt(ADR_BOOST_SPEED_PAS_3, 55);
  EEPROM.updateInt(ADR_BOOST_SPEED_PAS_4, 55);
  EEPROM.updateInt(ADR_BOOST_SPEED_PAS_5, 29);
  EEPROM.updateInt(ADR_BOOST_RAMP_UP_PAS, 5000);
  EEPROM.updateInt(ADR_CADENCE_COUNT_TIME, 250);
  EEPROM.updateInt(ADR_ENABLE_TORQUE_SENSOR, 1);
  EEPROM.updateInt(ADR_STARTUP_WEIGHT_ON_PEDAL, 100);
  EEPROM.updateInt(ADR_TORQUE_SENSOR_ADC_MIN, 325);
  EEPROM.updateInt(ADR_TORQUE_SENSOR_ADC_MAX, 620);
  EEPROM.updateInt(ADR_TORQUE_SENSOR_KGF_MAX, 750);

  EEPROM.updateInt(ADR_PWR_LIM_THR_1, 60);
  EEPROM.updateInt(ADR_PWR_LIM_THR_2, 100);
  EEPROM.updateInt(ADR_PWR_LIM_THR_3, 100);
  EEPROM.updateInt(ADR_PWR_LIM_THR_4, 100);
  EEPROM.updateInt(ADR_PWR_LIM_THR_5, 100);
  EEPROM.updateInt(ADR_MODE_THR_1, 0);
  EEPROM.updateInt(ADR_MODE_THR_2, 0);
  EEPROM.updateInt(ADR_MODE_THR_3, 0);
  EEPROM.updateInt(ADR_MODE_THR_4, 0);
  EEPROM.updateInt(ADR_MODE_THR_5, 0);
  EEPROM.updateInt(ADR_RAMP_UP_THR_1, 3500);
  EEPROM.updateInt(ADR_RAMP_UP_THR_2, 5000);
  EEPROM.updateInt(ADR_RAMP_UP_THR_3, 5000);
  EEPROM.updateInt(ADR_RAMP_UP_THR_4, 5000);
  EEPROM.updateInt(ADR_RAMP_UP_THR_5, 5000);

#endif
}
#endif

float eepromLoadFloat(int adr)
{
  float value = 0.0;
  value = EEPROM.readFloat(adr);
  return value;
}

void loadData()
{
  dist = eepromLoadFloat(ADRDIST);
  Wh_used = eepromLoadFloat(ADRWHUSED);
  vmax = eepromLoadFloat(ADRVMAX);
  moving_time = eepromLoadFloat(ADRMVTIME);
  trip = eepromLoadFloat(ADRTRIP);
  Wh_used_trp = eepromLoadFloat(ADRWHUSEDKM);
  powermax = eepromLoadFloat(ADRPMAX);
  imax = eepromLoadFloat(ADRIMAX);
  mah_used = eepromLoadFloat(ADRMAHUSED);
  Wh_used_dtg = eepromLoadFloat(ADRWHUSEDKMDTG);
  trip_dtg = eepromLoadFloat(ADRTRIPDTG);
  total_ah_used = eepromLoadFloat(ADRTOTMAHUSED);
#ifdef MTG
  moving_time_mtg = eepromLoadFloat(ADRMVTIMEMTG);
  Wh_used_mtg = eepromLoadFloat(ADRWHUSEDMTG);
  total_mh = eepromLoadFloat(ADRTOTMH);
#endif
}
