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

// Configuration
#include "MPe-configuration.h"

// Custom serial print functions
#include "MPe-PrintWithSC.h"
PrintWithSC mySerial(Serial);

// EEPROM Save&Load
#include "EEPROMex.h"

#ifdef PAS
// POWER PID
#include "PID_v1.h"
float pPID_Set, pPID_In, pPID_Out;
PID powerPID(&pPID_In, &pPID_Out, &pPID_Set, 0.1, 0.1, 0.1, 1, EEPROM.readInt(ADR_PIDPWMMAX)); //, DIRECT);
#endif

#ifdef OLED1
// DISPLAY
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "DejaVu_LGC_Sans_Mono_Bold_20.h"
Adafruit_SSD1306 display(4);
#endif

// ADS1115 ADC 15bit
#include "Adafruit_ADS1015.h"
Adafruit_ADS1115 ads;

// WATCHDOG
#include <avr/wdt.h>

// Expotential Filter
#include "Filter.h"
ExponentialFilter<float> pin_current_mV_filtered(10.0, 2480.0);
ExponentialFilter<float> vRef_filtered(10.0, 5.0);
ExponentialFilter<float> pin_voltage_mV_filtered(20.0, 2800.0);
ExponentialFilter<float> pin_temp1_V_filtered(3.0, 0.3);
ExponentialFilter<float> pin_temp2_V_filtered(3.0, 0.3);
ExponentialFilter<float> pas_rpm_ex(20.0, 0.0);
ExponentialFilter<float> speed_filtered(40.0, 0.0);
ExponentialFilter<float> weightOnPedal_adc_filtered(50.0, 0);
ExponentialFilter<float> humanPower_adc_filtered(15.0, 0);

bool b_up_p = false;
bool b_up_lp = false;
bool b_dwn_p = false;
bool b_dwn_lp = false;
bool b_updwn_p = false;
bool stopped = true;
bool cruisecontrol = false;
bool screenconfig = false;
bool screenconfig_just_entered = false;
bool temperatureOK = true;
bool brake = true;
bool throttle_relased_reset = true;
bool last_brake = true;
#ifdef CONFIG_MENU
bool configchanged = false;
#endif
bool statscreen = false;
bool rideOK = false;
bool pas_after_power_throttle = false;

volatile int rotation_impulses = 0;
#ifdef PAS
volatile int pas_rotation_impulses = 0;
#endif
volatile int speedOneimpulseTime = 0;
volatile bool speed_impulse = false;
volatile unsigned long timer_speed_one = 0;

int screen = 1;
int cruisecontrol_pwm = 0;
#ifdef CONFIG_MENU
int configCursor = 1;
#endif
int last_dtg = 0;
int last_assistmode = 0;
int address = 0;
int pwmOut = 0;
int voltageOut = 80;
int speedFactor = 100;

float powermax = 0.0;
float imax = 0.0;
float Wh_used = 0.0;
float Wh_used_trp = 0.0;
float Wh_used_dtg = 0.0;
float vRef = 5.0;
#ifdef MTG
float Wh_used_mtg = 0.0;
float moving_time_mtg = 0;
float last_mtg = 0.0;
float total_mh = 0.0;
#endif
float mah_used = 0.0;
float total_ah_used = 0.0;
float speed = 0.0;
float vmax = 0.0;
float dist = 0.0;
float trip = 0.0;
float trip_dtg = 0.0;
float moving_time = 0.0;
float cruisecontrol_speed = 0.0;

#ifdef PAS
unsigned long timer_pas_rpm = 0;
#endif
unsigned long timer_100ms_loop = 0;
unsigned long timer_30ms_loop = 0;
unsigned long timer_speed = 0;
unsigned long timer_button = 0;
unsigned long timer_refresh = 0;
unsigned long timer_send_uart = 0;
unsigned long timer_stopped = 0;
unsigned long timer_no_pedalling = 0;
unsigned long timer_adc_offset = 0;
unsigned long timer_value_delay_1 = 0;
unsigned long timer_value_delay_2 = 0;

#ifdef TESTING
unsigned long timer_speedtesting = 0;
unsigned long timer_randomtesting = 0;
float randomcurrent = 0.0;
float randomtemp1 = 0.0;
float randomtemp2 = 0.0;
int randomspeedms = 50;
#endif

#ifdef TEST_LOOP_SPEED
unsigned long timer_loopspeed = 0;
int loopcount = 0;
int looptime = 0;
#endif

float adc_offset = 0.0;
bool adc_will_save = 0;
bool adc_saved = 1;

#ifdef LIGHT_OPERATION
bool D4_state = 0;
bool D9_state = 0;
bool LCD_dark = 0;
#endif

