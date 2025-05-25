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

#ifdef OLED1
void screenReset()
{

  if (!screenconfig)
  {
    display.begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS, false);
  }
}
#endif

void screenRefresh()
{
#ifndef SERIALPLOT
  if ((millis() - timer_refresh > SCREENREFRESH) && (millis() > 2500))
#endif

#ifdef SERIALPLOT
    if ((millis() - timer_refresh > SCREENREFRESH) && (millis() > 2500) && (!EEPROM.readInt(ADR_SERIAL_PLOT)))
#endif
    {
      timer_refresh = millis();
#ifdef OLED1
      display.clearDisplay();
      // SCREEN RESOLUTION 128x32

      switch (screen)
      {
      case 1:
        screen1();
        break;
      case 2:
        screen2();
        break;
      case 3:
        screen3();
        break;
      case 4:
        screen4();
        break;
      case 5:
        screen5();
        break;

#ifdef MTG
      case 6:
        screen_MhWh();
        break;
#endif
      }

#ifdef CONFIG_MENU
      if (screenconfig)
      {
        display.clearDisplay();
        screenConfig();
      }
#endif

      display.display();
#endif
    }
}

#ifdef OLED1

void setFont2WHITE()
{
  display.setFont();
  display.setTextSize(2);
  display.setTextColor(WHITE);
}

void screen1()
{
  display.setFont();
  display.setRotation(0);
  display.setTextSize(2);

  // ASSIST MODE
  if (EEPROM.readInt(ADR_LEGALLIMIT_ON_OFF))
  {
    display.fillRect(114, 16, 12, 16, WHITE);
    display.setTextColor(BLACK);
  }
  display.setCursor(115, 17);
  display.println(EEPROM.readInt(ADR_ASSISTMODE));

  display.setTextColor(WHITE);

#ifndef MTG
  display.setCursor(intAlignRight(int(distToGo())), 0);
  display.println(int(distToGo()));
#endif

#ifdef MTG
  display.setCursor(intAlignRight(int(minutesToGo())), 0);
  display.println(int(minutesToGo()));
#endif

  display.setCursor(64, 18); // decimal speed
  display.println(int(speed * 10) % 10);

  display.setCursor(65, 0);
  char txt = ' ';
  if (brake)
    txt = '*';
  else if (cruisecontrol && rideOK)
    txt = 'J'; // Not used characters removed from glcdfont.c / actual characters changed their position
  else if (!rideOK)
    txt = '!';

  display.println(txt);

  // Descriptions
  display.setTextSize(1);
  display.setRotation(3);

  // SPEED UNIT
  display.setCursor(0, 0);

  if (EEPROM.readInt(ADR_KPHMPH))
  {
    // display.println("mph");
    display.println("$%'"); // Not used characters removed from glcdfont.c / actual characters changed their position
    display.setCursor(19, 118);
    // display.println("mi");
    display.println("$\""); // Not used characters removed from glcdfont.c / actual characters changed their position
  }
  else
  {
    // display.println("km/h");
    display.println("#$/'"); // Not used characters removed from glcdfont.c / actual characters changed their position
    display.setCursor(19, 118);
#ifndef MTG
    // display.println("km");
    display.println("#$"); // Not used characters removed from glcdfont.c / actual characters changed their position
#endif
#ifdef MTG
    // display.println("mt");
    display.println("$&"); // Not used characters removed from glcdfont.c / actual characters changed their position
#endif
  }

  display.setRotation(0);
  display.setFont(&DejaVu_LGC_Sans_Mono_Bold_20); // speed
  display.setTextSize(2);
  display.setCursor(10, 32);

  if (int(speed) < 10)
    display.print(0);

  if (speed >= 100)
  {
    speed = int(speed) % 100;
    display.fillRect(0, 0, 8, 32, BLACK);
    display.setCursor(-10, 32);
    display.print(1);
    display.setCursor(10, 32);
    if (speed < 10)
      display.print(0);
  }
  display.println(int(speed));

  display.fillRect(60, 30, 1, 2, WHITE); // coma

#define STARTX 80 // battery symbol   size
#define STARTY 16
#define SIZEX 29
#define SIZEY 16
#define SIZEXF 43

  display.drawRect(STARTX, STARTY, SIZEX, SIZEY, WHITE);
  display.fillRect(STARTX + SIZEX, STARTY + 2, 2, SIZEY - 4, WHITE);
  display.fillRect(STARTX + 2, STARTY + 2, (SIZEX - 4) * acuPercent() / 100, SIZEY - 4, WHITE);
  display.drawRect(STARTX + int((SIZEX) / 4), STARTY + 2, 1, SIZEY - 4, BLACK);       // 10     STARTX+ int((SIZEX-4)/4)
  display.drawRect(STARTX + (int((SIZEX) / 4)) * 2, STARTY + 2, 1, SIZEY - 4, BLACK); // 19
  display.drawRect(STARTX + (int((SIZEX) / 4)) * 3, STARTY + 2, 1, SIZEY - 4, BLACK); // 28
}

