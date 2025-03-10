#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPDash.h>
#include <SPI.h>
#include <Wire.h>
#include "MQ135.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// #include <LiquidCrystal_I2C.h>
// LiquidCrystal_I2C lcd(0x27, 16, 2);

/* Your Soft AP WiFi Credentials */
const char *ssid = "IoT Smart Kitchen"; // WiFi Name
const char* password = "1234567890"; // Password

#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)

#define DHTTYPE DHT11     // DHT 11
#define DHTPIN D4 //DHT11 Pin D4(GPIO 2)
#define flame D6

#define relay1 D5 // Light D5(GPIO 14)
#define relay4 D8 // Fan D8(GPIO 15)

#define buzzer D0 //Buzzer Pin D0(GPIO 16)
#define buzzer1 D7 //Buzzer Pin D0(GPIO 16)
#define PIR D3 //PIR Sensor Pin D3(GPIO 0)
 
int alarm_status;
int pir_status = 0;
int flame_status;
 
DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/* Start Webserver */
AsyncWebServer server(80);

/* Attach ESP-DASH to AsyncWebServer */
ESPDash dashboard(&server); 
/* 
  Dashboard Cards 
  Format - (Dashboard Instance, Card Type, Card Name, Card Symbol(optional) )
*/
Card AIQ(&dashboard, GENERIC_CARD, "Air Quality Index", "PPM");
Card Buzzer(&dashboard, STATUS_CARD, "AQI Buzzer");
Card temperature(&dashboard, TEMPERATURE_CARD, "Temperature", "°C");
Card humidity(&dashboard, HUMIDITY_CARD, "Humidity", "%");
Card Fire_Alert(&dashboard, STATUS_CARD, "Fire");
Card Buzzer1(&dashboard, STATUS_CARD, "Fire Buzzer");
Card light(&dashboard, BUTTON_CARD, "Light");
Card fan(&dashboard, BUTTON_CARD, "Fan");
Card Motion_PIR(&dashboard, STATUS_CARD, "Human");



void setup() {
  Serial.begin(115200);

  /* Connect WiFi */
  WiFi.softAP(ssid, password);
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
  /* Start AsyncWebServer */
  server.begin();
  
  dht.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // lcd.init();
  // lcd.backlight();

  pinMode(PIR, INPUT);
  pinMode(flame, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(buzzer1, OUTPUT);
  pinMode(relay1, OUTPUT);
  pinMode(relay4, OUTPUT);

  digitalWrite(buzzer, LOW);
  digitalWrite(buzzer1, LOW);
  digitalWrite(relay1, LOW);
  digitalWrite(relay4, LOW);
  delay(100);
}


void loop() {
  /* Update Card Values */
   MQ135 gasSensor = MQ135(A0);
  float air_quality = gasSensor.getPPM();
 
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  flame_status = digitalRead(flame);
 
  pir_status = digitalRead(PIR);
  alarm_status = digitalRead(buzzer);

 
  if (pir_status == 1)
  {
    Serial.println("Motion Detected");
    Motion_PIR.update("Motion Detected", "success");
    dashboard.sendUpdates();
  }
  else if (pir_status == 0)
  {
    Serial.println("Motion not Detected");
    Motion_PIR.update("Motion not Detected", "danger");
    dashboard.sendUpdates();
  }
 
   if (flame_status == LOW)
  {
      digitalWrite(buzzer1, HIGH);
      Serial.println("Buzzer Status: ON");
      Buzzer1.update("ON", "warning");
      Serial.println("Fire Detected");
      Fire_Alert.update("Fire Detected", "danger");
      dashboard.sendUpdates();
  }
  else if (flame_status == HIGH)
  {
      digitalWrite(buzzer1, LOW);
      Serial.println("Buzzer Status: OFF");
      Buzzer1.update("OFF", "success");
      Serial.println("No Fire");
      Fire_Alert.update("No Fire", "success");
      dashboard.sendUpdates();
  }

 
  if (air_quality > 150)
  {
    digitalWrite(buzzer, HIGH);
    digitalWrite(relay4, HIGH);
    Serial.println("Buzzer Status: ON");
    Serial.println("Exhaust Fan: ON");
    Buzzer.update("ON", "warning");
    fan.update(HIGH);
    dashboard.sendUpdates();
  }
  else
  {
    digitalWrite(buzzer, LOW);
    digitalWrite(relay4, LOW);
    Serial.println("Buzzer Status: OFF");
    Serial.println("Exhaust Fan: OFF");
    Buzzer.update("OFF", "success");
    fan.update(LOW);
    dashboard.sendUpdates();
  }
 
  Serial.print("Air Quality: ");
  Serial.print(air_quality);
  Serial.println(" PPM");
 
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C");
 
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println(" %");

 
  Serial.println();
  Serial.println("****************************");
  Serial.println();
 
  
  temperature.update(t);
  humidity.update(h);
  AIQ.update(air_quality);
  //Button for Light
  light.attachCallback([&](bool value){
  Serial.println("[light] Button Callback Triggered: "+String((value)?"HIGH":"LOW"));
  digitalWrite(relay1, (value));
  light.update(value);
  dashboard.sendUpdates();
});


//Button for Fan
  fan.attachCallback([&](bool value){
  Serial.println("[fan] Button Callback Triggered: "+String((value)?"HIGH":"LOW"));
  digitalWrite(relay4, (value));
  fan.update(value);
  dashboard.sendUpdates();
});
  /* Send Updates to our Dashboard (realtime) */
  dashboard.sendUpdates();
  delay(1500);
 
  // display temperature

  display.clearDisplay();
  display.setCursor(0, 0); //oled display
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("Air Quality Index");
 
  display.setCursor(0, 20); //oled display
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print(air_quality);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println(" PPM");
  display.display();
  delay(1500);
 
  display.clearDisplay();
 
  // display temperature
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Temperature: ");
  display.setTextSize(2);
  display.setCursor(0, 10);
  display.print(t);
  display.print(" ");
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);
  display.setTextSize(2);
  display.print("C");
 
  // display humidity
  display.setTextSize(1);
  display.setCursor(0, 35);
  display.print("Humidity: ");
  display.setTextSize(2);
  display.setCursor(0, 45);
  display.print(h);
  display.print(" %");
 
  display.display();

  delay(1500);
}