#include <WiFiNINA.h>
#include <BH1750.h>
#include <Wire.h>

char ssid[] = "YOUR_WIFI_SSID"; 
char pass[] = "YOUR_WIFI_PASSWORD"; 

WiFiClient client;
BH1750 lightMeter;

char HOST_NAME[] = "maker.ifttt.com"; 
String EVENT_NAME = "high_light_detected"; 
String IFTTT_WEBHOOK_KEY = "YOUR_IFTTT_WEBHOOK_KEY"; 
String PATH_NAME = "/trigger/" + EVENT_NAME + "/with/key/" + IFTTT_WEBHOOK_KEY; 
String queryString = "?value1=0"; 

const int thresholdLux = 500; 

void setup() {
  Serial.begin(9600); 
  while (!Serial); 

  connectToWiFi();

  Wire.begin(); 
  lightMeter.begin(); 
}

void loop() {
  float lux = lightMeter.readLightLevel(); 
  Serial.print("Current Light Level: ");
  Serial.println(lux);

  if (lux > thresholdLux) {
    sendLuxToIFTTT(lux); 
  }

  delay(3000); 
}

void connectToWiFi() {
  WiFi.begin(ssid, pass); 
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) { 
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nConnected to WiFi");
  Serial.print("Local IP Address: ");
  Serial.println(WiFi.localIP()); 
}

void sendLuxToIFTTT(float lux) {
  if (client.connect(HOST_NAME, 80)) { 
    queryString = "?value1=" + String(lux);

    client.print("GET " + PATH_NAME + queryString + " HTTP/1.1\r\n");
    client.print("Host: " + String(HOST_NAME) + "\r\n");
    client.print("Connection: close\r\n\r\n");

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.print(c);
      }
    }

    client.stop(); 
  } else {
    Serial.println("Failed to connect to IFTTT Server");
  }
}
