#include <DHT.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

// use your blynk libraray and blynk auth token in the code as needed 
#define LDR_PIN 34
#define PIR_PIN 27
#define LIGHT_LED 12

#define DHTPIN 4
#define DHTTYPE DHT11

#define FAN_RELAY 2
#define PUMP_RELAY 25

#define TRIG_PIN 5
#define ECHO_PIN 18

#define VIB_PIN 26

#define LDR_THRESHOLD 2000
#define MOTION_TIMEOUT 10000

const int MIN_LEVEL = 10;
const int MAX_LEVEL = 25;

const char* ssid = "Your WiFi_SSID";
const char* password = "Your WiFi_Password";

String botToken = "Your Telegram_Bot_Token";
String chatID = "Your Telegram_Chat_ID";

WiFiClientSecure client;

unsigned long lastMotionTime = 0;
bool motionActive = false;
unsigned long lastVibrationTime = 0;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);

  pinMode(LIGHT_LED, OUTPUT);
  pinMode(FAN_RELAY, OUTPUT);
  pinMode(PUMP_RELAY, OUTPUT);

  digitalWrite(FAN_RELAY, HIGH);
  digitalWrite(PUMP_RELAY, HIGH);

  pinMode(PIR_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);
  pinMode(VIB_PIN, INPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  dht.begin();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");

  client.setInsecure();

  Serial.println("ESP32 SMART AUTOMATION SYSTEM STARTED");
}

void loop() {

  if (digitalRead(PIR_PIN) == HIGH) {
    lastMotionTime = millis();
    motionActive = true;
    Serial.println("Motion detected");
  }

  int ldrValue = analogRead(LDR_PIN);

  if (ldrValue < LDR_THRESHOLD && motionActive) {
    digitalWrite(LIGHT_LED, HIGH);
  } else {
    digitalWrite(LIGHT_LED, LOW);
  }

  if (motionActive && millis() - lastMotionTime > MOTION_TIMEOUT) {
    digitalWrite(LIGHT_LED, LOW);
    digitalWrite(FAN_RELAY, LOW);
    motionActive = false;
    Serial.println("No motion Light & Fan OFF");
  }

  float temperature = dht.readTemperature();

  if (!isnan(temperature)) {
    Serial.print("Temp: ");
    Serial.print(temperature);
    Serial.println(" C");

    if (temperature < 22 && motionActive) {
      digitalWrite(FAN_RELAY, LOW);
      Serial.println("Fan ON");
    } else if (motionActive) {
      digitalWrite(FAN_RELAY, HIGH);
      Serial.println("Fan OFF");
    }
  }

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration > 0) {
    int distance = duration * 0.034 / 2;

    Serial.print("Water Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    if (distance <= MAX_LEVEL) {
      digitalWrite(PUMP_RELAY, LOW);
      Serial.println("Pump OFF");
    } else if (distance >= MIN_LEVEL) {
      digitalWrite(PUMP_RELAY, HIGH);
      Serial.println("Pump ON");
    }
  }

  if (digitalRead(VIB_PIN) == HIGH && millis() - lastVibrationTime > 15000) {
    Serial.println("Vibration detected");
    sendTelegram("Vibration Detected!");
    lastVibrationTime = millis();
  }

  Serial.print("LDR: ");
  Serial.print(ldrValue);
  Serial.print(" | Motion: ");
  Serial.println(motionActive ? "YES" : "NO");
  Serial.println("--------------------------------");

  delay(2000);
}

void sendTelegram(String msg) {
  String url = "/bot" + botToken + "/sendMessage?chat_id=" + chatID + "&text=" + msg;

  if (!client.connect("api.telegram.org", 443)) {
    Serial.println("Telegram Connection Failed");
    return;
  }

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: api.telegram.org\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("Telegram Sent");
}
