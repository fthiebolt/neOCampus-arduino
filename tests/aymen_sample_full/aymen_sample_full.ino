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

/*********************************************************************************************
 * Déclaration Pin Résistance des pieds pour CPT1(indication sur PCB) capteur en mode continu
 * *******************************************************************************************
 */
#define Rp1 18   // 10Mohms
#define Rp2 5  //  1 Mohms 
#define Rp3 17  // 100 Kohms
#define Rp4 16  // 10 Kohms


/******************************************************************************************
 * Déclaration Pin Résistance des pieds pour CPT2(indication sur PCB) capteur en mode continu
 * ****************************************************************************************
 */
#define Rp9 23 // 10Mohms
#define Rp10 22  //  1 Mohms
#define Rp11 21 // 100 Kohms
#define Rp12 19 // 10 Kohms

/******************************************************************************************
 * Déclaration Pin Résistance des pieds pour CPT3 (indication sur PCB) capteur en mode pulsé
 * ****************************************************************************************
 */
#define Rp13 13 // 10Mohms
#define Rp14 12  //  1 Mohms
#define Rp15 14 // 100 Kohms
#define Rp16 27 // 10 Kohms

/******************************************************************************************
 * Déclaration Pin Résistance des pieds pour CPT4(indication sur PCB) capteur en mode pulsé
 * ****************************************************************************************
 */
#define Rp5 4 // 10Mohms
#define Rp6 0  //  1 Mohms
#define Rp7 2 // 100 Kohms
#define Rp8 15 // 10 Kohms

 /*************************
 * Déclaration Pin Vs_raw: 
 * ************************
 */

#define heater_read_1 35  //Tension lu au Pin correspond à la tension du sense CPT1
#define heater_read_2 34 //Tension lu au Pin correspond à la tension du sense CPT2
#define heater_read_3 33 //Tension lu au Pin correspond à la tension du sense CPT3 (indication sur PCB) capteur en mode pulsé
#define heater_read_4 32  //Tension lu au Pin correspond à la tension du sense CPT4 (indication sur PCB) capteur en mode pulsé


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


pinMode(25, OUTPUT); // Pin lier au MOSFET capteur CPT3 pour faire une tension pulsé 
pinMode(26, OUTPUT); // Pin lier au MOSFET capteur CPT4 pour faire une tension pulsé 
   
pinMode(Rp1,INPUT); // Pin Résistance de pied 10Mohms pour sense capteur CPT1
pinMode(Rp2,INPUT); // Pin Résistance de pied 1Mohms pour sense capteur CPT1
pinMode(Rp3,INPUT); // Pin Résistance de pied 100kohms pour sense capteur CPT1
pinMode(Rp4,INPUT); // Pin Résistance de pied 10kohms pour sense capteur CPT1

pinMode(Rp9,INPUT); // Pin Résistance de pied 10Mohms pour sense capteur CPT2
pinMode(Rp10,INPUT); // Pin Résistance de pied 1Mohms pour sense capteur CPT2
pinMode(Rp11,INPUT); // Pin Résistance de pied 100kohms pour sense capteur CPT2
pinMode(Rp12,INPUT); // Pin Résistance de pied 10kohms pour sense capteur CPT2

pinMode(Rp13,INPUT); // Pin Résistance de pied 10Mohms pour sense capteur CPT3
pinMode(Rp14,INPUT); // Pin Résistance de pied 1Mohms pour sense capteur CPT3
pinMode(Rp15,INPUT); // Pin Résistance de pied 100kohms pour sense capteur CPT3
pinMode(Rp16,INPUT); // Pin Résistance de pied 10kohms pour sense capteur CPT3

pinMode(Rp5,INPUT); // Pin Résistance de pied 10Mohms pour sense capteur CPT4
pinMode(Rp6,INPUT); // Pin Résistance de pied 1Mohms pour sense capteur CPT4
pinMode(Rp7,INPUT); // Pin Résistance de pied 100kohms pour sense capteur CPT4
pinMode(Rp8,INPUT); // Pin Résistance de pied 10kohms pour sense capteur CPT4

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


