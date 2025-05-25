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

#ifdef SERIALPLOT
void printComma()
{
  DEBUG_PR(",");
}
#endif

void serialPrintInfo()
{

  if (millis() - timer_send_uart > TIME_SEND_UART)
  {
    if (EEPROM.readInt(ADR_PRINTINFO))
    {
      mySerial.write("MPe;");
      mySerial.printsc(speed, 1);
      mySerial.printsc(distToGo());
      mySerial.printsc(acuPercent());
      mySerial.printsc(trip, 1);
      mySerial.printsc(int(getPower()));
      mySerial.printsc(int(getTemp_1()));
      mySerial.printsc(EEPROM.readInt(ADR_ASSISTMODE));
      mySerial.printsc(int(dist));
      mySerial.printsc(int(avgSpeed()));
      mySerial.printsc(int(vmax));
      mySerial.printsc(int(moving_time));
      mySerial.printsc(getBatVoltage(), 1);
      mySerial.printsc(getCurrent(), 1);
      mySerial.printsc(int(imax));
      mySerial.printsc(int(powermax));
      mySerial.printsc(whkm(), 1);
      mySerial.printsc((EEPROM.readInt(ADR_BATCAP_AH) / 10.0), 1);
      mySerial.printsc((mah_used / 1000), 1);
      mySerial.printsc(int(getTemp_2()));
      mySerial.printsc(numberCharges());
      mySerial.printsc(brake);
      mySerial.printsc(cruisecontrol);
      mySerial.printsc(FIRMWARE_VERSION);
      mySerial.printsc(EEPROM.readInt(ADR_LEGALLIMIT_ON_OFF));
      mySerial.printsc(int(Wh_used));
      mySerial.printsc(!rideOK);
#ifdef PAS
      mySerial.printsc(getCadence());
#endif
#ifndef PAS
      mySerial.printsc(0);
#endif
      mySerial.printsc(getThrottleInputmV());
#ifdef PAS
      mySerial.printsc(analogRead(A3));                        // adc torque sensor
      Serial.print(int(weightOnPedal_adc_filtered.Current())); // kgF
#endif
#ifndef PAS
      mySerial.printsc(0);
      mySerial.print(0);
#endif
      mySerial.println(";");
    }

    timer_send_uart = millis();
  }
}

void serialConfig()
{

  if (Serial.available() > 1)
  {
    int action = 0;
    int address = 400;
    int value = 0;

    action = Serial.parseInt();

    if (action == 6004)
    {
      cruisecontrol = 0;
      SetThrRelResetFALSE();

      action = Serial.parseInt();

      switch (action)
      {
      case 1: // read INT
        address = Serial.parseInt();

        Serial.print("CFG;");
        mySerial.printsc(address);

        Serial.print(EEPROM.readInt(address));
        Serial.println(";");

        break;

      case 2: // save INT
        address = Serial.parseInt();
        value = Serial.parseInt();

        if (address == ADRDIST || address == ADRTOTMAHUSED)
          EEPROM.updateFloat(address, value);
        else if ((address == ADRFIRSTTIME) || (address % 2))
          break;
        else
          EEPROM.updateInt(address, value);
        break;

      case 3:
        resetCurrentAt0();
        break;

      case 4:
        zeroTrip();
        break;

      case 5:
        resetBattery();
        break;

      case 6:
        printAllConfig();
        break;

      case 7:
        enableCruiseControl();
        break;

      case 8:
        cruisecontrol = 1;
        slowDownCruiseControl();
        break;

      case 9:
        cruisecontrol = 1;
        speedUpCruiseControl();
        break;

#ifdef LIGHT_OPERATION
      case 11:
        if (!D4_state && !D9_state && !LCD_dark)
        {
          D4_state = !D4_state;
          digitalWrite(4, D4_state);
          LCD_dark = !LCD_dark;
        }
        else if (D4_state && !D9_state && LCD_dark)
        {
          LCD_dark = !LCD_dark;
        }
        else if (D4_state && !D9_state && !LCD_dark)
        {
          D9_state = !D9_state;
          digitalWrite(9, D9_state);
          LCD_dark = !LCD_dark;
        }
        else if (D4_state && D9_state && LCD_dark)
        {
          D9_state = !D9_state;
          digitalWrite(9, D9_state);
          D4_state = !D4_state;
          digitalWrite(4, D4_state);
          LCD_dark = !LCD_dark;
        }

        break;
#endif
      }
#ifdef TESTING
      saveData();
#endif
    }
  }
}

void printAllConfig()
{

  Serial.print("ALL;");

  for (int i = 200; i < 512; i += 2)
  {
    mySerial.printsc(EEPROM.readInt(i));
  }
  Serial.println("#END");
}
