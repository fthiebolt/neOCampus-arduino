/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core2 sample source code
*                          配套  M5Core2 示例源代码
* Visit for more information: https://docs.m5stack.com/en/module/4relay
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/module/4relay
*
* Describe: Module 4Relay.
* Date: 2022/07/22
*******************************************************************************
*/

#include <M5Core2.h>
#include "Module_4RELAY.h"
#include <M5GFX.h>

M5GFX display;
M5Canvas canvas(&display);

MODULE_4RELAY RELAY;

void drwaRect() {
    canvas.fillSprite(0);
    canvas.drawString("Module 4Relay", 160, 20);
    canvas.drawString("A REVERSE", 220, 80);
    canvas.drawString("B STEP", 220, 120);
    canvas.drawString("C RUNING", 220, 160);
    uint8_t state = RELAY.getAllRelayState();
    for (uint8_t i = 0; i < 4; i++) {
        if (((state >> i) & 0x01) == 1) {
            canvas.fillRect(20, 50 + 50 * i, 100, 20);
        } else {
            canvas.drawRect(20, 50 + 50 * i, 100, 20);
        }
    }
    canvas.pushSprite(0, 0);
}

void setup() {
    M5.begin(true, false, true, true);  // Init M5Core2.  初始化M5Core2
    // sensor.begin(&Wire1, 0x57, 21, 22, 200000L);
    while (!RELAY.begin(&Wire1, MODULE_4RELAY_ADDR, 21, 22, 200000L)) {
        Serial.println("4RELAY INIT ERROR");
        M5.Lcd.println("4RELAY INIT ERROR");
        delay(1000);
    };

    display.begin();
    canvas.setColorDepth(1);
    canvas.setFont(&fonts::Orbitron_Light_24);
    canvas.setTextSize(1);
    canvas.setTextDatum(MC_DATUM);
    canvas.createSprite(display.width(), display.height());
    canvas.setPaletteColor(1, ORANGE);
    canvas.pushSprite(0, 0);
}

uint8_t mode = 0;

bool delayBtn(unsigned long start) {
    while (1) {
        M5.update();
        if (M5.BtnA.wasPressed()) {
            mode = 0;
            return true;
        }
        if (M5.BtnB.wasPressed()) {
            mode = 1;
            return true;
        }
        if (M5.BtnC.wasPressed()) {
            mode = 2;
            return true;
        }
        if ((millis() - start) > 500) {
            return false;
        }
    }
}

void loop() {
    switch (mode) {
        case 0:
            RELAY.setAllRelay(true);
            drwaRect();
            if (delayBtn(millis())) {
                RELAY.setAllRelay(false);
                break;
            };
            RELAY.setAllRelay(false);
            drwaRect();
            if (delayBtn(millis())) {
                RELAY.setAllRelay(false);
                break;
            };
            break;
        case 1:
            for (uint8_t i = 0; i < 4; i++) {
                while (!delayBtn(millis()))
                    ;
                if (mode != 1) break;
                RELAY.setRelay(i, true);
                drwaRect();
            }
            for (uint8_t i = 0; i < 4; i++) {
                while (!delayBtn(millis()))
                    ;
                if (mode != 1) break;
                RELAY.setRelay(i, false);
                drwaRect();
            }
            break;
        case 2:
            for (uint8_t i = 0; i < 4; i++) {
                RELAY.setRelay(i, true);
                drwaRect();
                if (delayBtn(millis())) {
                    break;
                };
            }
            for (uint8_t i = 0; i < 4; i++) {
                RELAY.setRelay(i, false);
                drwaRect();
                if (delayBtn(millis())) {
                    break;
                };
            }
            break;
    }
}
