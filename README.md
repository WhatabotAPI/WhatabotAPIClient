

# Whatabot API Client Library  
## Overview The Whatabot API Client Library is designed to simplify the integration of Whatabot's services into Arduino projects. This library uses websockets to receive real-time WhatsApp messages, using whatabot.net as a broker. It provides an easy-to-use interface for handling incoming messages and interacting with the WhatsApp platform.

 ## Installation  
 ### Dependencies 
 Make sure to install the following libraries before using the Whatabot API Client Library:
  - [ArduinoJson](https://github.com/bblanchon/ArduinoJson) 
  - [WiFiManager](https://github.com/tzapu/WiFiManager) 
  
  ### Library Installation 
  1. Open the Arduino IDE. 
  2. Go to "Sketch" -> "Include Library" -> "Manage Libraries..." 
  3. Search for "Whatabot API Client Library" and click "Install."

 ## Usage  
### Constructor
```cpp 
WhatabotAPIClient(const char* apiKey, const char* chatId, const char* platform);
-   void begin(): Initializes the Whatabot API Client.
-   void loop(): Handles the main loop for the Whatabot API Client.
-   void onMessageReceived(void (*callback)(String)): Sets a callback function for handling received messages.
-   void onServerResponseReceived(void (*callback)(String)): Sets a callback function for handling all server responses.
-   void sendMessageWS(String text): Sends a message using websockets.
-   void sendMessageREST(String text): Sends a message using REST.```
```
## Example WebSocket
### Receiving and sending Whatsapp messages from ESP8266
```cpp 
#include <WhatabotAPIClient.h>
#include <WiFiManager.h>

#define WHATABOT_API_KEY "YOUR_WHATABOT_API_KEY"
#define WHATABOT_CHAT_ID "YOUR_PHONE_NUMBER"
#define WHATABOT_PLATFORM "whatsapp"

WiFiManager wifiManager;
WhatabotAPIClient whatabotClient(WHATABOT_API_KEY, WHATABOT_CHAT_ID, WHATABOT_PLATFORM);

#define AP_SSID "WhatabotAPI"
#define AP_PASS "whatabotapi"

void setup() {
  Serial.begin(115200);
  wifiManager.autoConnect(AP_SSID, AP_PASS);
  whatabotClient.begin();
  whatabotClient.onMessageReceived(onMessageReceived);
  whatabotClient.onServerResponseReceived(onServerResponseReceived);
}

void loop() {
  whatabotClient.loop();
}

void onServerResponseReceived(String message) {
  Serial.println(message);
}

void onMessageReceived(String message) {
  Serial.println(message);
  whatabotClient.sendMessageWS("Pong: " + message);
}
```

## Example REST
### Sending Whatsapp messages from ESP8266
```cpp 
#include <WhatabotAPIClient.h>
#include <WiFiManager.h>

#define WHATABOT_API_KEY "YOUR_WHATABOT_API_KEY"
#define WHATABOT_CHAT_ID "YOUR_PHONE_NUMBER"
#define WHATABOT_PLATFORM "whatsapp"

WiFiManager wifiManager;
WhatabotAPIClient whatabotClient(WHATABOT_API_KEY, WHATABOT_CHAT_ID, WHATABOT_PLATFORM);

#define AP_SSID "WhatabotAPI"
#define AP_PASS "whatabotapi"

void setup() {
  Serial.begin(115200);
  wifiManager.autoConnect(AP_SSID, AP_PASS);
  whatabotClient.sendMessageREST("Hi from Whatabot for ESP8266");
}

void loop() {
}
```