void screen2()
{
  setFont2WHITE();

  // MAX Speed  //TOP LEFT
  display.setCursor(11, 0);
  display.println(vmax, 0);

  // Average Speed  //DOWN LEFT
  display.setCursor(11, 16);
  display.println(avgSpeed(), 0);

  // Trip distance  //TOP RIGHT
  display.setCursor(floatAlignRight(trip), 0);
  display.println(trip, 1);

  // Moving time  //DOWN RIGHT
  display.setCursor(intAlignRight(moving_time), 16);
  display.println(moving_time, 0);

  // Descriptions
  display.setRotation(3);
  display.setTextSize(1);

  // TOP LEFT
  display.setCursor(19, 0);
  // display.println("MX");
  display.println("@("); // Not used characters removed from glcdfont.c / actual characters changed their position

  // DOWN LEFT
  display.setCursor(3, 0);
  // display.println("AV");
  display.println("A:"); // Not used characters removed from glcdfont.c / actual characters changed their position

  // TOP RIGHT
  display.setCursor(19, 119);
  // display.println("TR");
  display.println(";="); // Not used characters removed from glcdfont.c / actual characters changed their position

  // DOWN RIGHT
  display.setCursor(3, 119);
  // display.println("MT");
  display.println("@;"); // Not used characters removed from glcdfont.c / actual characters changed their position

  display.setRotation(0);
}

void screen3()
{
  setFont2WHITE();

  // Voltage  //TOP LEFT
  display.setCursor(11, 0);
  if (getBatVoltage() > 100.0 || getPower() > 9999)
    display.println(getBatVoltage(), 0);
  else
    display.println(getBatVoltage(), 1);

  // Current  //DOWN LEFT
  display.setCursor(11, 16);
  if (getCurrent() > 100.0 || powermax > 9999)
    display.println(getCurrent(), 0);
  else
    display.println(getCurrent(), 1);

  // Power  //TOP RIGHT
  float pwr = abs(getPower());
  display.setCursor(intAlignRight(pwr), 0);
  display.println(pwr, 0);

  // MAX Power  //DOWN RIGHT
  display.setCursor(intAlignRight(powermax), 16);
  display.println(powermax, 0);

  // Descriptions
  display.setRotation(3);
  display.setTextSize(1);

  // TOP LEFT
  display.setCursor(22, 0);
  // display.println("U");
  display.println(")"); // Not used characters removed from glcdfont.c / actual characters changed their position

  // DOWN LEFT
  display.setCursor(6, 0);
  display.println("I"); // Not used characters removed from glcdfont.c / actual characters changed their position

  // TOP RIGHT
  display.setCursor(22, 119);
  // display.println("P");
  display.println(">"); // Not used characters removed from glcdfont.c / actual characters changed their position

  // DOWN RIGHT
  display.setCursor(3, 119);
  // display.println("PM");
  display.println(">@"); // Not used characters removed from glcdfont.c / actual characters changed their position

  display.setRotation(0);
}

void screen4()
{
  setFont2WHITE();

  // I MAX   //TOP LEFT
  display.setCursor(11, 0);
  display.println(imax, 0);

  // Wh / km  //DOWN LEFT
  display.setCursor(11, 16);
  display.println(whkm(), 1);

  // Wh used  //TOP RIGHT
  display.setCursor(intAlignRight(int(Wh_used)), 0);
  display.println(Wh_used, 0);

  // Amp hours used  //DOWN RIGHT
  display.setCursor(floatAlignRight(mah_used / 1000), 16);
  display.println(mah_used / 1000, 1);

  // Descriptions
  display.setRotation(3);
  display.setTextSize(1);

  // TOP LEFT
  display.setCursor(19, 0);
  // display.println("IM");
  display.println("I@"); // Not used characters removed from glcdfont.c / actual characters changed their position

  // DOWN LEFT
  display.setCursor(3, 0);
  // display.println("WK");
  display.println("+K"); // Not used characters removed from glcdfont.c / actual characters changed their position

  // TOP RIGHT
  display.setCursor(19, 119);
  // display.println("WU");
  display.println("+)"); // Not used characters removed from glcdfont.c / actual characters changed their position

  // DOWN RIGHT
  display.setCursor(3, 119);
  // display.println("AU");
  display.println("A)"); // Not used characters removed from glcdfont.c / actual characters changed their position

  display.setRotation(0);
}

