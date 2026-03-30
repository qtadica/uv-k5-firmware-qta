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
#include "ui/helper.h"
#include "ui/welcome.h"
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
    
    // 1. CLEAN THE TOP
    memset(gStatusLine, 0, sizeof(gStatusLine));

    // 2. DRAW LOGO (Lines 0, 1, 2)
    // We copy 128 bytes to each specific row to prevent "shifting"
    memcpy(gStatusLine, g_qta_logo_short, 128);
    memcpy(gFrameBuffer[0], g_qta_logo_short + 128, 128);
    memcpy(gFrameBuffer[1], g_qta_logo_short + 256, 128);

    // 3. THE TEXT (Adjusted Line Numbers)
    // We will use Line 4, 5, and 6. 
    // If Line 7 was at the top, we stay away from it!
    
    UI_PrintStringSmallNormal("QTA MOD", 0, 127, 4);
    UI_PrintStringSmallNormal("BOOTING...", 0, 127, 5);
    UI_PrintStringSmallNormal("V1.0 READY", 0, 127, 6);

    // 4. THE PUSH
    ST7565_BlitStatusLine();
    ST7565_BlitFullScreen();
}
