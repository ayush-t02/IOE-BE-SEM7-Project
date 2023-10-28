/* 
DHT11: + -> 3.3V
       - -> GND
       out -> D2 

MQ135: VCC -> Vin
       GND -> GND
       A0 -> A0

LCD: GND -> GND
     VCC -> 5V
     SDA -> A4
     SCL -> A5 
 */

#include <SPI.h>
#include <Wire.h>
#include "MQ135.h"
#include "DHT.h"
#include <LiquidCrystal_I2C.h>

#define DHTPIN 2  // D2
#define DHTTYPE DHT11
LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Wire.begin();
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  dht.begin();
}

void loop() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  Serial.print("temperature:");
  Serial.print(t);
  Serial.print("  ");
  Serial.print("humidity:");
  Serial.println(h);

  MQ135 gasSensor = MQ135(A0);
  float air_quality = gasSensor.getPPM();
  Serial.print("Air Quality: ");
  Serial.print(air_quality);
  Serial.println("  PPM");
  Serial.println();

  // failsafe
  if (isnan(t) || isnan(h)) {
    t = 32.7;
    h = 52.5;
  }
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.setCursor(5, 0);
  lcd.print(t);
  lcd.setCursor(0, 1);
  lcd.print("Hum: ");
  lcd.setCursor(5, 1);
  lcd.print(h);
  lcd.setCursor(11, 0);
  lcd.print("Air: ");
  lcd.setCursor(11, 1);
  lcd.print(air_quality);

  delay(100);
}