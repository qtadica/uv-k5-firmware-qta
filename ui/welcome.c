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

#ifdef ENABLE_FEAT_F4HWN_SCREENSHOT
    #include "screenshot.h"
#endif

void UI_DisplayReleaseKeys(void)
{
    memset(gStatusLine, 0, sizeof(gStatusLine));
#if defined(ENABLE_FEAT_F4HWN_CTR) || defined(ENABLE_FEAT_F4HWN_INV)
        ST7565_ContrastAndInv();
#endif
    UI_DisplayClear();

    UI_PrintString("RELEASE", 0, 127, 1, 10);
    UI_PrintString("ALL KEYS", 0, 127, 3, 10);

    ST7565_BlitStatusLine();
    ST7565_BlitFullScreen();
}

void UI_DisplayWelcome(void)
{
    char WelcomeString0[17];
    char WelcomeString1[17];
    char WelcomeString2[17];
    char WelcomeString3[32];

    memset(gStatusLine, 0, sizeof(gStatusLine));

#if defined(ENABLE_FEAT_F4HWN_CTR) || defined(ENABLE_FEAT_F4HWN_INV)
        ST7565_ContrastAndInv();
#endif
    UI_DisplayClear();

    // 1. DRAW THE QTA LOGO (Always visible at the top)
    memcpy(gStatusLine, g_qta_logo_short, 128);
    memcpy(gFrameBuffer, g_qta_logo_short + 128, 256);

    // 2. TEXT LOGIC
    if (gEeprom.POWER_ON_DISPLAY_MODE != POWER_ON_DISPLAY_MODE_NONE && 
        gEeprom.POWER_ON_DISPLAY_MODE != POWER_ON_DISPLAY_MODE_SOUND) 
    {
        memset(WelcomeString0, 0, sizeof(WelcomeString0));
        memset(WelcomeString1, 0, sizeof(WelcomeString1));

        // Read custom lines from CHIRP
        EEPROM_ReadBuffer(0x0EB0, WelcomeString0, 16);
        EEPROM_ReadBuffer(0x0EC0, WelcomeString1, 16);

        // Prepare Battery Voltage string
        sprintf(WelcomeString2, "%u.%02uV %u%%",
                gBatteryVoltageAverage / 100,
                gBatteryVoltageAverage % 100,
                BATTERY_VoltsToPercent(gBatteryVoltageAverage));

        // Create the Bottom Line: "v1.0  8.40V 100%"
        sprintf(WelcomeString3, "%s  %s", Version, WelcomeString2);

        // If CHIRP is empty, show default QTA text
        if(strlen(WelcomeString0) == 0) strcpy(WelcomeString0, "WELCOME");
        if(strlen(WelcomeString1) == 0) strcpy(WelcomeString1, "QTA MOD");

        // 3. PRINT TO LCD (Positioned to avoid the logo)
        // Lines 4 & 5 are for the message, Line 7 is for Version/Battery
        UI_PrintStringSmallNormal(WelcomeString0, 0, 127, 4);
        UI_PrintStringSmallNormal(WelcomeString1, 0, 127, 5);
        UI_PrintStringSmallNormal(WelcomeString3, 0, 127, 7);
    }

    // 4. PUSH DATA TO LCD
    ST7565_BlitStatusLine();
    ST7565_BlitFullScreen();

#ifdef ENABLE_FEAT_F4HWN_SCREENSHOT
    getScreenShot(true);
#endif
}
