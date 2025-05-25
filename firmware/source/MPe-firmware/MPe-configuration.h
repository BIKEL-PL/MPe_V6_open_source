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

// USE BOOTLOADER FROM ARDUINO UNO FOR WATCHDOG WORKING PROPERLY !!!!!!!!!!
// PID PWM MIN AND MAX STORED IN PID_v1.cpp file

#define FIRMWARE_VERSION 6010
#define FIRST_TIME_CODE 12346

// #define INITIAL // Required for new hardware - stores eeprom values and first time CODE)

#ifdef INITIAL
// #define INIT_3000W
// #define INIT_6000W
// #define INIT_10000W
#endif

// CONFIG
#define OLED1 // Enable Oled Display for SSD1306 128x32 0,91"

#ifdef OLED1
#define CONFIG_MENU // Enable built-in Configuration screen/menu (using ~8% of memory !!!)
#endif

#define PAS // Enable PAS support // using ~12% of memory
// #define MTG // Adds Minutes To Go and moto-hours screen //using ~4% of memory
// #define LIGHT_OPERATION // Experimental lights turn on/off functionality. Requires external hardware.

// #define SERIALPLOT // Enable debug printing for plotting
// #define TESTING // OVERRIDES VALUES current,voltage speed etc... for something to display without actually measure anything.
// #define TEST_LOOP_SPEED // Enable debug prints with time needed to iterate over main loop
#define DEBUG // Enables debug messages //using ~1% of memory

// TIMERS

#define T100MS_LOOP 100
#define T30MS_LOOP 30
#define SPEED_COUNT_TIME 250
#define CADENCE_COUNT_TIME 500
#define SCREENREFRESH 602
#define TIME_SEND_UART 497

// REST
#ifdef MTG
#define NUM_SCREEN_POSITIONS 6
#endif

#ifndef MTG
#define NUM_SCREEN_POSITIONS 5
#endif

// PINS DIGITAL // UNUSED PINS: 1,4,9,10,11,12,13
#define PIN_PAS 2
#define PIN_DSPEED 3
#define PIN_THROTTLE_OUT 5
#define PIN_BUTTON_DOWN 7
#define PIN_BUTTON_UP 6
#define PIN_BRAKE 8

// PINS ANALOG ATMEGA328 // UNUSED PINS: A2,A3,A6,A7
#define PIN_LM35_TEMP_T1 0
#define PIN_LM35_TEMP_T2 1
#define PIN_SDA 4 // DISPLAY
#define PIN_SCL 5 // DISPLAY

// PINS ANALOG ADS1115
#define PIN_V_REFERENCE 0
#define PIN_CURRENT 1
#define PIN_VOLTAGE 2
#define PIN_THROTTLE_IN 3

#define TEMPRESVAL 2200.0
#define TEMPRESVAL2 1000.0

// Steinhart-Hart coefficients http://www.useasydocs.com/theory/ntc.htm
#define A_THERM 0.001129148
#define B_THERM 0.000234125
#define C_THERM 0.0000000876741

#define A_10K 0.001129148
#define B_10K 0.000234125
#define C_10K 0.0000000876741

#define A_KTY83 0.02262021256593017
#define B_KTY83 -0.003444732389272895
#define C_KTY83 0.00001374931587826135

/*
 #define A_KTY84 0.020453999478825556
 #define B_KTY84 -0.003255834625441624
 #define C_KTY84 0.000014308677805740018


 //KTY81-210
 #define A_KTY81 0.02036578643006971
 #define B_KTY81 -0.002642924987770203
 #define C_KTY81 0.000007026714406562518

 // http://www.useasydocs.com/theory/ntc.htm
*/

#ifdef DEBUG
#define DEBUG_PR(x) Serial.print(x)
#define DEBUG_PRSC(x) Serial.printsc(x)
#define DEBUG_PRLN(x) Serial.println(x)
#else
#define DEBUG_PR(x)
#define DEBUG_PRLN(x)
#endif

