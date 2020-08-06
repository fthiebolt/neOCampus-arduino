/*
 * This is Aymen Sendhi sample code to access its sensors data
 * The target board is made of 4 independant sensors.
 * aug.20
 */

/****************************
 * Déclaration des paramètre
 * **************************
 */
//****** Déclaration des bibliothèques utilisées**********//

#include <ArduinoJson.h>
#include <PubSubClient.h> // Bibliothèque MQTT
#include <WiFi.h>         // Bibliothèque WIFI


#define VCC 3.3 // Tension en Volt
#define DELAY_MS 1000
#define VG 3 //tension de changement de gamme en volt

/********************************************************
 * Déclaration Pin Résistance des pieds pour Rs capteur 1
 * ******************************************************
 */
#define Rp1 18   // 10Mohms
#define Rp2 5  //  1 Mohms 
#define Rp3 17  // 100 Kohms
#define Rp4 16  // 10 Kohms

/********************************************************
 * Déclaration Pin Résistance des pieds pour Rs_1 capteur 2
 * ******************************************************
 */
#define Rp5 4 // 10Mohms
#define Rp6 0  //  1 Mohms
#define Rp7 2 // 100 Kohms
#define Rp8 15 // 10 Kohms

 /*************************
 * Déclaration Pin Vs_raw: 
 * ************************
 */

#define heater_read_1 35 //Tension lu au Pin correspond à la tension du sense capteur 2
#define heater_read_3 32  //Tension lu au Pin correspond à la tension du sense capteur 4

//****** Déclaration des variables pour le programme **********//

//****** Connexion wifi avec le broker **********//

IPAddress broker(195,220,53,54); // IP address of your MQTT broker eg. 192.168.1.50
WiFiClient wclient;  
PubSubClient client(wclient); // Setup MQTT client
bool state=0;

const char* ssid     = "amilab"; //NOM du wifi client 
const char* password = "neocampus"; //Mot de passe du wifi client 
const char* mqttServer = "neocampus.univ-tlse3.fr";
const char *ID_2 = "carte_2";  // Name of our device, must be unique
const char *TOPIC = "TestTopic/aymen_gazsensor/AQS_2";  // Topic to subcribe to

int inchar;

//******* Connect to WiFi network **************//

void setup_wifi() 
{
  Serial.print("\nConnecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password); // Connect to network
  
  while (WiFi.status() != WL_CONNECTED)  // Wait for connection
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

////******* Reconnect to client **************//

void reconnect()
{
  while (!client.connected())  // Loop until we're reconnected
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(ID_2)) 
    {
      Serial.println("connected");
      Serial.print("Publishing to: ");
      
      Serial.println(TOPIC);
      client.subscribe(TOPIC);

      Serial.println('\n');
   
    } else 
    {
      Serial.println(" try again in 5 seconds");
      //Serial.print(client.state());
      // Wait 5 seconds before retrying
      delay(500);
    }
  }
}

void callback(char* topic, byte *payload, unsigned int length) {

  Serial.println("-------new message from broker-----");

  Serial.print("channel:");

  Serial.println(topic);

  Serial.print("data:");

  Serial.write(payload, length);

  Serial.println();

}

//****************************************************//

void setup() {
  // put your setup code here, to run once:
   delay(5000);
   Serial.begin(115200);
    
   setup_wifi(); // Connect to network
   client.setServer(broker, 1883);
   client.setCallback(callback);

//*******************************************//
   
pinMode(Rp1,INPUT); // Pin Résistance de pied 10Mohms pour sense capteur 1
pinMode(Rp2,INPUT); // Pin Résistance de pied 1Mohms pour sense capteur 1
pinMode(Rp3,INPUT); // Pin Résistance de pied 100kohms pour sense capteur 1
pinMode(Rp4,INPUT); // Pin Résistance de pied 10kohms pour sense capteur 1

pinMode(Rp5,INPUT); // Pin Résistance de pied 10Mohms pour sense capteur 2
pinMode(Rp6,INPUT); // Pin Résistance de pied 1Mohms pour sense capteur 2
pinMode(Rp7,INPUT); // Pin Résistance de pied 100kohms pour sense capteur 2
pinMode(Rp8,INPUT); // Pin Résistance de pied 10kohms pour sense capteur 2

}

//**********************************************//

void publishSerialData(char *serialData) {

  if (!client.connected()) {
    Serial.println(" Reconnection"); 
    reconnect();
  }

  client.publish(TOPIC, serialData);
  client.subscribe(TOPIC);
}

//**************************************************//