void screen5()
{
  setFont2WHITE();

  // Temp Controller  //TOP LEFT
  display.setCursor(11, 0);
  display.println(getTemp_1());

  // Temp Motor  //DOWN LEFT
  display.setCursor(11, 16);
  display.println(getTemp_2());

  // Overall distance  //TOP RIGHT
  display.setCursor(intAlignRight(int(dist)), 0);
  display.println(dist, 0);

  // Number of charging cycles  //DOWN RIGHT
  display.setCursor(intAlignRight(numberCharges()), 16);
  display.println(numberCharges());

  // Descriptions
  display.setRotation(3);
  display.setTextSize(1);

  // TOP LEFT
  display.setCursor(19, 0);
  // display.println("T1");
  display.println(";1"); // Not used characters removed from glcdfont.c / actual characters changed their position

  // DOWN LEFT
  display.setCursor(3, 0);
  // display.println("T2");
  display.println(";2"); // Not used characters removed from glcdfont.c / actual characters changed their position

  // TOP RIGHT
  display.setCursor(19, 119);
  // display.println("DS");
  display.println("D<"); // Not used characters removed from glcdfont.c / actual characters changed their position

  // DOWN RIGHT
  display.setCursor(3, 119);
  // display.println("NC");
  display.println("?C"); // Not used characters removed from glcdfont.c / actual characters changed their position

  display.setRotation(0);
}

#endif

#ifdef MTG
void screen_MhWh()
{
  setFont2WHITE();

  // Temp Controller  //TOP LEFT
  display.setCursor(11, 0);
  display.println(total_mh, 1);

  // Temp Motor  //DOWN LEFT
  display.setCursor(11, 16);
  display.println(whhour(), 0);

  // Descriptions
  display.setRotation(3);
  display.setTextSize(1);

  // TOP LEFT
  display.setCursor(19, 0);
  display.println("@H"); // Not used characters removed from glcdfont.c / actual characters changed their position

  // DOWN LEFT
  display.setCursor(3, 0);
  display.println("+H"); // Not used characters removed from glcdfont.c / actual characters changed their position

  display.setRotation(0);
}
#endif

#ifdef CONFIG_MENU
void screenConfig()
{

  setFont2WHITE();

  displayConfigValue(address, 3, 0);

  int value = 0;

  switch (address)
  {
  case 0:
    value = FIRMWARE_VERSION;
    break;

#ifdef PAS
  case 999:
    value = getCadence();
    break;
#endif

  case 998:
    value = getThrottleInputmV();
    break;

  case 997:
    value = analogRead(A3);
    break;

  case 996:
    value = int(weightOnPedal_adc_filtered.Current());
    break;

  default:
    value = EEPROM.readInt(getCfgAddress(address));
    break;
  }

  displayConfigValue(value, 5, 1);

  int x, y = 0;
  x = (configCursor - 1) * 12;
  y = 22;

  display.fillRect(x, y, 10, 2, WHITE);
}
#endif

#ifdef CONFIG_MENU
void displayConfigValue(unsigned int value, int maxlength, int startplace)
{
  int x = 0;
  int y = 6;

  if (startplace == 1)
    x = 48;

  display.setCursor(x, y);

  if (value < 10000 && maxlength == 5)
    display.print(0);

  if (value < 1000 && maxlength == 5)
    display.print(0);

  if (value < 100)
    display.print(0);

  if (value < 10)
    display.print(0);

  display.println(value);
}
#endif

#ifdef CONFIG_MENU
void selectConfig(int conf, bool updwn)
{
  int _address = getCfgAddress(address);

  if (conf < 4)
    address = changeConfigValue(address, updwn, conf);
  else
    EEPROM.updateInt(_address, changeConfigValue(EEPROM.readInt(_address), updwn, conf));
}

unsigned int changeConfigValue(unsigned int value, bool updwn, int place)
{
  unsigned int zero_nine = 0;
  unsigned int plus_minus = -1;

  if (updwn)
  {
    zero_nine = 9;
    plus_minus = 1;
  }

  if (updwn && place == 5)
    zero_nine = 6;

  switch (place)
  {
  case 3:
  case 9:
    if (value % 10 == zero_nine)
      value -= plus_minus * 9;
    else
      value += plus_minus * 1;
    break;
  case 2:
  case 8:
    if ((value / 10) % 10 == zero_nine)
      value -= plus_minus * 90;
    else
      value += plus_minus * 10;
    break;
  case 1:
  case 7:
    if ((value / 100) % 10 == zero_nine)
      value -= plus_minus * 900;
    else
      value += plus_minus * 100;
    break;
  case 6:
    if ((value / 1000) % 10 == zero_nine)
      value -= plus_minus * 9000;
    else
      value += plus_minus * 1000;
    break;
  case 5:
    if ((value / 10000) % 10 == zero_nine)
      value -= plus_minus * 60000;
    else
      value += plus_minus * 10000;
    break;
  }

  return value;
}

#endif

int intAlignRight(int value)
{

  int ret = 0;

  if (value < 10)
    ret = 105;

  if (value >= 10 && value < 100)
    ret = 93;

  if (value >= 100 && value < 1000)
    ret = 81;

  if (value >= 1000)
    ret = 69;

  if (value >= 10000)
    ret = 57;

  return ret;
}

int floatAlignRight(float value)
{
  int ret = 0;

  if (value < 10.0)
    ret = 105 - 24;

  if (value >= 10.0 && value < 100.0)
    ret = 93 - 24;

  if (value >= 100.0 && value < 1000.0)
    ret = 81 - 24;

  if (value >= 1000.0)
    ret = 69 - 24;

  return ret;
}
