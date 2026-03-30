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
#include "ui/helper.h"
#include "ui/welcome.h"
#include "version.h"
#include "bitmaps.h"

void UI_DisplayReleaseKeys(void)
{
    UI_DisplayClear();
    UI_PrintString("RELEASE", 0, 127, 1, 10);
    ST7565_BlitFullScreen();
}

void UI_DisplayWelcome(void)
{
    char WelcomeString0[17];
    char WelcomeString1[17];
    char BatteryString[32];

    UI_DisplayClear();
    memset(gStatusLine, 0, sizeof(gStatusLine));

    // 1. DRAW LOGO (Using the FLAT logic from your successful test)
    memcpy(gStatusLine, g_qta_logo_short, 128);
    memcpy(gFrameBuffer, g_qta_logo_short + 128, 256);

    // 2. READ CHIRP DATA SAFELY
    memset(WelcomeString0, 0, sizeof(WelcomeString0));
    memset(WelcomeString1, 0, sizeof(WelcomeString1));
    EEPROM_ReadBuffer(0x0EB0, WelcomeString0, 16);
    EEPROM_ReadBuffer(0x0EC0, WelcomeString1, 16);

    // 3. PREPARE VERSION & BATT (Safe length)
    sprintf(BatteryString, "%s %u.%02uV %u%%", 
            Version,
            gBatteryVoltageAverage / 100, 
            gBatteryVoltageAverage % 100, 
            BATTERY_VoltsToPercent(gBatteryVoltageAverage));

    // 4. PRINT TEXT (Avoiding the 'wrapping' Line 7)
    
    // If CHIRP line 1 is valid, show it, otherwise show default
    if (WelcomeString0[0] != 0xFF && strlen(WelcomeString0) > 0) {
        UI_PrintStringSmallNormal(WelcomeString0, 0, 127, 4);
    } else {
        UI_PrintStringSmallNormal("QTA MOD", 0, 127, 4);
    }

    // If CHIRP line 2 is valid, show it
    if (WelcomeString1[0] != 0xFF && strlen(WelcomeString1) > 0) {
        UI_PrintStringSmallNormal(WelcomeString1, 0, 127, 5);
    }

    // Always show the Version/Battery at the bottom (Line 6)
    UI_PrintStringSmallNormal(BatteryString, 0, 127, 6);

    // 5. THE PUSH
    ST7565_BlitStatusLine();
    ST7565_BlitFullScreen();
}
