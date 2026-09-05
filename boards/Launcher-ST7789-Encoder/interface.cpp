#include "core/powerSave.h"
#include <interface.h>

void _setup_gpio() {}

void _post_setup_gpio() {}

int getBattery() { return 0; }

void _setBrightness(uint8_t brightval) {}

void InputHandler(void) {
    checkPowerSaveTime();
    PrevPress = false;
    NextPress = false;
    SelPress = false;
    AnyKeyPress = false;
    EscPress = false;

    if (false /*Conditions for all inputs*/) {
        if (!wakeUpScreen()) AnyKeyPress = true;
        else goto END;
    }
    if (false /*Conditions for previous btn*/) { PrevPress = true; }
    if (false /*Conditions for Next btn*/) { NextPress = true; }
    if (false /*Conditions for Esc btn*/) { EscPress = true; }
    if (false /*Conditions for Select btn*/) { SelPress = true; }
END:
    if (AnyKeyPress) {
        long tmp = millis();
        while ((millis() - tmp) < 200 && false /*Conditions for all inputs*/);
    }
}

String keyboard(String mytext, int maxSize, String msg) {}

void powerOff() {}

void checkReboot() {}
