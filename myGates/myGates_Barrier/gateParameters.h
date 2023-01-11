// MY GATES PARAMETERS

#define NAVETTE_UUID "DEADDEAD-F88F-0042-F88F-010203040506"   // same UUID for all vehicles
#define FORCEGATEOPEN 0b0100                                  // minor high bits = 0b01 => force gate to open
#define CLEARGATECALIBRATION 0b1000                           // minor high bits = 0b10 => disable BLE scan and upload software
#define OTASWUPDATE 0b1100                                    // minor high bits = 0b11 => clear gate calibration

#define OTA_EXIT_AFTER 60*5             // after X sec if the software is not updating, getting out of the mode STATE_OTA
#define SCAN_TIME 1                   // scan period in second
#define GO_TO_SCAN_STATE_DELAY 4      // if no frame was received during X sec, go to STATE_SCAN
#define DELAY_REJECT_FRAME 3           // if the last frame was received more than X seconds ago, the average RSSI is not computed and returns an average RSSI of -100
#define PULSE_DURATION 500            // pulse to open gate. duration in ms
#define DELAY_BETWEEN_PULSE 5         // to keep the gate open emit a pulse every X seconds
#define RSSI_THRESHOLD_OPEN_GATE -95  // if the average RSSI is above this threshold the gate can be open
#define SERIAL_BAUDRATE 115200
