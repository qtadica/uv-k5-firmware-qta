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
    char BatteryString[17];

    UI_DisplayClear();

    // 1. DRAW LOGO (We know this works!)
    memcpy(gStatusLine, g_qta_logo_short, 128);
    memcpy(gFrameBuffer[0], g_qta_logo_short + 128, 128);
    memcpy(gFrameBuffer[1], g_qta_logo_short + 256, 128);

    // 2. READ CHIRP TEXT
    memset(WelcomeString0, 0, sizeof(WelcomeString0));
    memset(WelcomeString1, 0, sizeof(WelcomeString1));
    EEPROM_ReadBuffer(0x0EB0, WelcomeString0, 16);
    EEPROM_ReadBuffer(0x0EC0, WelcomeString1, 16);

    // 3. PREPARE BATTERY TEXT (Simplified to prevent crashing)
    sprintf(BatteryString, "%u.%02uV %u%%", 
            gBatteryVoltageAverage / 100, 
            gBatteryVoltageAverage % 100, 
            BATTERY_VoltsToPercent(gBatteryVoltageAverage));

    // 4. PRINT TO SCREEN
    // We use line 4, 5, and 7
    UI_PrintStringSmallNormal(WelcomeString0, 0, 127, 4);
    UI_PrintStringSmallNormal(WelcomeString1, 0, 127, 5);
    UI_PrintStringSmallNormal(BatteryString,  0, 127, 7);

    // 5. THE PUSH (This MUST be the last part of the function)
    ST7565_BlitStatusLine();
    ST7565_BlitFullScreen();
}