digitalWrite(25, HIGH); // sets the digital pin MOSFET 25 on (CPT3)
digitalWrite(26, HIGH); // sets the digital pin MOSFET 26 on (CPT4)
delay(60000);            

  if (Serial.available())
  { 
    inchar=Serial.parseInt();
  }
 /*********************************************************
  *          Programme mesure du Rs pour capteur CPT1
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
 
  float Vs_1 = Vs_raw * (VCC / 4095.0);
    
  if(Vs_1 >VG)
  {
    Rl=1E+6; //1Mohms
    pinMode(Rp1,INPUT);
    pinMode(Rp2,OUTPUT);
    digitalWrite(Rp2, LOW);
    pinMode(Rp3,INPUT);
    pinMode(Rp4,INPUT);
    delay(DELAY_MS);
    Vs_raw = analogRead(heater_read_1);
    Vs_1 = Vs_raw * (VCC / 4095.0);
    if(Vs_1>VG)
    {
      Rl=100E+3; //100kohms
      pinMode(Rp1,INPUT);
      pinMode(Rp2,INPUT);
      pinMode(Rp3,OUTPUT);
      digitalWrite(Rp3, LOW);
      pinMode(Rp4,INPUT);
      delay(DELAY_MS);
      Vs_raw  = analogRead(heater_read_1);
      Vs_1 = Vs_raw * (VCC / 4095.0);
      if(Vs_1>VG)
      {
        Rl=10E+3; //10K
        pinMode(Rp1,INPUT);
        pinMode(Rp2,INPUT);
        pinMode(Rp3,INPUT);
        pinMode(Rp4,OUTPUT);
        digitalWrite(Rp4, LOW);
        delay(DELAY_MS);
        Vs_raw = analogRead(heater_read_1);
        Vs_1 = Vs_raw * (VCC / 4095.0);
       }
    }
  }


/*********************************************************
  *          Programme mesure du Rs pour capteur CPT2
  * *******************************************************
  */
  float R2=10E+6; //10Mohms
  pinMode(Rp9,OUTPUT);
  digitalWrite(Rp9, LOW);
  pinMode(Rp10,INPUT);
  pinMode(Rp11,INPUT);
  pinMode(Rp12,INPUT);
  delay(DELAY_MS);
 int Vs_raw_2  = analogRead(heater_read_2);
 
  float Vs_2 = Vs_raw_2 * (VCC / 4095.0);
    
  if(Vs_2 >VG)
  {
    R2=1E+6; //1Mohms
    pinMode(Rp9,INPUT);
    pinMode(Rp10,OUTPUT);
    digitalWrite(Rp10, LOW);
    pinMode(Rp11,INPUT);
    pinMode(Rp12,INPUT);
    delay(DELAY_MS);
    Vs_raw_2 = analogRead(heater_read_2);
    Vs_2 = Vs_raw_2 * (VCC / 4095.0);
    if(Vs_2>VG)
    {
      R2=100E+3; //100kohms
      pinMode(Rp9,INPUT);
      pinMode(Rp10,INPUT);
      pinMode(Rp11,OUTPUT);
      digitalWrite(Rp11, LOW);
      pinMode(Rp12,INPUT);
      delay(DELAY_MS);
      Vs_raw_2  = analogRead(heater_read_2);
      Vs_2 = Vs_raw_2 * (VCC / 4095.0);
      if(Vs_2>VG)
      {
        R2=10E+3; //10K
        pinMode(Rp9,INPUT);
        pinMode(Rp10,INPUT);
        pinMode(Rp11,INPUT);
        pinMode(Rp12,OUTPUT);
        digitalWrite(Rp12, LOW);
        delay(DELAY_MS);
        Vs_raw_2 = analogRead(heater_read_2);
        Vs_2 = Vs_raw_2 * (VCC / 4095.0);
       }
    }
  }

  /*********************************************************
  *          Programme mesure du Rs pour capteur CPT3
  * *******************************************************
  */
  float R3=10E+6; //10Mohms
  pinMode(Rp13,OUTPUT);
  digitalWrite(Rp13, LOW);
  pinMode(Rp14,INPUT);
  pinMode(Rp15,INPUT);
  pinMode(Rp16,INPUT);
  delay(DELAY_MS);
 int Vs_raw_3  = analogRead(heater_read_3);
 
  float Vs_3 = Vs_raw_3 * (VCC / 4095.0);
    
  if(Vs_3 >VG)
  {
    R3=1E+6; //1Mohms
    pinMode(Rp13,INPUT);
    pinMode(Rp14,OUTPUT);
    digitalWrite(Rp14, LOW);
    pinMode(Rp15,INPUT);
    pinMode(Rp16,INPUT);
    delay(DELAY_MS);
    Vs_raw_3 = analogRead(heater_read_3);
    Vs_3 = Vs_raw_3 * (VCC / 4095.0);
    if(Vs_3>VG)
    {
      R3=100E+3; //100kohms
      pinMode(Rp13,INPUT);
      pinMode(Rp14,INPUT);
      pinMode(Rp15,OUTPUT);
      digitalWrite(Rp15, LOW);
      pinMode(Rp12,INPUT);
      delay(DELAY_MS);
      Vs_raw_3  = analogRead(heater_read_3);
      Vs_3 = Vs_raw_3 * (VCC / 4095.0);
      if(Vs_3>VG)
      {
        R3=10E+3; //10K
        pinMode(Rp13,INPUT);
        pinMode(Rp14,INPUT);
        pinMode(Rp15,INPUT);
        pinMode(Rp16,OUTPUT);
        digitalWrite(Rp16, LOW);
        delay(DELAY_MS);
        Vs_raw_3 = analogRead(heater_read_3);
        Vs_3 = Vs_raw_3 * (VCC / 4095.0);
       }
    }
  }

   /*********************************************************
  *          Programme mesure du Rs pour capteur CPT4
  * *******************************************************
  */
  float R4=10E+6; //10Mohms
  pinMode(Rp5,OUTPUT);
  digitalWrite(Rp5, LOW);
  pinMode(Rp6,INPUT);
  pinMode(Rp7,INPUT);
  pinMode(Rp8,INPUT);
  delay(DELAY_MS);
 int Vs_raw_4  = analogRead(heater_read_4);
 
  float Vs_4 = Vs_raw_4 * (VCC / 4095.0);
    
  if(Vs_4 >VG)
  {
    R4=1E+6; //1Mohms
    pinMode(Rp5,INPUT);
    pinMode(Rp6,OUTPUT);
    digitalWrite(Rp6, LOW);
    pinMode(Rp7,INPUT);
    pinMode(Rp8,INPUT);
    delay(DELAY_MS);
    Vs_raw_4 = analogRead(heater_read_4);
    Vs_4 = Vs_raw_4 * (VCC / 4095.0);
    if(Vs_4>VG)
    {
      R4=100E+3; //100kohms
      pinMode(Rp5,INPUT);
      pinMode(Rp6,INPUT);
      pinMode(Rp7,OUTPUT);
      digitalWrite(Rp7, LOW);
      pinMode(Rp8,INPUT);
      delay(DELAY_MS);
      Vs_raw_4  = analogRead(heater_read_4);
      Vs_4 = Vs_raw_4 * (VCC / 4095.0);
      if(Vs_4>VG)
      {
        R4=10E+3; //10K
        pinMode(Rp5,INPUT);
        pinMode(Rp6,INPUT);
        pinMode(Rp7,INPUT);
        pinMode(Rp8,OUTPUT);
        digitalWrite(Rp8, LOW);
        delay(DELAY_MS);
        Vs_raw_4 = analogRead(heater_read_4);
        Vs_4 = Vs_raw_4 * (VCC / 4095.0);
       }
    }
  }
  
