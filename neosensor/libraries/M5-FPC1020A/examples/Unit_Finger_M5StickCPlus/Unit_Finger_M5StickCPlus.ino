/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5StickCPlus sample source code
*                          配套  M5StickCPlus 示例源代码
* Visit for more information: https://docs.m5stack.com/en/unit/finger
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/unit/finger
*
* Product: Finger Unit example
* Date: 2022/7/18
*******************************************************************************
FINGER UNIT use case: Press the left button to enter the fingerprint entry mode.
Press the middle button to enter the fingerprint identification mode FINGER UNIT
使用案例按左键进入指纹录入模式，短按中间键进入指纹识别模式
*/

#include <M5StickCPlus.h>
#include "M5_FPC1020A.h"

FingerPrint Finger;

void setup() {
    M5.begin();             // Init M5StickCPlus.  初始化M5StickCPlus
    M5.Lcd.setRotation(3);  // Rotate the screen.  旋转屏幕
    M5.Lcd.setTextColor(GREEN);
    M5.Lcd.println("Finger Unit TEST");
    M5.Lcd.println("Btn.A add a user fingerprint");
    M5.Lcd.println("Btn.B delete all user");
    Finger.begin(&Serial2, 32, 33);
}

void loop() {
    M5.update();
    uint8_t res1;
    // ButtonA: Add user.  添加用户
    if (M5.BtnA.wasPressed()) {
        M5.Lcd.fillRect(0, 8, 160, 80, BLACK);
        M5.Lcd.setCursor(0, 20);
        M5.Lcd.println("Start Fingerprint Typing");
        M5.Lcd.println("Put Your Finger on the sensor");
        M5.Lcd.println("wating....");

        res1 = Finger.fpm_addUser(22, 1);  //(user_num, userPermission)
        if (res1 == ACK_SUCCESS)
            M5.Lcd.println("Success");
        else
            M5.Lcd.println("Fail");
    }
    // ButtonB: Matching.  匹配指纹
    if (M5.BtnB.wasPressed()) {
        M5.Lcd.fillRect(0, 8, 160, 80, BLACK);
        M5.Lcd.setCursor(0, 20);
        M5.Lcd.println("Start Verify Fingerprint");
        res1 = Finger.fpm_compareFinger();
        if (res1 == ACK_SUCCESS) {
            M5.Lcd.println("Success");
            M5.Lcd.print("User ID: ");
            M5.Lcd.println(Finger.fpm_getUserId());
        } else {
            M5.Lcd.println("No Such User");
        }
    }
}
