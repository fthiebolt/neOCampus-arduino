#include "M5Stack.h"
#include "TFTTerminal.h"
#include "freertos/queue.h"
TFT_eSprite TerminalBuff = TFT_eSprite(&M5.Lcd);
TFTTerminal terminal(&TerminalBuff);

String waitRevice() {
    String recvStr;
    do {
        recvStr = Serial2.readStringUntil('\n');
    } while (recvStr.length() == 0);
    Serial.println(recvStr);
    terminal.println(recvStr);
    return recvStr;
}

void sendATCMD(String cmdStr) {
    Serial2.print(cmdStr);
    delay(100);
}

int sendATCMDAndRevice(String cmdStr) {
    Serial2.print(cmdStr);
    delay(100);
    waitRevice();
    String recvStr = waitRevice();
    if (recvStr.indexOf("OK") != -1) {
        return 0;
    } else {
        return -1;
    }
}

void setup() {
    M5.begin();
    Serial2.begin(115200, SERIAL_8N1, 16, 17);
    Serial2.flush();
    delay(100);
    M5.Lcd.fillRect(0, 0, 320, 240, TFT_BLACK);
    M5.Lcd.fillRect(0, 0, 320, 40, TFT_WHITE);
    M5.Lcd.setTextColor(TFT_BLACK);
    M5.Lcd.setTextDatum(TC_DATUM);
    M5.Lcd.drawString("COMX-LoraWan(915) TEST", 160, 10, 4);
    M5.Lcd.setTextDatum(TL_DATUM);
    M5.Lcd.setTextColor(TFT_WHITE);
    TerminalBuff.createSprite(240, 200);
    terminal.setGeometry(20, 55, 300, 200);
    terminal.setFontsize(1);

    sendATCMD("AT?\r\n");
    delay(100);
    Serial2.flush();
    sendATCMDAndRevice("AT+ILOGLVL=0\r\n");
    sendATCMDAndRevice("AT+CSAVE\r\n");
    sendATCMDAndRevice("AT+IREBOOT=0\r\n");
    terminal.println("LoraWan Rebooting");
    delay(2000);
    terminal.println("LoraWan config");
    // Set Join Mode OTAA
    sendATCMDAndRevice("AT+CJOINMODE=0\r\n");
    sendATCMDAndRevice("AT+CDEVEUI=d896e0ff00000240\r\n");
    sendATCMDAndRevice("AT+CAPPEUI=0000000000000001\r\n");
    sendATCMDAndRevice("AT+CAPPKEY=98929b92f09e2daf676d646d0f61d250\r\n");
    sendATCMDAndRevice("AT+CULDLMODE=2\r\n");
    // Set ClassC mode
    sendATCMDAndRevice("AT+CCLASS=2\r\n");
    sendATCMDAndRevice("AT+CWORKMODE=2\r\n");

    sendATCMDAndRevice("AT+CRXP=0,0,923300000\r\n");

    // TX Freq
    // 902.3
    // 902.5
    // 902.7
    // 902.9
    // 903.1
    // 903.3
    // 903.5
    // 903.7
    // MARK 0000 0000 0000 0001 | 0x001

    sendATCMDAndRevice("AT+CFREQBANDMASK=0001\r\n");

    // RX Freq
    // 923.3 - (RX1)
    // 923.9 - (RX1)
    // 924.5 - (RX1)
    // 925.1 - (RX1)
    // 925.7 - (RX1)
    // 926.3 - (RX1)
    // 926.9 - (RX1)
    // 927.5 - (RX1)
    // 923.3 - (RX2)   | 923300000

    sendATCMDAndRevice("AT+CJOIN=1,0,10,8\r\n");
}

enum systemstate {
    kIdel = 0,
    kJoined,
    kSending,
    kWaitSend,
    kEnd,
};
int system_fsm = kIdel;

int loraWanSendNUM = -1;
int loraWanSendCNT = -1;

void loop() {
    String recvStr = waitRevice();
    if (recvStr.indexOf("+CJOIN:") != -1) {
        if (recvStr.indexOf("OK") != -1) {
            Serial.println("[ INFO ] JOIN IN SUCCESSFUL");
            terminal.println("LoraWan JOIN");
            system_fsm = kJoined;
        } else {
            Serial.println("[ INFO ] JOIN IN FAIL");
            terminal.println("LoraWan JOIN FAIL");
            system_fsm = kIdel;
        }
    } else if (recvStr.indexOf("OK+RECV") != -1) {
        if (system_fsm == kJoined) {
            system_fsm = kSending;
        } else if (system_fsm == kWaitSend) {
            system_fsm = kSending;
            char strbuff[128];
            if ((loraWanSendCNT < 5) && (loraWanSendNUM == 8)) {
                sprintf(strbuff, "TSET OK CNT: %d", loraWanSendCNT);
                terminal.println(strbuff);
            } else {
                sprintf(strbuff, "FAILD NUM:%d CNT:%d", loraWanSendNUM,
                        loraWanSendCNT);
                terminal.println(strbuff);
            }
        }
    } else if (recvStr.indexOf("OK+SEND") != -1) {
        String snednum = recvStr.substring(8);
        // Serial.printf(" [ INFO ] SEND NUM %s \r\n",snednum.c_str());
        loraWanSendNUM = snednum.toInt();
    } else if (recvStr.indexOf("OK+SENT") != -1) {
        String snedcnt = recvStr.substring(8);
        // Serial.printf(" [ INFO ] SEND CNT %s \r\n",snedcnt.c_str());
        loraWanSendCNT = snedcnt.toInt();
    }

    if (system_fsm == kSending) {
        terminal.println("LoraWan Sending");
        sendATCMD("AT+DTRX=1,15,8,4655434b20535443\r\n");
        system_fsm = kWaitSend;
    }
    // if (M5.BtnA.wasPressed())
    // {
    //    sendATCMDAndRevice("AT+CLINKCHECK=1\r\n");
    //    delay(100);
    // }
    // if (M5.BtnB.wasPressed())
    // {
    //    sendATCMDAndRevice("AT+DTRX=1,15,8,4655434b20535443\r\n");
    //    delay(100);
    // }
    // if (M5.BtnC.wasPressed())
    // {
    //    sendATCMDAndRevice("AT+DRX?\r\n");
    //    delay(100);
    // }
    delay(10);
    M5.update();
}
