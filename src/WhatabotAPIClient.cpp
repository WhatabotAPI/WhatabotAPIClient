#include "WhatabotAPIClient.h"

WhatabotAPIClient::WhatabotAPIClient(const char* apiKey, const char* chatId, const char* platform) {
	this->host = "apiv2.whatabot.net";	
	this->apiKey = apiKey;
    this->chatId = chatId;
    this->platform = platform;	
}

void WhatabotAPIClient::begin() {	
    Serial.println("Connecting to Whatabot API...");
    webSocket.beginSSL(host, 443, "/realtimeMessages");
    String headers = "x-api-key: " + String(apiKey) + "\r\n" +
                     "x-chat-id: " + String(chatId) + "\r\n" +
                     "x-platform: " + String(platform);
    webSocket.setExtraHeaders(headers.c_str());

    webSocket.setReconnectInterval(20000);
    webSocket.onEvent([this](WStype_t type, uint8_t* payload, size_t length) {
        this->webSocketEvent(type, payload, length);
    });
}

void WhatabotAPIClient::loop() {
    webSocket.loop();
}

void WhatabotAPIClient::onMessageReceived(void (*callback)(String)) {
    textReceivedCallback = callback;
}

void WhatabotAPIClient::onServerResponseReceived(void (*callback)(String)) {
    serverResponseReceivedCallback = callback;
}

void WhatabotAPIClient::webSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
    switch (type) {
        case WStype_DISCONNECTED:
            Serial.println("Disconnected from Whatabot API server");
            break;
        case WStype_CONNECTED:
            Serial.println("Connected to Whatabot API server");
            webSocket.sendTXT("{\"protocol\":\"json\",\"version\":1}");
            break;
        case WStype_TEXT:		
            processJsonMessage(payload, length);
            break;
        default:
            break;
    }
}

void WhatabotAPIClient::processJsonMessage(uint8_t* payload, size_t length) {
    char jsonString[length + 1];
    memcpy(jsonString, payload, length);
    jsonString[length] = '\0';

    DynamicJsonDocument doc(length * 2);
    deserializeJson(doc, jsonString);
	
	if(doc.containsKey("target"))
	{
		if (doc.containsKey("type")) 
		{		
			if (doc.containsKey("arguments")) 
			{
				int messageType = doc["type"];
				String target = doc["target"];
				if (messageType == 1)
				{
					serverResponseReceivedCallback(jsonString);
					if(target == "ReceiveMessage") 
					{
						const char* text = doc["arguments"][0];
						textReceivedCallback(String(text));
					}
				}
			}
		}	
	}    
}

void WhatabotAPIClient::sendMessageWS(String text) {	
	DynamicJsonDocument jsonDocument(200);
	jsonDocument["type"] = 1;
	jsonDocument["target"] = "SendMessage";

    JsonArray argumentsArray = jsonDocument.createNestedArray("arguments");
    argumentsArray.add(text);
	
    String jsonString;
    serializeJson(jsonDocument, jsonString);
	String messageToSend = jsonString + "";
	webSocket.sendTXT(messageToSend);
}


void WhatabotAPIClient::sendMessageREST(String text) {
    Serial.print("Trying to send message");

    if (WiFi.status() == WL_CONNECTED) { 
		HTTPClient http;
        std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
        client->setInsecure();  // Ignore SSL certificate

        Serial.print("[HTTPS] begin...\n");
        if (http.begin(*client, "https://" + String(host) + "/Whatsapp/RequestSendMessage")) {  // HTTPS
			
			http.setTimeout(20000);
            Serial.println("[HTTPS] POST...\n");
            http.addHeader("Content-Type", "application/json");
        
            String jsonPayload = "{\"ApiKey\":" + String(apiKey) +
                                ",\"Text\":\"" + text +
                                "\",\"Phone\":" + String(chatId) + "}";	

            int httpCode = http.POST(jsonPayload);
			
			Serial.println("hhtp");
			Serial.println(httpCode);

            if (httpCode > 0) {
                Serial.printf("[HTTPS] POST... code: %d\n", httpCode);

                if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
                    String payload = http.getString();
                    Serial.println(payload);
                }
            } else {
                Serial.printf("[HTTPS] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
            }

            http.end();
        } else {
            Serial.printf("[HTTPS] Unable to connect\n");
        }
    } else {
        Serial.println("WiFi not connected!");
    }
}