float  Rs_1= Rl*VCC/Vs_1-Rl; //CAPTEUR CPT1 EN MODE CONTINU
float  Rs_2= R2*VCC/Vs_2-R2; //CAPTEUR CPT2 EN MODE CONTINU
float  Rs_3= R3*VCC/Vs_3-R3; //CAPTEUR CPT3 EN MODE PULSEE
float  Rs_4= R4*VCC/Vs_4-R4; //CAPTEUR CPT4 EN MODE PULSEE


//***********************//

const int capacity = JSON_OBJECT_SIZE(6);

  StaticJsonDocument<capacity> doc;

  doc["SensorType"] = "GazSensors";

  doc["Cart"] = 2;

  doc["Sensor_1"] = "Bi_SnO2/WO3-CuO"; //CAPTEUR CPT1 EN MODE CONTINU

  doc["Value_Rs1"] = Rs_1;

  doc["Sensor_2"] = "Bi_SnO2/WO3-CuO"; //CAPTEUR CPT2 EN MODE CONTINU

  doc["Value_Rs2"] = Rs_2;

  doc["Sensor_3"] = "Bi_SnO2/WO3-CuO"; //CAPTEUR CPT3 EN MODE PULSEE

  doc["Value_Rs3"] = Rs_3;

  doc["Sensor_4"] = "Bi_SnO2/WO3-CuO"; //CAPTEUR CPT4 EN MODE PULSEE

  doc["Value_Rs4"] = Rs_4;
  
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

