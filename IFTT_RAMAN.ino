#include <SPI.h>
#include <WiFiNINA.h>

// Replace with your Wi-Fi credentials
char wifiSSID[] = "raman";
char wifiPassword[] = "kuku1234";

// Replace with your IFTTT webhook key and event names
const char* iftttWebhookKey = "bRwkLEo0YZcqqZxyjDX4ashCDY5o7io1yeNKM1RMTp5";
const char* startSunlightEvent = "sunlight_start";
const char* stopSunlightEvent = "sunlight_stop";

const int lightSensorPin = A0;
bool isSunlightDetected = false;

WiFiClient wifiClient;

void setup() {
  Serial.begin(9600);

  // Connect to Wi-Fi
  while (WiFi.begin(wifiSSID, wifiPassword) != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  int lightSensorValue = analogRead(lightSensorPin);

  // Check for the start of sunlight
  if (!isSunlightDetected && lightSensorValue < 750) {
    sendIFTTTEvent(startSunlightEvent);
    isSunlightDetected = true;
  } 
  // Check for the stop of sunlight
  else if (isSunlightDetected && lightSensorValue > 750) {
    sendIFTTTEvent(stopSunlightEvent);
    isSunlightDetected = false;
  }

  delay(1000);  // Delay between readings
}

void sendIFTTTEvent(const char* eventName) {
  if (wifiClient.connect("maker.ifttt.com", 80)) {
    Serial.println("Connected to IFTTT");

    // Create the URL with event name and webhook key
    String url = "/trigger/";
    url += eventName;
    url += "/with/key/";
    url += iftttWebhookKey;

    // Send an HTTP POST request to IFTTT
    wifiClient.println("POST " + url + " HTTP/1.1");
    wifiClient.println("Host: maker.ifttt.com");
    wifiClient.println("Connection: close\r\n\r\n");
    delay(10);

    // Print the response from IFTTT
    while (wifiClient.available()) {
      char c = wifiClient.read();
      Serial.print(c);
    }
    Serial.println();
    wifiClient.stop();
    Serial.println("IFTTT event sent");
  } else {
    Serial.println("Connection to IFTTT failed");
  }
}