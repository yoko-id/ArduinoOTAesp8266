/*
 *	Custom OTA page in bitearray format
 *
 *	Renzo Mischianti <www.mischianti.org>
 *
 *	https://www.mischianti.org/category/tutorial/esp8266-tutorial/esp8266-firmware-management-tutorial/
 */


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include "index_color.h"
#ifndef STASSID
#define STASSID "<YOUR-SSID>"
#define STAPSK  "<YOUR-PASSWD>"
#endif

const char* host = "esp8266-webupdate";
const char *ssid     = "SMART HOME";
const char *password = "rahasiadong";
const char* ihostname = "SWITCH LAMPU"; //Identitas Hostname Perangkat ini

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

void setup(void) {

  Serial.begin(115200);
  Serial.println();
  Serial.println("Booting Sketch...");
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    Serial.println("WiFi failed, retrying.");
  }

  MDNS.begin(host);

  // handler for the /update form page
  httpServer.on("/update", HTTP_GET, [&](){
//    if(username != emptyString && password != emptyString && !httpServer.authenticate(username.c_str(), password.c_str()))
//      return httpServer.requestAuthentication();
	  httpServer.sendHeader(F("Content-Encoding"), F("gzip"));
	  httpServer.send_P(200, "text/html", (const char*)index_color_html_gz, (int)index_color_html_gz_len);
  });

  httpUpdater.setup(&httpServer);
  httpServer.begin();

  MDNS.addService("http", "tcp", 80);
  Serial.printf("HTTPUpdateServer modified ready! Open http://%s.local/update in your browser\n", host);
}

void loop(void) {
  httpServer.handleClient();
  MDNS.update();
}
