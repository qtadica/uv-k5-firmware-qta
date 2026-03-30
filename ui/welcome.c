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

// Define our own safe buffer here so we don't get "undeclared" errors
static char s_WelcomeBuffer[32];

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

    // 1. DRAW LOGO (Using the flat pointer logic that we know works)
    memcpy(gStatusLine, g_qta_logo_short, 128);
    memcpy(gFrameBuffer, g_qta_logo_short + 128, 256);

    // 2. CHIRP LINE 1
    memset(s_WelcomeBuffer, 0, sizeof(s_WelcomeBuffer));
    EEPROM_ReadBuffer(0x0EB0, s_WelcomeBuffer, 16);
    
    // Sanitize: Stop at the first weird character to prevent font-crash
    for (int i = 0; i < 16; i++) {
        if (s_WelcomeBuffer[i] < 32 || s_WelcomeBuffer[i] > 126) {
            s_WelcomeBuffer[i] = '\0';
            break;
        }
    }

    if (strlen(s_WelcomeBuffer) > 0) {
        UI_PrintStringSmallNormal(s_WelcomeBuffer, 0, 127, 4);
    } else {
        UI_PrintStringSmallNormal("QTA MOD", 0, 127, 4);
    }

    // 3. CHIRP LINE 2
    memset(s_WelcomeBuffer, 0, sizeof(s_WelcomeBuffer));
    EEPROM_ReadBuffer(0x0EC0, s_WelcomeBuffer, 16);
    
    for (int i = 0; i < 16; i++) {
        if (s_WelcomeBuffer[i] < 32 || s_WelcomeBuffer[i] > 126) {
            s_WelcomeBuffer[i] = '\0';
            break;
        }
    }

    if (strlen(s_WelcomeBuffer) > 0) {
        UI_PrintStringSmallNormal(s_WelcomeBuffer, 0, 127, 5);
    }

    // 4. VERSION & BATTERY 
    memset(s_WelcomeBuffer, 0, sizeof(s_WelcomeBuffer));
    sprintf(s_WelcomeBuffer, "%s  %u.%02uV", 
            Version, 
            gBatteryVoltageAverage / 100, 
            gBatteryVoltageAverage % 100);
    UI_PrintStringSmallNormal(s_WelcomeBuffer, 0, 127, 6);

    // 5. PUSH TO SCREEN
    ST7565_BlitStatusLine();
    ST7565_BlitFullScreen();
}
