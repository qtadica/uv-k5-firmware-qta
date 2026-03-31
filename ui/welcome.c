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
#include "settings.h"

// Static buffer to prevent stack/memory crashes
static char s_Buf[32];

void UI_DisplayReleaseKeys(void)
{
    UI_DisplayClear();
    UI_PrintString("RELEASE", 0, 127, 1, 10);
    ST7565_BlitFullScreen();
}

void UI_DisplayWelcome(void)
{
    // 1. Check Menu Setting using the names the compiler wants
    if (gEeprom.POWER_ON_DISPLAY_MODE == POWER_ON_DISPLAY_MODE_NONE) {
        return;
    }

    UI_DisplayClear();
    memset(gStatusLine, 0, sizeof(gStatusLine));

    // 2. DRAW LOGO (Only if set to ALL)
    if (gEeprom.POWER_ON_DISPLAY_MODE == POWER_ON_DISPLAY_MODE_ALL) {
        memcpy(gStatusLine, g_qta_logo_short, 128);
        memcpy(gFrameBuffer, g_qta_logo_short + 128, 256);
    }

    // 3. DRAW MESSAGES (If set to ALL or MESSAGE)
    if (gEeprom.POWER_ON_DISPLAY_MODE == POWER_ON_DISPLAY_MODE_ALL || 
        gEeprom.POWER_ON_DISPLAY_MODE == POWER_ON_DISPLAY_MODE_MESSAGE) {
        
        // Line 4 (CHIRP 1)
        memset(s_Buf, 0, sizeof(s_Buf));
        EEPROM_ReadBuffer(0x0EB0, s_Buf, 16);
        for (int i = 0; i < 16; i++) { if (s_Buf[i] < 32 || s_Buf[i] > 126) { s_Buf[i] = '\0'; break; } }
        
        if (strlen(s_Buf) > 0) {
            UI_PrintStringSmallNormal(s_Buf, 0, 127, 4);
        } else {
            UI_PrintStringSmallNormal("QTA MOD", 0, 127, 4);
        }

        // Line 5 (CHIRP 2)
        memset(s_Buf, 0, sizeof(s_Buf));
        EEPROM_ReadBuffer(0x0EC0, s_Buf, 16);
        for (int i = 0; i < 16; i++) { if (s_Buf[i] < 32 || s_Buf[i] > 126) { s_Buf[i] = '\0'; break; } }
        
        if (strlen(s_Buf) > 0) {
            UI_PrintStringSmallNormal(s_Buf, 0, 127, 5);
        } else {
            UI_PrintStringSmallNormal("V1.0 ONLINE", 0, 127, 5);
        }
    }

    // 4. DRAW VOLTAGE (If set to ALL or VOLTAGE)
    if (gEeprom.POWER_ON_DISPLAY_MODE == POWER_ON_DISPLAY_MODE_ALL || 
        gEeprom.POWER_ON_DISPLAY_MODE == POWER_ON_DISPLAY_MODE_VOLTAGE) {
        
        memset(s_Buf, 0, sizeof(s_Buf));
        sprintf(s_Buf, "%s  %u.%02uV", 
                Version, 
                gBatteryVoltageAverage / 100, 
                gBatteryVoltageAverage % 100);
        UI_PrintStringSmallNormal(s_Buf, 0, 127, 6);
    }

    // 5. THE PUSH
    ST7565_BlitStatusLine();
    ST7565_BlitFullScreen();
}
