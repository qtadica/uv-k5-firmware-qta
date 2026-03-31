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

static char s_Buf[32];

void UI_DisplayReleaseKeys(void)
{
    UI_DisplayClear();
    UI_PrintString("RELEASE", 0, 127, 1, 10);
    ST7565_BlitFullScreen();
}

void UI_DisplayWelcome(void)
{
    // 1. Check Menu Setting - If set to NONE (0), exit immediately
    if (gEeprom.PWR_ON_DISPLAY_MODE == PWR_ON_DISPLAY_MODE_OFF) {
        return;
    }

    UI_DisplayClear();
    memset(gStatusLine, 0, sizeof(gStatusLine));

    // 2. Draw Logo ONLY if set to ALL (1)
    if (gEeprom.PWR_ON_DISPLAY_MODE == PWR_ON_DISPLAY_MODE_FULL) {
        memcpy(gStatusLine, g_qta_logo_short, 128);
        memcpy(gFrameBuffer, g_qta_logo_short + 128, 256);
    }

    // 3. Process Text ONLY if set to ALL (1) or MESSAGE (2)
    if (gEeprom.PWR_ON_DISPLAY_MODE == PWR_ON_DISPLAY_MODE_FULL || gEeprom.PWR_ON_DISPLAY_MODE == PWR_ON_DISPLAY_MODE_MESSAGE) {
        // Line 4 (CHIRP 1)
        memset(s_Buf, 0, sizeof(s_Buf));
        EEPROM_ReadBuffer(0x0EB0, s_Buf, 16);
        for (int i = 0; i < 16; i++) { if (s_Buf[i] < 32 || s_Buf[i] > 126) { s_Buf[i] = '\0'; break; } }
        
        if (strlen(s_Buf) > 0) UI_PrintStringSmallNormal(s_Buf, 0, 127, 4);
        else UI_PrintStringSmallNormal("QTA MOD", 0, 127, 4);

        // Line 5 (CHIRP 2)
        memset(s_Buf, 0, sizeof(s_Buf));
        EEPROM_ReadBuffer(0x0EC0, s_Buf, 16);
        for (int i = 0; i < 16; i++) { if (s_Buf[i] < 32 || s_Buf[i] > 126) { s_Buf[i] = '\0'; break; } }
        
        if (strlen(s_Buf) > 0) UI_PrintStringSmallNormal(s_Buf, 0, 127, 5);
        else UI_PrintStringSmallNormal("Welcome", 0, 127, 5);
    }

    // 4. Draw Battery ONLY if set to ALL (1) or VOLTAGE (3)
    if (gEeprom.PWR_ON_DISPLAY_MODE == PWR_ON_DISPLAY_MODE_FULL || gEeprom.PWR_ON_DISPLAY_MODE == PWR_ON_DISPLAY_MODE_VOLTAGE) {
        memset(s_Buf, 0, sizeof(s_Buf));
        sprintf(s_Buf, "%s  %u.%02uV", Version, gBatteryVoltageAverage / 100, gBatteryVoltageAverage % 100);
        UI_PrintStringSmallNormal(s_Buf, 0, 127, 6);
    }

    // 5. Final Push
    ST7565_BlitStatusLine();
    ST7565_BlitFullScreen();
}
