#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <ESP32Servo.h>

// Pin definitions
#define DHTPIN 4
#define DHTTYPE DHT22
#define SOIL_MOISTURE_PIN 34
#define RELAY_PIN 18
#define LED_PIN 2
#define SERVO_PIN 5

// Thresholds
const int moistureThreshold = 2000;
const int tempthreshold = 10;
const int humidityThreshold = 80;

// Components
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 20, 4); // For a 20x4 LCD
Servo waterPump;

void setup() {
  Serial.begin(115200);
  
  dht.begin();
  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Smart Irrigation Sys");
  
  waterPump.attach(SERVO_PIN);
  waterPump.write(0); // Initial position
  
  pinMode(SOIL_MOISTURE_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  
  digitalWrite(RELAY_PIN, HIGH); // Ensure relay is off initially
  digitalWrite(LED_PIN, LOW);    // Ensure LED is off initially
}

void startWatering() {
  waterPump.write(90); // Assume 90 degrees is the pumping position
  lcd.setCursor(0, 1);
  lcd.print("Watering ON ");
  Serial.println("Watering ON...");
  digitalWrite(LED_PIN, HIGH); // LED ON when watering
}

void stopWatering() {
  waterPump.write(0); // Assume 0 degrees is the off position
  lcd.setCursor(0, 1);
  lcd.print("Watering OFF");
  Serial.println("Watering OFF");
  digitalWrite(LED_PIN, LOW); // LED OFF when watering stops
}

void loop() {
  int soilMoistureValue = analogRead(SOIL_MOISTURE_PIN);
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  Serial.print("Soil Moisture: ");
  Serial.println(soilMoistureValue);
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println("C");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print("C Hum: ");
  lcd.print(humidity);
  lcd.print("%");

  // Wwtering logic
  if (soilMoistureValue < moistureThreshold && temperature > tempthreshold && humidity < humidityThreshold) {
    digitalWrite(RELAY_PIN, LOW); // Turn ON watering
    startWatering();
  } else {
    digitalWrite(RELAY_PIN, HIGH); // Turn OFF watering
    stopWatering();
  }

  delay(2000);
}
