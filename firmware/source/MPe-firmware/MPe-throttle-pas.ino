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

int16_t getThrottleInputmV()
{
  return ads.readADC_SingleEnded(PIN_THROTTLE_IN) * 0.01875;
}

void setThrottle()
{
#ifdef PAS
  // PID Compute
  float pwr = 0.0;
  pwr = getPower();

  if (pwr < 0)
    pwr = 0;

  pPID_In = pwr;

  pidTune();

#ifndef SERIALPLOT
  powerPID.Compute();
  pwmOut = (int)pPID_Out;
#endif

  // Serial Print for graph plotting
#ifdef SERIALPLOT
  bool plot_print = powerPID.Compute(); // synchoronize print with PID compute
  float power_in = pPID_In;
  pwmOut = (int)pPID_Out;
#endif

#endif

  unsigned int tot_min = EEPROM.readInt(ADR_TOT_MIN);
  unsigned int tot_max = EEPROM.readInt(ADR_TOT_MAX);
  unsigned int tin_min = EEPROM.readInt(ADR_TIN_MIN);
  unsigned int tin_max = EEPROM.readInt(ADR_TIN_MAX);

  tot_min = constrain(tot_min, 60, 120); // prevent motor from self running due to set wrong values or eeprom corruption
  tot_max = constrain(tot_max, 300, 430);
  tin_min = constrain(tin_min, 70, 150);
  tin_max = constrain(tin_max, 300, 430);

  int pwmout_min = int((((tot_min / 100.0) * 255.0) / 4.75)); // 4.75V is reference voltage - 0.25V due to voltage drop on TOT connector when connetced to controller. It has to be fixed value (4.75) becouse damaged ADS1115 causes motor to self run if this value was set as vRef
  int pwmout_max = int((((tot_max / 100.0) * 255.0) / 4.75));

  int thrinminPLUS = tin_min + 5;

  int16_t pin_thrIn_mV = 0;
  pin_thrIn_mV = getThrottleInputmV();

  bool thumb_throttle_active = pin_thrIn_mV > thrinminPLUS;

  int assistmode = EEPROM.readInt(ADR_ASSISTMODE);
  int throttlemode = 0;
  int assistmodeadd = 2 * (assistmode - 1);

  // Cruise control deactivation from throttle input
  if (cruisecontrol && thumb_throttle_active)
  {
    cruisecontrol = false;
  }
  //===

  // Throttle release reset after brake pressed
  if (throttle_relased_reset == false && !thumb_throttle_active)
    throttle_relased_reset = true;
  //===

  if (rideOK)
  {
    bool legallimit = EEPROM.readInt(ADR_LEGALLIMIT_ON_OFF);
    unsigned int set = 0;

    // SpeedFactor selector variables
    unsigned int spd_max = 0;
    unsigned int spd_max_min5 = 0;
    unsigned int CC_max_speed = 0;
    unsigned int power = 0;
    unsigned int cadence_min = 0;
    unsigned int cadence_max = 0;
    unsigned int ramp_up_dwn = 0;
    //===

#ifdef PAS
    int cadence = getCadence();
#endif

#ifndef PAS
    int cadence = 0;
#endif

    // Throttle mode selector
    if (cruisecontrol && !legallimit) // cruisecontrol
    {
      throttlemode = 4;
    }
    else if ((!thumb_throttle_active || legallimit) && !cruisecontrol && ((cadence > EEPROM.readInt(ADR_CAD_MIN_PAS_1 + assistmodeadd) && checkMinimumHumanPower()) || checkStartupWeightOnPedal())) // PAS
    {
      throttlemode = 3;
    }
    else if ((EEPROM.readInt(ADR_MODE_THR_1 + assistmodeadd) == 1) && !legallimit) // throttle power
    {
      throttlemode = 2;
    }
    else if ((EEPROM.readInt(ADR_MODE_THR_1 + assistmodeadd) == 0) && !legallimit) // throttle voltage
    {
      throttlemode = 1;
    }
    //===

    switch (throttlemode)
    {
    case 1: // throttle voltage

#ifdef PAS
      pPID_Set = 0;
      ZeroNoPedallingTimer();
#endif

      if (throttle_relased_reset)
      {

        unsigned int pwr_limit = EEPROM.readInt(ADR_PWR_LIM_THR_1 + assistmodeadd);
        pwr_limit = constrain(pwr_limit, 0, 100);

        set = mapConstrain(pin_thrIn_mV, tin_min, tin_max, tot_min, tot_max * 0.01 * pwr_limit);

        voltageOut = valueDelay(voltageOut, set, EEPROM.readInt(ADR_RAMP_UP_THR_1 + assistmodeadd), 10000, 1);

        pwmOut = mapConstrain(voltageOut, tot_min, tot_max, pwmout_min, pwmout_max);
      }
      else
        pwmOut = pwmout_min;

      break;

#ifdef PAS
    case 2: // throttle power

      pas_after_power_throttle = true;

      power = EEPROM.readInt(ADR_PWR_LIM_THR_1 + assistmodeadd);
      power = constrain(power, 0, 1000);

      if (throttle_relased_reset)
      {
        set = mapConstrain(pin_thrIn_mV, tin_min, tin_max, 0, power);

        pPID_Set = valueDelay(pPID_Set, set, EEPROM.readInt(ADR_RAMP_UP_THR_1 + assistmodeadd), 5000, 1);
      }
      else
        pPID_Set = 0;

      break;
#endif

#ifdef PAS
    case 3: // PAS

      if (pas_after_power_throttle)
      {
        pPID_Set = 0;
        pas_after_power_throttle = false;
      }

      if ((speed >= EEPROM.readInt(ADR_MIN_SPD_PAS_1 + assistmodeadd) && speed >= 0.15) || checkStartupWeightOnPedal())
      {
        spd_max = EEPROM.readInt(ADR_SPD_LIM_PAS_1 + assistmodeadd);
        if (spd_max > 50)
          spd_max = 50;
        spd_max_min5 = spd_max - 5;

        cadence_min = EEPROM.readInt(ADR_CAD_MIN_PAS_1 + assistmodeadd);
        cadence_max = EEPROM.readInt(ADR_CAD_MAX_PAS_1 + assistmodeadd);

        power = EEPROM.readInt(ADR_PWR_LIM_PAS_1 + assistmodeadd);

        // SET POWER FROM CADENCE AND SPEED - START
        if (legallimit)
        {

          if (spd_max > EEPROM.readInt(ADR_LEGALLIMIT_SPEED))
          {
            spd_max = EEPROM.readInt(ADR_LEGALLIMIT_SPEED);
            spd_max_min5 = spd_max - 5;
          }

          if (power > EEPROM.readInt(ADR_LEGALLIMIT_POWER))
            power = EEPROM.readInt(ADR_LEGALLIMIT_POWER);
        }

        ramp_up_dwn = EEPROM.readInt(ADR_SPEEDFACTOR_RAMP_UP);

        set = int(mapConstrain(int(speed * 10), spd_max_min5 * 10, spd_max * 10, 100, EEPROM.readInt(ADR_SPEEDFACTORMIN)));
        speedFactor = valueDelay(speedFactor, set, ramp_up_dwn, ramp_up_dwn, 1);

        ramp_up_dwn = EEPROM.readInt(ADR_RAMP_UP_PAS_1 + assistmodeadd);
        set = int(mapConstrain(getCadence(), cadence_min, cadence_max, 0, power) * (speedFactor / 100.0));
        // SET POWER FROM CADENCE AND SPEED - END

        // SET POWER FROM TORQUE SENSOR, CADENCE AND SPEED - START
        if (power <= 20 && EEPROM.readInt(ADR_ENABLE_TORQUE_SENSOR))
          set = int(humanPower_adc_filtered.Current() * power * 0.5 * (speedFactor / 100.0));

        if (legallimit)
        {
          if (set > EEPROM.readInt(ADR_LEGALLIMIT_POWER))
            set = EEPROM.readInt(ADR_LEGALLIMIT_POWER);
        }
        // SET POWER FROM TORQUE SENSOR, CADENCE AND SPEED - END

        if (set > 3000)
          set = 3000;

        unsigned long zero_cadence_time = millis() - timer_no_pedalling;

        // PAS FAST_STARTUP START
        if (zero_cadence_time < 300)
        {
          if ((cadence > cadence_min && checkMinimumHumanPower()) || checkStartupWeightOnPedal())
          {
            set = 350;
            ramp_up_dwn = 5000;
          }
        }
        // PAS FAST_STARTUP END

        // PAS BOOST START
        if (!legallimit)
        {
          if (zero_cadence_time < EEPROM.readInt(ADR_BOOST_TIME_PAS_1 + assistmodeadd))
          {
            if (int(speed) < EEPROM.readInt(ADR_BOOST_SPEED_PAS_1 + assistmodeadd))
            {
              if (cadence > cadence_min)
              {
                set = EEPROM.readInt(ADR_BOOST_POWER_PAS_1 + assistmodeadd);
                ramp_up_dwn = EEPROM.readInt(ADR_BOOST_RAMP_UP_PAS);
              }
            }
          }
        }
        // PAS BOOST END

        // Finally set the PID for PAS
        pPID_Set = float(valueDelay(int(pPID_Set), set, ramp_up_dwn, ramp_up_dwn, 2));
      }
      else
        pPID_Set = 0;

      break;
#endif

#ifdef PAS
    case 4: // cruisecontrol

      spd_max = cruisecontrol_speed;
      spd_max_min5 = spd_max - 5;

      CC_max_speed = EEPROM.readInt(ADR_CRUISE_CONTROL_MAX_SPEED);

      if (CC_max_speed > 45)
        CC_max_speed = 45;

      power = mapConstrain(spd_max, 5, CC_max_speed, EEPROM.readInt(ADR_CRUISE_CONTROL_POWER_MIN), EEPROM.readInt(ADR_CRUISE_CONTROL_POWER_MAX));

      if (power > 2000)
        power = 2000;

      ramp_up_dwn = EEPROM.readInt(ADR_SPEEDFACTOR_RAMP_UP);

      set = int(mapConstrain(int(speed * 10), spd_max_min5 * 10, spd_max * 10, 100, EEPROM.readInt(ADR_SPEEDFACTORMIN)));
      speedFactor = valueDelay(speedFactor, set, ramp_up_dwn, ramp_up_dwn, 1);

      ramp_up_dwn = EEPROM.readInt(ADR_CRUISE_CONTROL_POWER_RAMP_UP);
      set = int(power * (speedFactor / 100.0));

      pPID_Set = float(valueDelay(int(pPID_Set), set, ramp_up_dwn, ramp_up_dwn, 2));

      break;
#endif

    default:
#ifdef PAS
      pPID_Set = 0;
#endif
      pwmOut = pwmout_min;
      break;
    }
#ifdef PAS
    if (pPID_Set < 5)
      powerPID.RESET();
#endif

    analogWrite(PIN_THROTTLE_OUT, pwmOut);
  }
  else
  {
#ifdef PAS

    pPID_In = 20000;
    pPID_Set = 0;
    powerPID.RESET();
#endif
    cruisecontrol = false;

    voltageOut = 80;

    analogWrite(PIN_THROTTLE_OUT, pwmout_min);
  }

#ifdef SERIALPLOT
  if (plot_print && EEPROM.readInt(ADR_SERIAL_PLOT))
  {
    DEBUG_PR(int(power_in));
    printComma();
    DEBUG_PR(int(pPID_Set));
    printComma();
    DEBUG_PR(int(speed * 10));
    printComma();
    DEBUG_PR(int(analogRead(A3)));
    printComma();
    DEBUG_PR(int(weightOnPedal_adc_filtered.Current()));
    printComma();
    DEBUG_PR(int(getCadence()));
    printComma();
    DEBUG_PRLN(int(humanPower_adc_filtered.Current()));
  }
#endif
}

