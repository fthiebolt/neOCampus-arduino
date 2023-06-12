/*
*******************************************************************************
* Copyright (c) 2022 by M5Stack
*                  Equipped with M5StickC sample source code
*                          配套  M5StickC 示例源代码
* Visit for more information: https://docs.m5stack.com/en/unit/dlight
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/unit/dlight
*
* Product: Unit DLight
* Date: 2022/7/27
*******************************************************************************
  Please connect the sensor to port, the Lux value will be displayed
  on the display after successful initialization 请将传感器连接至端口,
  成功初始化后显示屏上将显示Lux数值
*/

#include <M5StickC.h>
#include <M5GFX.h>
#include <M5_DLight.h>

M5GFX display;
M5Canvas canvas(&display);

M5_DLight sensor;
uint16_t lux;

void setup() {
    M5.begin();
    display.begin();
    canvas.setTextDatum(MC_DATUM);
    canvas.setColorDepth(1);
    canvas.setFont(&fonts::Orbitron_Light_24);
    canvas.setTextSize(1);
    canvas.setRotation(3);
    canvas.createSprite(display.width(), display.height());
    canvas.setPaletteColor(1, ORANGE);
    Serial.println("Sensor begin.....");
    sensor.begin();

    // CONTINUOUSLY_H_RESOLUTION_MODE
    // CONTINUOUSLY_H_RESOLUTION_MODE2
    // CONTINUOUSLY_L_RESOLUTION_MODE
    // ONE_TIME_H_RESOLUTION_MODE
    // ONE_TIME_H_RESOLUTION_MODE2
    // ONE_TIME_L_RESOLUTION_MODE
    sensor.setMode(CONTINUOUSLY_H_RESOLUTION_MODE);
}

char info[40];

void loop() {
    lux = sensor.getLUX();
    sprintf(info, "lux: %d", lux);
    canvas.fillSprite(BLACK);
    canvas.drawString(info, 80, 40);
    canvas.pushSprite(0, 0);
    Serial.println(info);
    delay(100);
}
