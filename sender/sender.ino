#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>
#include <PubSubClient.h>

// LoRa pin definitions
#define SCLK 5
#define MISO 19
#define MOSI 27
#define CS 18
#define RST 14
#define DI0 26

#define TRIGGER_PIN 14
#define ECHO_PIN 12

// Wi-Fi and MQTT broker settings
const char* ssid = "La_Fibre_dOrange_2.4G_8BFA";
const char* password = "QFSHEX9CP4972HS2FD";
const char* mqttServer = "192.168.11.110";
const int mqttPort = 1883;
const char* mqttTopic = "valeur";

// MQTT client
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);


void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);  

  // Initialize LoRa module
  SPI.begin(SCLK, MISO, MOSI, CS);
  LoRa.setPins(CS, RST, DI0);

  if (!LoRa.begin(866E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Initializing OK!");

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Connect to MQTT broker
  mqttClient.setServer(mqttServer, mqttPort);
  while (!mqttClient.connected()) {
    if (mqttClient.connect("ESP32Client")) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("Failed to connect to MQTT broker, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" Retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void loop() {
  // Send data via LoRa
    long d, distance;
  
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  
  d = pulseIn(ECHO_PIN, HIGH);
  distance = d * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  LoRa.beginPacket();
  LoRa.print(distance);
  LoRa.endPacket();
  Serial.println("Sent data via LoRa");

  
}