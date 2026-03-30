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
    char WelcomeString[17];
    
    UI_DisplayClear();

    // 1. SAFE LOGO DRAWING
    // We only copy the first 128 bytes to the status line for now to test
    memcpy(gStatusLine, g_qta_logo_short, 128);

    // 2. BASIC TEXT (No complex math/logic that could crash it)
    UI_PrintString("QTA MOD v1.0", 0, 127, 2, 10);
    
    // Read one line from CHIRP just to see if it works
    memset(WelcomeString, 0, sizeof(WelcomeString));
    EEPROM_ReadBuffer(0x0EB0, WelcomeString, 16);
    UI_PrintString(WelcomeString, 0, 127, 4, 10);

    ST7565_BlitStatusLine();
    ST7565_BlitFullScreen();
}