void setup()
{

  Serial.begin(9600);
  Serial.setTimeout(50); // serial.parseint hangs for 1s without it

  // PINMODE
  pinMode(PIN_THROTTLE_OUT, OUTPUT);
  analogWrite(PIN_THROTTLE_OUT, 0);

  pinMode(PIN_BUTTON_UP, INPUT_PULLUP);
  pinMode(PIN_BUTTON_DOWN, INPUT_PULLUP);
  pinMode(PIN_BRAKE, INPUT_PULLUP);

  if ((EEPROM.readInt(ADR_MOT_MAG) > 1))
    attachInterrupt(digitalPinToInterrupt(PIN_DSPEED), mot_rot, FALLING);
  else
    attachInterrupt(digitalPinToInterrupt(PIN_DSPEED), speed_one, FALLING);

  // UNUSED PINS ANALOG
  pinMode(A2, INPUT_PULLUP); // UNUSED PIN
  pinMode(A6, INPUT_PULLUP); // UNUSED PIN
  pinMode(A7, INPUT_PULLUP); // UNUSED PIN

  // Torque sensor
  pinMode(A3, INPUT);

#ifndef LIGHT_OPERATION
  pinMode(4, INPUT_PULLUP);
#endif

#if not defined LIGHT_OPERATION
  pinMode(9, INPUT_PULLUP);
#endif

#ifdef LIGHT_OPERATION
  pinMode(4, OUTPUT);
  pinMode(9, OUTPUT);
  digitalWrite(4, LOW);
  D4_state = 0;
  digitalWrite(9, LOW);
  D9_state = 0;
#endif

  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);

#ifdef OLED1
  // LCD Init
  screenReset();
  display.display();
#endif

  // EEPROMex
  if (EEPROM.readInt(ADRFIRSTTIME) == FIRST_TIME_CODE)
  // if(1)
  {
    loadData();

#ifdef INITIAL
    display.clearDisplay();
    display.setFont();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(11, 0);
    display.println("LBAD BK"); // Not used characters removed from glcdfont.c / actual characters changed their position
    // display.println("LOAD OK");
    display.display();
    while (true)
      ;
#endif
  }
#ifndef INITIAL
  else
  {
    while (true)
      ;
  }
#endif
#ifdef INITIAL
  else
  {

    for (int i = 2; i < 512; i += 2)
    {
      EEPROM.updateInt(i, 55555);
    }

    initialConfigSave();

    display.clearDisplay();
    display.setFont();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(11, 0);
    display.println("I?I; BK"); // Not used characters removed from glcdfont.c / actual characters changed their position
    // display.println("INIT OK");
    display.display();

    while (true)
      ;
  }
#endif

  // ADS INIT
  ads.begin();

  // Acu Full Charge Check & reset
  float vol = 0.0;

  for (int i = 0; i < 3; i++)
    vol = getBatRawVoltage();

  if (vol > (eepromLoadFloat(ADRLASTVOLTAGE) + 2.0))
  {
    resetBattery();
  }

  // TEMP INIT
  pin_temp1_V_filtered.SetCurrent(0.005 * analogRead(PIN_LM35_TEMP_T1));
  pin_temp2_V_filtered.SetCurrent(0.005 * analogRead(PIN_LM35_TEMP_T2));

  // Enable print out for bluetooth
  EEPROM.updateInt(ADR_PRINTINFO, 1);
  // #endif
#ifdef SERIALPLOT
  // EEPROM.updateInt(ADR_PRINTINFO,0);
  EEPROM.updateInt(ADR_SERIAL_PLOT, 0);
#endif

  // Always start with legal mode
  if (EEPROM.readInt(ADR_AUTOLEGAL) && !EEPROM.readInt(ADR_WATCHDOGRESET))
  {
    EEPROM.updateInt(ADR_LEGALLIMIT_ON_OFF, 1);
  }

  // Watchdog

  enable_watchdog();
  EEPROM.updateInt(ADR_WATCHDOGRESET, 0);
}

void loop()
{
#ifdef TEST_LOOP_SPEED
  timer_loopspeed = millis();
#endif

  // every loop
  wdt_reset();
  serialConfig();
  setThrottle();
  checkButtons();

  // 100ms
  if ((millis() - timer_100ms_loop) > T100MS_LOOP)
  {

    checkVRef();
    set_adc_offset();
    checkEbrake();
    checkTemp();
    checkIfrideOK();
#ifdef PAS
    checkTorque();
#endif

    timer_100ms_loop = millis();
  }

  // 30ms
  speed_one_check();
  checkCurrentAndVoltage();

  // 250ms
  countSpeed();

  // 1000 / 2500ms
  checkIfStopped();

  // 500ms
#ifdef PAS
  checkCadence();
#endif

  // 500ms
  screenRefresh();

  serialPrintInfo();

#ifdef TESTING
  if (millis() - timer_speedtesting > randomspeedms)
  {
    timer_speedtesting = millis();
    mot_rot();
  }

  if (millis() - timer_randomtesting > 2000)
  {
    timer_randomtesting = millis();
    testRandom();
  }
#endif

#ifdef TEST_LOOP_SPEED
  int time = millis() - timer_loopspeed;
  loopcount++;
  looptime += time;
  if (loopcount == 20)
  {
    Serial.println(looptime / 20);
    loopcount = 0;
    looptime = 0;
  }
#endif

  // test watchog
  /*  if (millis() > 10000)
    {
    while(1);
    } */
}
