#include "idf/launcher_platform.h"
#include "powerSave.h"
#include <interface.h>

/***************************************************************************************
** Function:    _setup_gpio()
** Location:    main.cpp
** Description: initial setup for the device
***************************************************************************************/
void _setup_gpio() {
    // Configura os pinos do joystick e botões com pull-up interno
    pinMode(14, INPUT_PULLUP); // Botão Select / Clique
    pinMode(0, INPUT_PULLUP);  // Botão Boot (Esc)
    
    // Configura os pinos analógicos/digitais do Joystick
    pinMode(12, INPUT);
    pinMode(13, INPUT);
}

/***************************************************************************************
** Function:    _post_setup_gpio()
** Location:    main.cpp
** Description: second stage gpio setup, run after TFT and before SD card initialization
***************************************************************************************/
void _post_setup_gpio() {}

/***************************************************************************************
** Function: _late_setup_gpio()
** Location: main.cpp
** Description: third stage gpio, run befor bootscreen animation
***************************************************************************************/
void _late_setup_gpio() {}

/***************************************************************************************
** Function name: getBattery()
** location: display.cpp
** Description:   Delivers the battery value from 1-100
***************************************************************************************/
int getBattery() { return 100; }

/*********************************************************************
** Function: setBrightness
** location: settings.cpp
** set brightness value
**********************************************************************/
void _setBrightness(uint8_t brightval) {}

/*********************************************************************
** Function: InputHandler
** Handles the variables PrevPress, NextPress, SelPress, AnyKeyPress and EscPress
**********************************************************************/
void InputHandler(void) {
    checkPowerSaveTime();
    PrevPress = false;
    NextPress = false;
    SelPress = false;
    AnyKeyPress = false;
    EscPress = false;

    // Leitura dos botões físicos (Ativo em LOW por causa do PULLUP)
    bool selectPressed = (digitalRead(14) == LOW);
    bool escPressed = (digitalRead(0) == LOW);

    // Leitura dos eixos do Joystick (Conversão ADC do ESP32-S3: 0 a 4095)
    int joyX = analogRead(12);
    int joyY = analogRead(13);

    // Limites de limiar (Threshold) para detectar o movimento do joystick
    bool joyUp    = (joyY < 1000);
    bool joyDown  = (joyY > 3000);
    bool joyLeft  = (joyX < 1000);
    bool joyRight = (joyX > 3000);

    // Condição geral se qualquer input for acionado
    if (selectPressed || escPressed || joyUp || joyDown || joyLeft || joyRight) {
        if (!wakeUpScreen()) {
            AnyKeyPress = true;
        } else {
            goto END;
        }
    }

    // Mapeamento dos comandos do Launcher
    if (joyUp || joyLeft)     { PrevPress = true; }
    if (joyDown || joyRight)  { NextPress = true; }
    if (selectPressed)        { SelPress = true; }
    if (escPressed)           { EscPress = true; }

END:
    if (AnyKeyPress) {
        long tmp = launcherMillis();
        while ((launcherMillis() - tmp) < 200 && (digitalRead(14) == LOW || digitalRead(0) == LOW));
    }
}

/*********************************************************************
** Function: powerOff
** location: mykeyboard.cpp
** Turns off the device (or try to)
**********************************************************************/
void powerOff() {
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_0, LOW);
    vTaskDelay(pdMS_TO_TICKS(200));
    esp_deep_sleep_start();
}

/*********************************************************************
** Function: reboot
** Handles reboot process for devices
**********************************************************************/
void reboot() {
    ESP.restart();
}
