#ifndef WhatabotAPIClient_h
#define WhatabotAPIClient_h

#include <ArduinoJson.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include "ArduinoWebSockets/src/WebSocketsClient.h"

class WhatabotAPIClient {
public:
    WhatabotAPIClient(const char* apiKey, const char* chatId, const char* platform);
    void begin();
    void loop();
    void onMessageReceived(void (*callback)(String));
	void onServerResponseReceived(void (*callback)(String));
	void sendMessageWS(String text);
	void sendMessageREST(String text);	
private:
    WebSocketsClient webSocket;
	WiFiClientSecure client;
	const char* host; 
	const char* apiKey; 
	const char* chatId; 
	const char* platform; 
    void webSocketEvent(WStype_t type, uint8_t* payload, size_t length);
    void processJsonMessage(uint8_t* payload, size_t length);
    void (*textReceivedCallback)(String);
	void (*serverResponseReceivedCallback)(String);
};

#endif