void checkIfrideOK()
{
  int assistmode = EEPROM.readInt(ADR_ASSISTMODE);

  int16_t pin_thrIn_mV = 0;
  pin_thrIn_mV = ads.readADC_SingleEnded(PIN_THROTTLE_IN) * 0.01875;

  if (
      (getBatVoltage() >= EEPROM.readInt(ADR_LVC) / 10.0) && temperatureOK && !brake && (assistmode > 0) && (getCurrent() >= -((float)EEPROM.readInt(ADR_CUR_PROT))) && (pin_thrIn_mV < EEPROM.readInt(ADR_THR_SAFE_VOLTAGE))) // 370))

    rideOK = true;

  else

    rideOK = false;
}

void SetThrRelResetFALSE()
{
  throttle_relased_reset = false;
}

void enableCruiseControl()
{
  if (!cruisecontrol && screen == 1 && !EEPROM.readInt(ADR_LEGALLIMIT_ON_OFF) && (EEPROM.readInt(ADR_CRUISE_CONTROL_MAX_SPEED) > 0))
  {
    cruisecontrol = 1;
    cruisecontrol_speed = speed + 3.5;
    if (cruisecontrol_speed > 45.0)
      cruisecontrol_speed = 45.0;
  }
}

void slowDownCruiseControl()
{
  if (cruisecontrol && cruisecontrol_speed > 6.0)
    cruisecontrol_speed -= 2.0;
}

