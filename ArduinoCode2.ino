#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPLcq1wP69k"
#define BLYNK_DEVICE_NAME "SilienciaTec"
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#define DHTPIN D2
#define DHTTYPE DHT11

#define BLYNK_AUTH_TOKEN "15Lc7g7JwsUu-HaOe-wSHV8yGAHCekSM"
char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "112";
char pass[] = "3336296163";

//Define the API Key
#define API_KEY "AIzaSyBoCSP5XCyp0_Tsm14TfcrGfTbB40Q1pZM"
//Credenciales Proyecto Firebase */
const char *FIREBASE_HOST="https://equipo-3-tc1004b-default-rtdb.firebaseio.com/"; 
const char *FIREBASE_AUTH="jvDOicwqkWEfJHo5J6yGiEdfCjnxQjlNiyu1l7bw";

String path = "/Biblioteca Campus GDL";
FirebaseData firebaseData;
bool iterar = true;

BlynkTimer timer;

int ledRojo = D6;
int ledVerde = D7;
int ledAzul = D8;

const int sampleWindow = 50;                              // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;
#define SENSOR_PIN A0

float t;
float h;

const byte pinBuzzer = D5;
  

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  Serial.begin(115200);
  pinMode(ledRojo,OUTPUT);
  pinMode(ledVerde,OUTPUT);
  pinMode(ledAzul,OUTPUT);
  pinMode (SENSOR_PIN, INPUT);
  
  Blynk.begin(auth, ssid, pass);
  Serial.println(F("DHTxx test!"));
  dht.begin();
  Serial.print("Conectando...");
  while (WiFi.status()!= WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conexión OK!");
  Serial.print("IP Local: ");
  Serial.println(WiFi.localIP());
 
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}

void loop()
{
  Blynk.run();
  delay (500);
  // You can inject your own code or combine it with other sketches.
  // Check ther examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
}

    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.print(F("%  Temperature: "));
    Serial.print(t);
    Serial.print(F("°C "));

// Conversion de señal a db ........................................................................................................................................
   
   unsigned long startMillis= millis();                   // Start of sample window
   float peakToPeak = 0;                                  // peak-to-peak level
 
   unsigned int signalMax = 0;                            //minimum value
   unsigned int signalMin = 1024;                         //maximum value
 
                                                          // collect data for 50 mS
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(SENSOR_PIN);                    //get reading from microphone
      if (sample < 1024)                                  // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;                           // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;                           // save just the min levels
         }
      }
   }
 
   peakToPeak = signalMax - signalMin;                    // max - min = peak-peak amplitude
   int db = map(peakToPeak,20,900,49.5,90);             //calibrate for deciBels

  delay(500);
  Serial.print("Loudness: ");
  Serial.print(db);
  Serial.println(" dB");

//Actuador del LED RGB ........................................................................................................................................................
   if(db>90){
    digitalWrite(ledRojo,255);
    digitalWrite(ledVerde,0);
    digitalWrite(ledAzul,0);
    analogWrite(pinBuzzer, 100);
   }
   else if(db>50){
    digitalWrite(ledRojo,0);
    digitalWrite(ledVerde,0);
    digitalWrite(ledAzul,255);
    analogWrite(pinBuzzer, 0);
   }
   else {
    digitalWrite(ledRojo,0);
    digitalWrite(ledVerde,255);
    digitalWrite(ledAzul,0);
    analogWrite(pinBuzzer, 0);
   }

  //Envio de datos a blynk ................................................................................................................................................

   Blynk.virtualWrite(V1,h);
   Blynk.virtualWrite(V0,t);
   Blynk.virtualWrite(V2,db);
 
 // Envio de datos a FireBase .............................................................................................................................................

  String nodo = "Proyecto-iot";
  Firebase.setFloat(firebaseData, path + "/Temperatura", t);
  delay(150);
  Firebase.setFloat(firebaseData, path + "/Humedad", h);
  delay(150);
  Firebase.setInt(firebaseData, path + "/Decibeles", db);
  
  
  
  
  
}
