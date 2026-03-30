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
    // We make these buffers 32 to prevent any "spilling" (overflow)
    char WelcomeString0[32];
    char WelcomeString1[32];
    char BatteryString[32];

    UI_DisplayClear();
    memset(gStatusLine, 0, sizeof(gStatusLine));

    // 1. DRAW LOGO (Lines 0, 1, 2)
    memcpy(gStatusLine, g_qta_logo_short, 128);
    memcpy(gFrameBuffer[0], g_qta_logo_short + 128, 128);
    memcpy(gFrameBuffer[1], g_qta_logo_short + 256, 128);

    // 2. GET CHIRP DATA
    memset(WelcomeString0, 0, sizeof(WelcomeString0));
    memset(WelcomeString1, 0, sizeof(WelcomeString1));
    EEPROM_ReadBuffer(0x0EB0, WelcomeString0, 16);
    EEPROM_ReadBuffer(0x0EC0, WelcomeString1, 16);

    // 3. PREPARE VERSION & BATT
    // We use a safe sprintf that won't overflow our 32-byte buffer
    sprintf(BatteryString, "%s %u.%02uV %u%%", 
            Version,
            gBatteryVoltageAverage / 100, 
            gBatteryVoltageAverage % 100, 
            BATTERY_VoltsToPercent(gBatteryVoltageAverage));

    // 4. PRINT TEXT IN ORDER
    // Line 4: CHIRP 1
    UI_PrintStringSmallNormal(WelcomeString0, 0, 127, 4);
    // Line 5: CHIRP 2
    UI_PrintStringSmallNormal(WelcomeString1, 0, 127, 5);
    // Line 6: Version & Battery
    UI_PrintStringSmallNormal(BatteryString, 0, 127, 6);

    // 5. THE PUSH
    ST7565_BlitStatusLine();
    ST7565_BlitFullScreen();
    
    // Tiny delay so the user can actually see the screen before it goes to the radio
    // for (uint32_t i = 0; i < 1000000; i++) { __asm("nop"); }
}