void speedUpCruiseControl()
{
  if (cruisecontrol && cruisecontrol_speed < 45.0 && !screenconfig)
    cruisecontrol_speed += 2.0;
}

void ZeroNoPedallingTimer()
{
  timer_no_pedalling = millis();
}

bool checkStartupWeightOnPedal()
{
  int min_spd = EEPROM.readInt(ADR_MIN_SPD_PAS_1 + (2 * (EEPROM.readInt(ADR_ASSISTMODE) - 1)));
  if (EEPROM.readInt(ADR_ENABLE_TORQUE_SENSOR))
    return weightOnPedal_adc_filtered.Current() > EEPROM.readInt(ADR_STARTUP_WEIGHT_ON_PEDAL) && speed < 5.0 && speed >= min_spd;
  else
    return 0;
}

bool checkMinimumHumanPower()
{
  int power = EEPROM.readInt(ADR_PWR_LIM_PAS_1 + (2 * (EEPROM.readInt(ADR_ASSISTMODE) - 1)));
  if (EEPROM.readInt(ADR_ENABLE_TORQUE_SENSOR) && power < 21)
    return humanPower_adc_filtered.Current() > 3 || weightOnPedal_adc_filtered.Current() > 10;
  else
    return 1;
}

#ifdef PAS
void checkTorque()
{
  int analog_torque = analogRead(A3);
  weightOnPedal_adc_filtered.Filter(mapConstrain(analog_torque, EEPROM.readInt(ADR_TORQUE_SENSOR_ADC_MIN), EEPROM.readInt(ADR_TORQUE_SENSOR_ADC_MAX), 0, EEPROM.readInt(ADR_TORQUE_SENSOR_KGF_MAX))); // max torque sensor force is 60kgF, multiplied by 10 to have decimal point values as int

  humanPower_adc_filtered.Filter((weightOnPedal_adc_filtered.Current() * getCadence()) / 55);

  // if just rotatnig cranks without force
  if (!checkMinimumHumanPower())
    ZeroNoPedallingTimer();
}
#endif

