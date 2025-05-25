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

void checkButtons()
{

  bool b_up = !digitalRead(PIN_BUTTON_UP);
  bool b_dwn = !digitalRead(PIN_BUTTON_DOWN);

  bool test100 = millisMinusTimerButton() > 100;

  // BOTH CLICK

  if (b_up && b_dwn && !b_updwn_p)
  {
    b_updwn_p = true;
    b_dwn_p = true;
    b_dwn_lp = true;
    b_up_p = true;
    b_up_lp = true;
    resetTimerButton();
  }

  if ((!b_up || !b_dwn) && b_updwn_p)
  {
#ifdef OLED1
    screenReset();
#endif
    UPDWNDepressedAction();
    b_updwn_p = false;
    b_dwn_p = false;
    b_dwn_lp = false;
    b_up_p = false;
    b_up_lp = false;
    resetTimerButton();
  }

  // UP CLICK
  if (b_up && !b_up_p && test100)
  {
    b_up_p = true;
    resetTimerButton();
  }

  if (!b_up && b_up_p && !b_up_lp)
  {
    if (test100)
    {
#ifdef OLED1
      screenReset();
#endif
      UPDepressedAction();
    }
    b_up_p = false;
    b_up_lp = false;
  }

  // DOWN CLICK
  if (b_dwn && !b_dwn_p && test100)
  {
    b_dwn_p = true;
    resetTimerButton();
  }

  if (!b_dwn && b_dwn_p && !b_dwn_lp)
  {
    if (test100)
    {
#ifdef OLED1
      screenReset();
#endif
      DWNDepressedAction();
    }
    b_dwn_p = false;
    b_dwn_lp = false;
  }

  // UPDWN LONG CLICK
  if (b_updwn_p && (millisMinusTimerButton() > 2000))
  {
    UPDWNLongPressedAction();
  }

  // UP LONG CLICK
  if (b_up && b_up_p && !b_up_lp)
  {
    if ((millisMinusTimerButton() > 500))
    {
      resetTimerButton();
      b_up_lp = true;
      UPLongPressedAction();
    }
  }

  if (!b_up && b_up_p && b_up_lp)
  {
    // UPLongDepressedAction();
    b_up_p = false;
    b_up_lp = false;
  }

  // DOWN LONG CLICK
  if (b_dwn && b_dwn_p && !b_dwn_lp)
  {
    if (millisMinusTimerButton() > 500)
    {
      b_dwn_lp = true;
      resetTimerButton();
      DWNLongPressedAction();
    }
  }

  if (!b_dwn && b_dwn_p && b_dwn_lp)
  {
    // DWNLongDepressedAction();
    b_dwn_p = false;
    b_dwn_lp = false;
  }
}

void UPDWNDepressedAction()
{
  if (!EEPROM.readInt(ADR_BT_BUTTONS) && !screenconfig)
  {
    statscreen = !statscreen;
    if (screen == 1)
      screen = 2;
    else
      screen = 1;
  }

  if (screenconfig && !screenconfig_just_entered)
  {
    screenconfig = 0;
    statscreen = 0;
    screen = 1;
  }

  if (screenconfig_just_entered)
    screenconfig_just_entered = false;
}

void UPDWNLongPressedAction()
{
  if (!screenconfig)
  {
    screenconfig = true;
    screenconfig_just_entered = true;
  }
}

void UPDepressedAction()
{

#ifdef OLED1

  if (!cruisecontrol && !screenconfig && statscreen)
  {
    if (screen < NUM_SCREEN_POSITIONS)
      screen++;
    else
      screen = 2;
  }

  if (screen == 1 && !screenconfig && !cruisecontrol)
  {
    int assist = EEPROM.readInt(ADR_ASSISTMODE);
    cruisecontrol = false;
    assist += 1;
    if (assist > 5)
      assist = 5;
    EEPROM.updateInt(ADR_ASSISTMODE, assist);
    SetThrRelResetFALSE();
    // powerPID.RESET();
  }

  if (cruisecontrol)
  {
    cruisecontrol = false;
  }

#ifdef CONFIG_MENU
  if (screenconfig && !configchanged)
    selectConfig(configCursor, 1);
  else
    configchanged = false;
#endif

#endif

#ifndef OLED1
  cruisecontrol = false;
#endif
}

void UPLongPressedAction()
{
  speedUpCruiseControl();

#ifdef CONFIG_MENU
  if (screenconfig)
  {

    if (configCursor < 9)
    {
      configCursor += 1;
    }

    resetTimerButton();
    b_up_lp = false;
    configchanged = true;
  }
#endif
}

void DWNDepressedAction()
{
  if (screen == 1 && !screenconfig && !cruisecontrol && !brake)
  {
    int assist = EEPROM.readInt(ADR_ASSISTMODE);
    assist -= 1;
    if (assist < 0)
      assist = 0;
    EEPROM.updateInt(ADR_ASSISTMODE, assist);
    SetThrRelResetFALSE();
  }

#ifdef OLED1
  if ((!cruisecontrol && !screenconfig && statscreen))
  {
    if (screen > 2)
      screen--;
    else
      screen = NUM_SCREEN_POSITIONS;
  }

  if (cruisecontrol)
  {
    cruisecontrol = false;
  }

#ifdef CONFIG_MENU
  if (screenconfig && !configchanged)
    selectConfig(configCursor, 0);
  else
    configchanged = false;
#endif

#endif

#ifndef OLED1
  cruisecontrol = false;
#endif
}

void DWNLongPressedAction()
{
  if (!screenconfig)
    enableCruiseControl();

  slowDownCruiseControl();

#ifdef PAS
  if (screen == 1 && brake)
  {
    if (EEPROM.readInt(ADR_LEGALLIMIT_ON_OFF))
      EEPROM.updateInt(ADR_LEGALLIMIT_ON_OFF, 0);
    else
      EEPROM.updateInt(ADR_LEGALLIMIT_ON_OFF, 1);
  }

#endif

#ifdef OLED1
  if (screen == 2 && !screenconfig)
  {
    zeroTrip();
  }

  if (screen == 3 && !screenconfig)
  {
    resetCurrentAt0();
  }

  if (screen == 4 && !screenconfig)
  {
    resetBattery();
  }

#endif

#ifdef CONFIG_MENU

  if (screenconfig)
  {

    if (configCursor > 1)
    {
      configCursor -= 1;
    }

    resetTimerButton();
    b_dwn_lp = false;
    configchanged = true;
  }

#endif
}

void resetTimerButton()
{
  timer_button = millis();
}

unsigned int millisMinusTimerButton()
{
  return millis() - timer_button;
}
