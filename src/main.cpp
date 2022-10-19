// Web Server with SPIFFS
#define STRUCT_MAGIC 12345678

#include <Arduino.h>
#include <spiffs.h>
#include "ESPAsyncWebServer.h"

const char *ssid = "MaisonRay300";
const char *password = "CamilleEmilie";
const int led = 2;
const int capteurLuminosite = 34;
const char *version= "0.0";

/*
WIFI_OFF     WIFI_MODE_NULL
WIFI_STA     WIFI_MODE_STA
WIFI_AP      WIFI_MODE_AP
WIFI_AP_STA  WIFI_MODE_APSTA */

struct EEPROM_Data {
  long unsigned int magic;
  char WiFiMode[4];
  char ssid[32];
  char WiFiPassword[32];
  char OTApassword[32];
  char hostname[64];
  bool http_enable;
  bool rtsp_enable;
  unsigned short rtsp_port;
  };

//Initial Valeur stored in EEPROM
const struct EEPROM_Data INITIAL_VALUE={
                    STRUCT_MAGIC,
                    "STA",
                   "MaisonRay300",
                    "CamilleEmilie",
                    "123456",
                    "Esp32Cam",
                    false,
                   false,
                    554
                };

AsyncWebServer server(80);

void setup()
{
  //----------------------------------------------------Serial
  Serial.begin(115200);
  Serial.println("\n");

  //----------------------------------------------------GPIO
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  pinMode(capteurLuminosite, INPUT);

  //----------------------------------------------------SPIFFS
  if(!SPIFFS.begin())
  {
    Serial.println("Erreur SPIFFS...");
    return;
  }

  File root = SPIFFS.open("/");
  File file = root.openNextFile();

  while(file)
  {
    Serial.print("File: ");
    Serial.println(file.name());
    file.close();
    file = root.openNextFile();
  }

  //----------------------------------------------------WIFI
  WiFi.begin(ssid, password);
	Serial.print("Tentative de connexion...");
	
	while(WiFi.status() != WL_CONNECTED)
	{
		Serial.print(".");
		delay(100);
	}
	
	Serial.println("\n");
	Serial.println("Connexion etablie!");
	Serial.print("Adresse IP: ");
	Serial.println(WiFi.localIP());

  //----------------------------------------------------SERVER
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.on("/w3.css", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/w3.css", "text/css");
  });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/script.js", "text/javascript");
  });

  server.on("/lireLuminosite", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    int val = analogRead(capteurLuminosite);
    String luminosite = String(val);
    request->send(200, "text/plain", luminosite);
  });

  server.on("/restart", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    Serial.println("RESTART BUTTON");
    request->send(200);
  });

  server.on("/save", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    Serial.println("SAVE BUTTON");
    request->send(200);
  });
  
  server.on("/reinit", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    Serial.println("REINIT BUTTON");
    request->send(200);
  });

  server.begin();
  Serial.println("Serveur actif!");
}

void loop()
{

}
