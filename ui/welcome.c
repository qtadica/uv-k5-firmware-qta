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
    UI_DisplayClear();
    memset(gStatusLine, 0, sizeof(gStatusLine));

    // 1. DRAW LOGO (Safe flat copy)
    memcpy(gStatusLine, g_qta_logo_short, 128);
    memcpy(gFrameBuffer, g_qta_logo_short + 128, 256);

    // 2. CHIRP LINE 1 (Using shared global memory so we don't crash the radio)
    memset(gString, 0, sizeof(gString));
    EEPROM_ReadBuffer(0x0EB0, gString, 16);
    if (gString[0] != 0xFF && gString[0] != 0x00) {
        gString[16] = '\0'; 
        UI_PrintStringSmallNormal(gString, 0, 127, 4);
    } else {
        UI_PrintStringSmallNormal("QTA MOD", 0, 127, 4);
    }

    // 3. CHIRP LINE 2 (Re-using the exact same memory)
    memset(gString, 0, sizeof(gString));
    EEPROM_ReadBuffer(0x0EC0, gString, 16);
    if (gString[0] != 0xFF && gString[0] != 0x00) {
        gString[16] = '\0';
        UI_PrintStringSmallNormal(gString, 0, 127, 5);
    }

    // 4. VERSION & BATTERY VOLTAGE
    // Re-using the same memory one last time. Removed % math to prevent division crashes.
    memset(gString, 0, sizeof(gString));
    sprintf(gString, "%s  %u.%02uV", Version, gBatteryVoltageAverage / 100, gBatteryVoltageAverage % 100);
    UI_PrintStringSmallNormal(gString, 0, 127, 6);

    // 5. PUSH TO SCREEN
    ST7565_BlitStatusLine();
    ST7565_BlitFullScreen();
}
