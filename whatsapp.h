#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include "facebookGraphCert.h"

const char ssid[] = WIFI_SSID;
const char pass[] = WIFI_PASS;
const String whatsapp_access_token = WHATSAPP_ACCESS_TOKEN;
const String target_phone_number = TARGET_PHONE_NUMBER;
const String fb_graph_messages_url = "https://graph.facebook.com/v20.0/431609330040853/messages";

WiFiMulti WiFiMulti;

void connectWIFI() {
  WiFi.disconnect(true);
  WiFiMulti.addAP(ssid, pass);
  WiFi.mode(WIFI_STA);
  while ((WiFiMulti.run() != WL_CONNECTED)) {
    Serial.print(".");
  }
  Serial.println("WiFi Connected");
}

void sendWhatsapp() {
  Serial.print("Send whatsapp");
  WiFiClientSecure *client = new WiFiClientSecure;
  if (client) {
    client -> setCACert(FACEBOOK_GRAPH_CERT);
    {
      HTTPClient https;

      if (https.begin(*client, fb_graph_messages_url)) {
        https.addHeader("Content-Type", "application/json");
        https.addHeader("Authorization", "Bearer " + String(WHATSAPP_ACCESS_TOKEN));

        String jsonPayload = "{\"messaging_product\":\"whatsapp\",\"to\":\"" + target_phone_number + "\",\"type\":\"template\",\"template\":{\"name\":\"statement_available_2\",\"language\":{\"code\":\"en_US\"}}}";
        int httpCode = https.POST(jsonPayload);

        if (httpCode > 0) {
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            String payload = https.getString();
            Serial.print("HTTP Response: ");
            Serial.println(payload);
          }
        } else {
          Serial.print("HTTP Post failed: ");
          Serial.println(https.errorToString(httpCode).c_str());
        }

        https.end();
      }
    }

    delete client;
  }
}