//--************************************************************//

digitalWrite(25, LOW); // sets the digital pin MOSFET 25 oFF (CPT3)
digitalWrite(26, LOW); // sets the digital pin MOSFET 26 OFF (CPT4)
delay(60000);            

  if (Serial.available())
  { 
    inchar=Serial.parseInt();
  }
 /*********************************************************
  *          Programme mesure du Rs pour capteur CPT1
  * *******************************************************
  */
  float Rl_p=10E+6; //10Mohms
  pinMode(Rp1,OUTPUT);
  digitalWrite(Rp1, LOW);
  pinMode(Rp2,INPUT);
  pinMode(Rp3,INPUT);
  pinMode(Rp4,INPUT);
  delay(DELAY_MS);
 int Vs_raw_p  = analogRead(heater_read_1);
 
  float Vs_1_p = Vs_raw_p * (VCC / 1023.0);
    
  if(Vs_1_p >VG)
  {
    Rl_p=1E+6; //1Mohms
    pinMode(Rp1,INPUT);
    pinMode(Rp2,OUTPUT);
    digitalWrite(Rp2, LOW);
    pinMode(Rp3,INPUT);
    pinMode(Rp4,INPUT);
    delay(DELAY_MS);
    Vs_raw_p = analogRead(heater_read_1);
    Vs_1_p = Vs_raw_p * (VCC / 1023.0);
    if(Vs_1_p>VG)
    {
      Rl_p=100E+3; //100kohms
      pinMode(Rp1,INPUT);
      pinMode(Rp2,INPUT);
      pinMode(Rp3,OUTPUT);
      digitalWrite(Rp3, LOW);
      pinMode(Rp4,INPUT);
      delay(DELAY_MS);
      Vs_raw_p  = analogRead(heater_read_1);
      Vs_1_p = Vs_raw_p * (VCC / 1023.0);
      if(Vs_1_p>VG)
      {
        Rl_p=10E+3; //10K
        pinMode(Rp1,INPUT);
        pinMode(Rp2,INPUT);
        pinMode(Rp3,INPUT);
        pinMode(Rp4,OUTPUT);
        digitalWrite(Rp4, LOW);
        delay(DELAY_MS);
        Vs_raw_p = analogRead(heater_read_1);
        Vs_1_p = Vs_raw_p * (VCC / 1023.0);
       }
    }
  }


