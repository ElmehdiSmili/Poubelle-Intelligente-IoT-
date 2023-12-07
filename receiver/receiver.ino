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


// Wi-Fi and MQTT broker settings
const char* ssid = "nina";
const char* password = "12345678";
const char* mqttServer = "192.168.137.81";
const int mqttPort = 1883;
const char* mqttTopic = "valeur";

// MQTT client
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

String LoRaData;

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

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

  Serial.print(WiFi.localIP());
  // Connect to MQTT broker
  mqttClient.setServer(mqttServer, mqttPort);
  String clientId = "ESP32Client";
  while (!mqttClient.connected()) {
    if (mqttClient.connect(clientId.c_str())) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("Failed to connect to MQTT broker, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" Retrying in 5 seconds...");
      String clientId = "ESP32Client-";
      clientId += String(random(0xffff), HEX);
    }
  }
}

void loop() {
  // Try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // Received a packet
    Serial.print("Received packet: ");

    // Read packet
    while (LoRa.available()) {
      LoRaData = LoRa.readString();
      Serial.print(LoRaData);
    }
    Serial.println();

    // Publish data to MQTT broker
    if (mqttClient.connected()) {
      mqttClient.publish(mqttTopic, LoRaData.c_str());
      Serial.println("Published data to MQTT broker");
    } else {
      Serial.println("Failed to publish data, MQTT client disconnected");
    }
  }

  // Maintain MQTT connection
  if (!mqttClient.connected()) {
    reconnect();
  }

  mqttClient.loop();
}

void reconnect() {
  // Loop until reconnected to MQTT broker
  while (!mqttClient.connected()) {
    Serial.println("Attempting MQTT connection...");
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