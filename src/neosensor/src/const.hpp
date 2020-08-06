#ifndef CONST_HPP
#define CONST_HPP

/* https consts */
#define AUTH_SERVER "https://sensocampus.univ-tlse3.fr"
#define CRED_REQ "/device/credentials?mac="
#define CONF_REQ "/device/config"

/* mqttsclient consts */
#define DEFT_LOGIN "test"
#define DEFT_PWD "test"
#define DEFT_SERVER "neocampus.univ-tlse3.fr"
#define DEFT_PORT 8883
#define DEFT_TOPIC_BASE "TestTopic"
#define DEFT_TOPIC "airquality"

#define LOGIN_SIZE 32
#define PWD_SIZE 64
#define SERVER_SIZE 32
#define TOPIC_BASE_SIZE 32
#define TOPIC_SIZE 20

#define MAX_SENSORS 30

/* memory consts */
#define CRED_FILE "/cred.txt"
#define MQTT_FILE "/conf.txt"
#define WIFI_FILE "/wconf.txt"

#define FILE_SIZE 10
#define MAX_JSON_SIZE 2048 //(JSON_OBJECT_SIZE(128)+1024)
#define CRED_JSON_SIZE 176 //(JSON_OBJECT_SIZE(4)+4*32+16)

#endif