#ifdef PAS
void pas_rot()
{
  pas_rotation_impulses++;
}

int getCadence()
{
  return pas_rpm_ex.Current();
}

void checkCadence()
{
  int time = millis() - timer_pas_rpm;
  int impulses = 0;
  float pas_rps = 0.0;
  float pas_rpm = 0.0;

  int interval = EEPROM.readInt(ADR_CADENCE_COUNT_TIME);

  if (time >= interval)
  {

    detachInterrupt(PIN_PAS);

    impulses = pas_rotation_impulses;
    pas_rotation_impulses = 0;
    timer_pas_rpm = millis();

    attachInterrupt(digitalPinToInterrupt(PIN_PAS), pas_rot, CHANGE);

    pas_rps = (impulses * 500.0 / time) / (EEPROM.readInt(ADR_PASMAGNETS));

    pas_rpm = pas_rps * 60.0;

    pas_rpm_ex.Filter(pas_rpm);

    if (impulses == 0)
    {
      pas_rpm_ex.SetCurrent(0);
      ZeroNoPedallingTimer();
      if (!cruisecontrol)
        speedFactor = 100;
    }
  }
}

#endif

#ifdef PAS
void pidTune()
{

  int spd = 1;

  if (abs(pPID_Set - pPID_In) >= EEPROM.readInt(ADR_LOW_THRESHOLD))
    spd = 0;

  switch (spd)
  {
  case 0: // power PID fast
    powerPID.SetTunings(EEPROM.readInt(ADR_POWERKP) / 10000.0, EEPROM.readInt(ADR_POWERKI) / 1000.0, EEPROM.readInt(ADR_POWERKD) / 100000.0, 1);
    break;

  case 1: // power PID slow
    powerPID.SetTunings(EEPROM.readInt(ADR_P_LOW) / 10000.0, EEPROM.readInt(ADR_I_LOW) / 1000.0, EEPROM.readInt(ADR_D_LOW) / 100000.0, 1);
    break;
  }
}
#endif