/*********************************************************
  *          Programme mesure du Rs pour capteur CPT2
  * *******************************************************
  */
  float R2_p=10E+6; //10Mohms
  pinMode(Rp9,OUTPUT);
  digitalWrite(Rp9, LOW);
  pinMode(Rp10,INPUT);
  pinMode(Rp11,INPUT);
  pinMode(Rp12,INPUT);
  delay(DELAY_MS);
 int Vs_raw_2_p  = analogRead(heater_read_2);
 
  float Vs_2_p = Vs_raw_2_p * (VCC / 1023.0);
    
  if(Vs_2_p >VG)
  {
    R2_p=1E+6; //1Mohms
    pinMode(Rp9,INPUT);
    pinMode(Rp10,OUTPUT);
    digitalWrite(Rp10, LOW);
    pinMode(Rp11,INPUT);
    pinMode(Rp12,INPUT);
    delay(DELAY_MS);
    Vs_raw_2_p = analogRead(heater_read_2);
    Vs_2_p = Vs_raw_2_p * (VCC / 1023.0);
    if(Vs_2_p>VG)
    {
      R2_p=100E+3; //100kohms
      pinMode(Rp9,INPUT);
      pinMode(Rp10,INPUT);
      pinMode(Rp11,OUTPUT);
      digitalWrite(Rp11, LOW);
      pinMode(Rp12,INPUT);
      delay(DELAY_MS);
      Vs_raw_2_p  = analogRead(heater_read_2);
      Vs_2_p = Vs_raw_2_p * (VCC / 1023.0);
      if(Vs_2_p>VG)
      {
        R2_p=10E+3; //10K
        pinMode(Rp9,INPUT);
        pinMode(Rp10,INPUT);
        pinMode(Rp11,INPUT);
        pinMode(Rp12,OUTPUT);
        digitalWrite(Rp12, LOW);
        delay(DELAY_MS);
        Vs_raw_2_p = analogRead(heater_read_2);
        Vs_2_p = Vs_raw_2_p * (VCC / 1023.0);
       }
    }
  }

  /*********************************************************
  *          Programme mesure du Rs pour capteur CPT3
  * *******************************************************
  */
  float R3_p=10E+6; //10Mohms
  pinMode(Rp13,OUTPUT);
  digitalWrite(Rp13, LOW);
  pinMode(Rp14,INPUT);
  pinMode(Rp15,INPUT);
  pinMode(Rp16,INPUT);
  delay(DELAY_MS);
 int Vs_raw_3_p  = analogRead(heater_read_3);
 
  float Vs_3_p = Vs_raw_3_p * (VCC / 1023.0);
    
  if(Vs_3_p >VG)
  {
    R3_p=1E+6; //1Mohms
    pinMode(Rp13,INPUT);
    pinMode(Rp14,OUTPUT);
    digitalWrite(Rp14, LOW);
    pinMode(Rp15,INPUT);
    pinMode(Rp16,INPUT);
    delay(DELAY_MS);
    Vs_raw_3_p = analogRead(heater_read_3);
    Vs_3_p = Vs_raw_3_p * (VCC / 1023.0);
    if(Vs_3_p>VG)
    {
      R3_p=100E+3; //100kohms
      pinMode(Rp13,INPUT);
      pinMode(Rp14,INPUT);
      pinMode(Rp15,OUTPUT);
      digitalWrite(Rp15, LOW);
      pinMode(Rp12,INPUT);
      delay(DELAY_MS);
      Vs_raw_3_p  = analogRead(heater_read_3);
      Vs_3_p = Vs_raw_3_p * (VCC / 1023.0);
      if(Vs_3_p>VG)
      {
        R3_p=10E+3; //10K
        pinMode(Rp13,INPUT);
        pinMode(Rp14,INPUT);
        pinMode(Rp15,INPUT);
        pinMode(Rp16,OUTPUT);
        digitalWrite(Rp16, LOW);
        delay(DELAY_MS);
        Vs_raw_3_p = analogRead(heater_read_3);
        Vs_3_p = Vs_raw_3_p * (VCC / 1023.0);
       }
    }
  }

   /*********************************************************
  *          Programme mesure du Rs pour capteur CPT4
  * *******************************************************
  */
  float R4_p=10E+6; //10Mohms
  pinMode(Rp5,OUTPUT);
  digitalWrite(Rp5, LOW);
  pinMode(Rp6,INPUT);
  pinMode(Rp7,INPUT);
  pinMode(Rp8,INPUT);
  delay(DELAY_MS);
 int Vs_raw_4_p  = analogRead(heater_read_4);
 
  float Vs_4_p = Vs_raw_4_p * (VCC / 1023.0);
    
  if(Vs_4_p >VG)
  {
    R4_p=1E+6; //1Mohms
    pinMode(Rp5,INPUT);
    pinMode(Rp6,OUTPUT);
    digitalWrite(Rp6, LOW);
    pinMode(Rp7,INPUT);
    pinMode(Rp8,INPUT);
    delay(DELAY_MS);
    Vs_raw_4_p = analogRead(heater_read_4);
    Vs_4_p = Vs_raw_4_p * (VCC / 1023.0);
    if(Vs_4_p>VG)
    {
      R4_p=100E+3; //100kohms
      pinMode(Rp5,INPUT);
      pinMode(Rp6,INPUT);
      pinMode(Rp7,OUTPUT);
      digitalWrite(Rp7, LOW);
      pinMode(Rp8,INPUT);
      delay(DELAY_MS);
      Vs_raw_4_p  = analogRead(heater_read_4);
      Vs_4_p = Vs_raw_4_p * (VCC / 1023.0);
      if(Vs_4_p>VG)
      {
        R4_p=10E+3; //10K
        pinMode(Rp5,INPUT);
        pinMode(Rp6,INPUT);
        pinMode(Rp7,INPUT);
        pinMode(Rp8,OUTPUT);
        digitalWrite(Rp8, LOW);
        delay(DELAY_MS);
        Vs_raw_4_p = analogRead(heater_read_4);
        Vs_4_p = Vs_raw_4_p * (VCC / 1023.0);
       }
    }
  }
  
