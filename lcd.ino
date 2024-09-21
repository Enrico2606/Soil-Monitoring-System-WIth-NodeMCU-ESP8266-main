#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const char* ssid     = ""; // wifi name
const char* password = ""; // wifi password

const char* serverName = ""; //backend url

String key = ""; // unique key

String sensorID = ""; //id sensor

const int air_value = 770;
const int water_value = 393;
const int sensor_pin = A0;
int output_value ;
int kelembapan_cabe = 70;
int output_kurang;
int output_lebih;

void setup() {

   Serial.begin(115200);
 
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected");  

}
 
void loop() {
  //Kode loop silahkan disesuaikan
  output_value = analogRead(sensor_pin);
  output_value = map(output_value,air_value,water_value,0,100);
  Serial.print(output_value);

  display.setCursor(0, 16);

  display.print("Humid: ");
  display.print(output_value);
  display.println("%");
  
  if(output_value < kelembapan_cabe){
    output_kurang = kelembapan_cabe - output_value;
    display.print("Lack : ");
    display.print(output_kurang);
    display.println("%");
    }
  else{
    output_lebih = output_value - kelembapan_cabe;
    display.print("Over : ");
    display.print(output_lebih);
    display.println("%");
    }
     
  display.display();
  
  if(WiFi.status()== WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;
    
    http.begin(client, serverName);
    
    http.addHeader("Content-Type", "application/x-www-form-urlencoded", "application/json");
    
    String httpRequestData = "key=" + key + "&alat_id=" + sensorID + "&kelembapan=" + output_value;
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
    
    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);
     
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }

  delay(3000);
  display.clearDisplay();

}
