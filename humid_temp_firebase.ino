#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>
#include <DHT.h>

#define DHTPIN 14         //D5
#define DHTTYPE DHT11
#define LED 4   //D2

DHT dht(DHTPIN, DHTTYPE);

float temp;
float humid;

FirebaseData ledData;
FirebaseData dataFirebase;

char* wifi_name = "*********";          // your wifi name here
char* password = "*********";           // your wifi password here

char* database_secret = "*********";    // Real-time database secret key here
char* firebase_addr = "********";       // Enter database HOST without "https:// "  and "/" at the end of URL

void setup(){
    Serial.begin(115200);
    pinMode(LED, OUTPUT);
    wifi_connect();
    Firebase.begin(firebase_addr, database_secret);
    pinMode(DHTPIN, INPUT);
    dht.begin();
}

void loop(){
    
    sensorUpdate();
    ledStatus();
    delay(100);
}

void wifi_connect(){
    Serial.println("start connecting");
    WiFi.begin(wifi_name, password);
    while(!(WiFi.status() == WL_CONNECTED)){
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nconnected");
}

void sensorUpdate(){
    temp = dht.readTemperature();
    humid = dht.readHumidity();
    Serial.println(temp);
    Serial.println(humid);
    Firebase.setFloat(dataFirebase, "/FirebaseIOT/temperature", temp);
    Firebase.setFloat(dataFirebase, "/FirebaseIOT/humidity", humid);
}

void ledStatus(){
    if (Firebase.getString(ledData, "/FirebaseIOT/led")){
        Serial.println(ledData.stringData());
        if (ledData.stringData() == "1") {
            digitalWrite(LED, HIGH);
        }
        else if (ledData.stringData() == "0"){
            digitalWrite(LED, LOW);
        }
    }
}

