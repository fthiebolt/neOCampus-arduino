/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core2 sample source code
*                          配套  M5Core2 示例源代码
* Visit for more information: https://docs.m5stack.com/en/unit/4relay
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/unit/4relay
*
* Product: Unit 4RELAY.
* Date: 2022/7/22
*******************************************************************************
  Please connect to port A,Control 4 relays and demonstrate the asynchronous
control relay LED 请连接A端口，控制4继电器，并演示异步控制继电器LED
-------------------------------------------------------------------------------
  RELAY control reg           | 0x10
  -----------------------------------------------------------------------------
  Relay_ctrl_mode_reg[0]      | R/W | System control
                              | 7 | 6 | 5 | 4 | 3 | 2 | 1 |     0     |
                              | R | R | R | R | R | R | R | Sync Mode |
                              | -Sync Mode:0 LED&Relay Async
                              | -Sync Mode:1 LED&Relay Sync
  ---------------------------------------------------------------------------------
  Relay_ctrl_mode_reg[1]      | R/W | Relay & LED control
                              |  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
                              | LED1| LED2| LED3| LED4| RLY1| RLY2| RLY3| RLY4|
-------------------------------------------------------------------------------*/
// Note: The relay can only be controlled in synchronous mode, if the relay is
// controlled in asynchronous mode, it will be invalid.
// 注意:只能在同步模式下控制继电器,如果在异步模式下对继电器进行控制将无效,.

#include <M5Core2.h>
#include "Unit_4RELAY.h"

UNIT_4RELAY relay;

void setup() {
    M5.begin(true, false, true, true);  // Init M5Core2 And the I2C port(21,22).
                                        // 初始化 M5Core2 和I2C(21,22)端口
    M5.Lcd.setCursor(
        80, 0, 4);  // Set the cursor position to (80,0). 将光标位置设置为(80,0)
    M5.Lcd.print("4-RELAY UNIT\n\n");
    M5.Lcd.print("   Relay State: \n   Sync Mode: ");
    M5.Lcd.setCursor(0, 220, 2);
    M5.Lcd.print("Independent switch  Sync/Async    ALL relay");
    relay.Init(0);  // Set the lamp and relay to asynchronous mode(Async =
                    // 0,Sync = 1).  将灯和继电器设为非同步模式
}

char count_i   = 0;
bool sync_flag = 0, all_flag = 0;

void loop() {
    M5.update();  // Check button down state.  检测按键按下状态
    if (M5.BtnA.wasPressed()) {  // If button A is pressed.  如果按键A按下
        M5.Lcd.fillRect(160, 50, 100, 20, BLACK);
        M5.Lcd.setCursor(160, 50, 4);
        if (count_i < 4) {
            M5.Lcd.printf("%d ON", count_i + 1);
            if (sync_flag) {
                relay.relayWrite(
                    count_i,
                    1);  // Open the relay at Count_i.  打开count_i处的继电器
            } else
                relay.ledWrite(count_i,
                               1);  // Turn on count_I to get led lights.
                                    // 打开count_i出得led灯
        } else {
            M5.Lcd.printf("%d OFF", (count_i - 3));
            if (sync_flag) {
                relay.relayWrite(
                    (count_i - 4),
                    0);  // Close the relay at Count_i.  关闭count_i处的继电器
            } else
                relay.ledWrite(
                    (count_i - 4),
                    0);  // Turn off the COUNt_I leds.  关闭count_i出得led灯
        }
        count_i++;
        if (count_i >= 8) count_i = 0;
    } else if (M5.BtnB.wasPressed()) {
        sync_flag = !sync_flag;
        relay.switchMode(sync_flag);
        M5.Lcd.fillRect(160, 80, 100, 20, BLACK);
        M5.Lcd.setCursor(160, 80, 4);
        if (!sync_flag) {
            M5.Lcd.print("Async");
        } else {
            M5.Lcd.print("Sync");
        }
    }
    if (M5.BtnC.wasPressed()) {
        all_flag = !all_flag;
        M5.Lcd.fillRect(160, 50, 100, 20, BLACK);
        M5.Lcd.setCursor(160, 50, 4);
        if (all_flag) {
            M5.Lcd.printf("ALL.ON ");
            if (sync_flag)
                relay.relayAll(1);  // Open all the relays.  打开所有的继电器
            else
                relay.ledAll(1);  // Turn on all the lights. 打开所有的灯
        } else {
            M5.Lcd.printf("ALL.OFF");
            if (sync_flag)
                relay.relayAll(0);  // Close all relays.  关闭所有的继电器
            else
                relay.ledAll(0);  // Turn off all the lights.  关闭所有的灯
        }
    }
}