float  Rs_1_p= Rl_p*VCC/Vs_1_p-Rl_p;
float  Rs_2_p= R2_p*VCC/Vs_2_p-R2_p;
float  Rs_3_p= R3_p*VCC/Vs_3_p-R3_p;
float  Rs_4_p= R4_p*VCC/Vs_4_p-R4_p;


//***********************//

const int capacity_p = JSON_OBJECT_SIZE(6);

  StaticJsonDocument<capacity_p> doc_p;

  doc_p["SensorType"] = "GazSensors";

  doc_p["Cart"] = 2;

  doc_p["Sensor_1"] = "Bi_SnO2/WO3-CuO";

  doc_p["Value_Rs1_p"] = Rs_1_p;

  doc_p["Sensor_2"] = "Bi_SnO2/WO3-CuO";

  doc_p["Value_Rs2_p"] = Rs_2_p;

  doc_p["Sensor_3"] = "Bi_SnO2/WO3-CuO";

  doc_p["Value_Rs3_p"] = Rs_3_p;

  doc_p["Sensor_4"] = "Bi_SnO2/WO3-CuO";

  doc_p["Value_Rs4_p"] = Rs_4_p;
  
  String rep_p;
  serializeJson(doc_p,rep_p);
  Serial.println(rep_p);

  if (!client.connected()) {
     
    client.publish(TOPIC, rep_p.c_str());
    client.subscribe(TOPIC);
  }
  else{
    reconnect();
    }
    
}
