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
    // We keep the variables very simple to prevent memory overflow
    char WelcomeString[17];
    
    UI_DisplayClear();

    // 1. FLAT MEMORY LOGO DRAWING (The Safe Way)
    // Copy first 128 bytes to status line
    memcpy(gStatusLine, g_qta_logo_short, 128);
    // Copy the remaining 256 bytes to the start of the main buffer
    memcpy(gFrameBuffer, g_qta_logo_short + 128, 256);

    // 2. DEBUG TEXT
    // If you see "QTA-OK" on the screen, we know the new code is working!
    UI_PrintStringSmallNormal("QTA-OK", 0, 127, 4);

    // 3. BATTERY (Minimalist to prevent sprintf crashes)
    uint16_t volt = gBatteryVoltageAverage;
    sprintf(WelcomeString, "%u.%02uV", volt / 100, volt % 100);
    UI_PrintStringSmallNormal(WelcomeString, 0, 127, 6);

    // 4. PUSH TO HARDWARE
    ST7565_BlitStatusLine();
    ST7565_BlitFullScreen();
}
