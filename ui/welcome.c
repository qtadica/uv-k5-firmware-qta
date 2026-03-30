/* Copyright 2023 Dual Tachyon
 * https://github.com/DualTachyon
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string.h>
#include "driver/eeprom.h"
#include "driver/st7565.h"
#include "external/printf/printf.h"
#include "helper/battery.h"
#include "settings.h"
#include "misc.h"
#include "ui/helper.h"
#include "ui/welcome.h"
#include "ui/status.h"
#include "version.h"
#include "bitmaps.h"

void UI_DisplayReleaseKeys(void)
{
    UI_DisplayClear();
    UI_PrintString("RELEASE", 0, 127, 1, 10);
    UI_PrintString("ALL KEYS", 0, 127, 3, 10);
    ST7565_BlitFullScreen();
}

void UI_DisplayWelcome(void)
{
    char WelcomeString0[17];
    char WelcomeString1[17];
    char BatteryString[32];

    UI_DisplayClear();

    // 1. CLEAR THE STATUS BAR (Stops the battery icon from appearing over logo)
    memset(gStatusLine, 0, sizeof(gStatusLine));

    // 2. DRAW LOGO (Lines 0, 1, 2)
    memcpy(gStatusLine, g_qta_logo_short, 128);
    memcpy(gFrameBuffer[0], g_qta_logo_short + 128, 128);
    memcpy(gFrameBuffer[1], g_qta_logo_short + 256, 128);

    // 3. READ CHIRP TEXT
    memset(WelcomeString0, 0, sizeof(WelcomeString0));
    memset(WelcomeString1, 0, sizeof(WelcomeString1));
    EEPROM_ReadBuffer(0x0EB0, WelcomeString0, 16);
    EEPROM_ReadBuffer(0x0EC0, WelcomeString1, 16);

    // 4. PREPARE VERSION & BATTERY (Now safe to include Version!)
    // This will look like: "v1.0  8.40V 100%"
    sprintf(BatteryString, "%s  %u.%02uV %u%%", 
            Version,
            gBatteryVoltageAverage / 100, 
            gBatteryVoltageAverage % 100, 
            BATTERY_VoltsToPercent(gBatteryVoltageAverage));

    // 5. PRINT TEXT (Using Lines 4, 5, and 7 to stay away from the logo)
    // If CHIRP lines are empty, we give them a default value
    if(strlen(WelcomeString0) == 0) strcpy(WelcomeString0, "WELCOME");
    if(strlen(WelcomeString1) == 0) strcpy(WelcomeString1, "QTA MOD");

    UI_PrintStringSmallNormal(WelcomeString0, 0, 127, 4);
    UI_PrintStringSmallNormal(WelcomeString1, 0, 127, 5);
    UI_PrintStringSmallNormal(BatteryString,  0, 127, 7);

    // 6. FINAL PUSH TO SCREEN
    ST7565_BlitStatusLine();
    ST7565_BlitFullScreen();
}
