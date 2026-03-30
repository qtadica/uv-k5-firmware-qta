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
    memset(gStatusLine,  0, sizeof(gStatusLine));
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
    // 1. Prepare the screen memory
    memset(gStatusLine, 0, sizeof(gStatusLine));

    #if defined(ENABLE_FEAT_F4HWN_CTR) || defined(ENABLE_FEAT_F4HWN_INV)
        ST7565_ContrastAndInv();
    #endif

    UI_DisplayClear();

    // 2. Copy your PETER logo directly into the screen buffer
    memcpy(gFrameBuffer, g_boot_logo, 1024);

    // 3. Push the image to the physical LCD
    ST7565_BlitFullScreen();

    #ifdef ENABLE_FEAT_F4HWN_SCREENSHOT
        getScreenShot(true);
    #endif
}