// EEPROM ADDRESSES
#define ADRFIRSTTIME 0

// calculated in loop() and saved with saveData() to be able to restore them after restart
#define ADRDIST 4
#define ADRWHUSED 8
#define ADRVMAX 12
#define ADRWHUSEDKM 16
#define ADRMVTIME 20
#define ADRTRIP 24
#define ADRPMAX 28
#define ADRMAHUSED 32
#define ADRWHUSEDKMDTG 36
#define ADRTRIPDTG 40
#define ADRTOTMAHUSED 44
#define ADRLASTVOLTAGE 48
#define ADRWHUSEDMTG 52
#define ADRMVTIMEMTG 56
#define ADRIMAX 60
#define ADRTOTMH 64

// Hidden configuration
#define ADR0CURRENT 100
#define ADR_PRINTINFO 104
#define ADR_SERIAL_PLOT 106

#define ADR_WATCHDOGRESET 112

// main configuration, rarely changed, set with initialConfig() - not required to load them into variables with loadData()
#define ADR_ASSISTMODE 200
#define ADR_BATCAP_AH 202
#define ADR_BATCAP_WH 204
#define ADR_LVC 206
#define ADR_FULL_BATT_V 208
#define ADR_MVPERA 210
#define ADR_CURDIR 212
#define ADR_VOL_DIV 214
#define ADR_CUR_SENSOR_OK 216
#define ADR_CUR_PROT 218
// #define	empty 220
// #define	empty 222
// #define	empty 224
#define ADR_TOT_MIN 226
#define ADR_TOT_MAX 228
#define ADR_TIN_MIN 230
#define ADR_TIN_MAX 232
#define ADR_THR_RESET 234
#define ADR_THR_SAFE_VOLTAGE 236
// #define	empty 238
// #define	empty 240
// #define	empty 242
// #define	empty 244
// #define	empty 246
// #define	empty 248
#define ADR_KPHMPH 250
#define ADR_PERIMETER 252
#define ADR_MOT_MAG 254
#define ADR_GEAR_RATIO 256
// #define	empty 258
// #define	empty 260
// #define	empty 262
// #define	empty 264
// #define	empty 266
#define ADR_BT_BUTTONS 268
#define ADR_EBRAKEHILO 270
// #define	ADR_EBRAKEINSTALLEDBYPASS 272
// #define	empty 274
// #define	empty 276
// #define	empty 278
#define ADR_TEMPCTEMPF 280
#define ADR_TEMPTYPE1 282
#define ADR_TEMPTYPE2 284
#define ADR_OVHT1 286
#define ADR_OVHT2 288
// #define	ADR_OVHT_REDPOWER 290
// #define	empty 292
// #define	empty 294
// #define	empty 296
// #define	empty 298
#define ADR_POWERKP 300
#define ADR_POWERKI 302
#define ADR_POWERKD 304
#define ADR_P_LOW 306
#define ADR_I_LOW 308
#define ADR_D_LOW 310
#define ADR_LOW_THRESHOLD 312
// #define	ADR_CRUISEKI 314
// #define	ADR_CRUISEKD 316
#define ADR_SPEEDFACTORMIN 318
#define ADR_PIDPWMMAX 320
#define ADR_SPEEDFACTOR_RAMP_UP 322
#define ADR_CRUISE_CONTROL_POWER_MIN 324
#define ADR_CRUISE_CONTROL_POWER_MAX 326
#define ADR_CRUISE_CONTROL_POWER_RAMP_UP 328
#define ADR_CRUISE_CONTROL_MAX_SPEED 330
// #define	empty 332
// #define	empty 334
// #define	empty 336
// #define	empty 338
#define ADR_AUTOLEGAL 340
#define ADR_LEGALLIMIT_ON_OFF 342
#define ADR_LEGALLIMIT_SPEED 344
#define ADR_LEGALLIMIT_POWER 346
#define ADR_PASMAGNETS 348
#define ADR_PWR_LIM_PAS_1 350
#define ADR_PWR_LIM_PAS_2 352
#define ADR_PWR_LIM_PAS_3 354
#define ADR_PWR_LIM_PAS_4 356
#define ADR_PWR_LIM_PAS_5 358
#define ADR_SPD_LIM_PAS_1 360
#define ADR_SPD_LIM_PAS_2 362
#define ADR_SPD_LIM_PAS_3 364
#define ADR_SPD_LIM_PAS_4 366
#define ADR_SPD_LIM_PAS_5 368
#define ADR_MIN_SPD_PAS_1 370
#define ADR_MIN_SPD_PAS_2 372
#define ADR_MIN_SPD_PAS_3 374
#define ADR_MIN_SPD_PAS_4 376
#define ADR_MIN_SPD_PAS_5 378
#define ADR_CAD_MIN_PAS_1 380
#define ADR_CAD_MIN_PAS_2 382
#define ADR_CAD_MIN_PAS_3 384
#define ADR_CAD_MIN_PAS_4 386
#define ADR_CAD_MIN_PAS_5 388
#define ADR_CAD_MAX_PAS_1 390
#define ADR_CAD_MAX_PAS_2 392
#define ADR_CAD_MAX_PAS_3 394
#define ADR_CAD_MAX_PAS_4 396
#define ADR_CAD_MAX_PAS_5 398
#define ADR_RAMP_UP_PAS_1 400
#define ADR_RAMP_UP_PAS_2 402
#define ADR_RAMP_UP_PAS_3 404
#define ADR_RAMP_UP_PAS_4 406
#define ADR_RAMP_UP_PAS_5 408
#define ADR_BOOST_POWER_PAS_1 410
#define ADR_BOOST_POWER_PAS_2 412
#define ADR_BOOST_POWER_PAS_3 414
#define ADR_BOOST_POWER_PAS_4 416
#define ADR_BOOST_POWER_PAS_5 418
#define ADR_BOOST_TIME_PAS_1 420
#define ADR_BOOST_TIME_PAS_2 422
#define ADR_BOOST_TIME_PAS_3 424
#define ADR_BOOST_TIME_PAS_4 426
#define ADR_BOOST_TIME_PAS_5 428
#define ADR_BOOST_SPEED_PAS_1 430
#define ADR_BOOST_SPEED_PAS_2 432
#define ADR_BOOST_SPEED_PAS_3 434
#define ADR_BOOST_SPEED_PAS_4 436
#define ADR_BOOST_SPEED_PAS_5 438
#define ADR_BOOST_RAMP_UP_PAS 440
#define ADR_CADENCE_COUNT_TIME 442
#define ADR_ENABLE_TORQUE_SENSOR 444
#define ADR_STARTUP_WEIGHT_ON_PEDAL 446
#define ADR_TORQUE_SENSOR_ADC_MIN 448
#define ADR_TORQUE_SENSOR_ADC_MAX 450
#define ADR_TORQUE_SENSOR_KGF_MAX 452
// #define	empty 454
// #define	empty 456
// #define	empty 458
#define ADR_PWR_LIM_THR_1 460
#define ADR_PWR_LIM_THR_2 462
#define ADR_PWR_LIM_THR_3 464
#define ADR_PWR_LIM_THR_4 466
#define ADR_PWR_LIM_THR_5 468
#define ADR_MODE_THR_1 470
#define ADR_MODE_THR_2 472
#define ADR_MODE_THR_3 474
#define ADR_MODE_THR_4 476
#define ADR_MODE_THR_5 478
#define ADR_RAMP_UP_THR_1 480
#define ADR_RAMP_UP_THR_2 482
#define ADR_RAMP_UP_THR_3 484
#define ADR_RAMP_UP_THR_4 486
#define ADR_RAMP_UP_THR_5 488
// #define	empty 490
// #define	empty 492
// #define	empty 494
// #define	empty 496
// #define	empty 498
// #define	empty 500
// #define	empty 502
// #define	empty 504
// #define	empty 506
// #define	empty 508
// #define	empty 510