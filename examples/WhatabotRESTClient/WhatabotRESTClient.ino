#include <WhatabotAPIClient.h>
#include <WiFiManager.h>

#define WHATABOT_API_KEY "YOUR_WHATABOT_API_KEY"
#define WHATABOT_CHAT_ID "YOUR_PHONE_NUMBER"
#define WHATABOT_PLATFORM "whatsapp"

WiFiManager wifiManager;
WhatabotAPIClient whatabotClient(WHATABOT_API_KEY, WHATABOT_CHAT_ID, WHATABOT_PLATFORM);

//You must connect to the generated network, and from there, you can set the SSID and password for your Wi-Fi network to which the ESP8266 will connect.
//If you dont want to use WiFiManager you can still connect to WiFi using other logic.
#define AP_SSID "WhatabotAPI"
#define AP_PASS "whatabotapi"

void setup() {
  Serial.begin(115200);
  wifiManager.autoConnect(AP_SSID, AP_PASS);
  whatabotClient.sendMessageREST("Hi from Whatabot for ESP8266");
}

void loop() {
 
}