void loop() {

client.loop();

  if (Serial.available())
  { 
    inchar=Serial.parseInt();
  }
 /*********************************************************
  *          Programme mesure du Rs pour capteur 1
  * *******************************************************
  */
  float Rl=10E+6; //10Mohms
  pinMode(Rp1,OUTPUT);
  digitalWrite(Rp1, LOW);
  pinMode(Rp2,INPUT);
  pinMode(Rp3,INPUT);
  pinMode(Rp4,INPUT);
  delay(DELAY_MS);
 int Vs_raw  = analogRead(heater_read_1);
 
  float Vs = Vs_raw * (VCC / 1023.0);
    
  if(Vs >VG)
  {
    Rl=1E+6; //1Mohms
    pinMode(Rp1,INPUT);
    pinMode(Rp2,OUTPUT);
    digitalWrite(Rp2, LOW);
    pinMode(Rp3,INPUT);
    pinMode(Rp4,INPUT);
    delay(DELAY_MS);
    Vs_raw = analogRead(heater_read_1);
    Vs = Vs_raw * (VCC / 1023.0);
    if(Vs>VG)
    {
      Rl=100E+3; //100kohms
      pinMode(Rp1,INPUT);
      pinMode(Rp2,INPUT);
      pinMode(Rp3,OUTPUT);
      digitalWrite(Rp3, LOW);
      pinMode(Rp4,INPUT);
      delay(DELAY_MS);
      Vs_raw  = analogRead(heater_read_1);
      Vs = Vs_raw * (VCC / 1023.0);
      if(Vs>VG)
      {
        Rl=10E+3; //10K
        pinMode(Rp1,INPUT);
        pinMode(Rp2,INPUT);
        pinMode(Rp3,INPUT);
        pinMode(Rp4,OUTPUT);
        digitalWrite(Rp4, LOW);
        delay(DELAY_MS);
        Vs_raw = analogRead(heater_read_1);
        Vs = Vs_raw * (VCC / 1023.0);
       }
    }
  }


 /*********************************************************
  *          Programme mesure du Rs pour capteur 2
  * *******************************************************
  */
  float R2=10E+6; //10Mohms
  pinMode(Rp5,OUTPUT);
  digitalWrite(Rp5, LOW);
  pinMode(Rp6,INPUT);
  pinMode(Rp7,INPUT);
  pinMode(Rp8,INPUT);
  delay(DELAY_MS);
 int Vs_raw_1  = analogRead(heater_read_3);
 
  float Vs_1 = Vs_raw_1 * (VCC / 1023.0);
    
  if(Vs_1 >VG)
  {
    R2=1E+6; //1Mohms
    pinMode(Rp5,INPUT);
    pinMode(Rp6,OUTPUT);
    digitalWrite(Rp6, LOW);
    pinMode(Rp7,INPUT);
    pinMode(Rp8,INPUT);
    delay(DELAY_MS);
    Vs_raw_1 = analogRead(heater_read_3);
    Vs_1 = Vs_raw_1 * (VCC / 1023.0);
    if(Vs_1>VG)
    {
      R2=100E+3; //100kohms
      pinMode(Rp5,INPUT);
      pinMode(Rp6,INPUT);
      pinMode(Rp7,OUTPUT);
      digitalWrite(Rp7, LOW);
      pinMode(Rp8,INPUT);
      delay(DELAY_MS);
      Vs_raw_1  = analogRead(heater_read_3);
      Vs_1 = Vs_raw_1 * (VCC / 1023.0);
      if(Vs_1>VG)
      {
        R2=10E+3; //10K
        pinMode(Rp5,INPUT);
        pinMode(Rp6,INPUT);
        pinMode(Rp7,INPUT);
        pinMode(Rp8,OUTPUT);
        digitalWrite(Rp8, LOW);
        delay(DELAY_MS);
        Vs_raw_1 = analogRead(heater_read_3);
        Vs_1 = Vs_raw_1 * (VCC / 1023.0);
       }
    }
  }

float  Rs= Rl*VCC/Vs-Rl;
float  Rs_1= R2*VCC/Vs_1-R2;


//***********************//

const int capacity = JSON_OBJECT_SIZE(6);

  StaticJsonDocument<capacity> doc;

  doc["SensorType"] = "GazSensors";

  doc["Cart"] = 2;

  doc["Sensor_1"] = "Bi_SnO2/WO3-CuO";

  doc["Value_Rs1"] = Rs;

  doc["Sensor_2"] = "Bi_SnO2/WO3-CuO";

  doc["Value_Rs2"] = Rs_1;

  
  String rep;
  serializeJson(doc,rep);
  Serial.println(rep);

  if (!client.connected()) {
     
    client.publish(TOPIC, rep.c_str());
    client.subscribe(TOPIC);
  }
  else{
    reconnect();
    }

}
