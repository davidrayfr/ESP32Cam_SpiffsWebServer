// Web Server with SPIFFS
#define STRUCT_MAGIC 12345678

#include <Arduino.h>
#include "SPIFFS.h"
#include "ESPAsyncWebServer.h"

const char *ssid = "MaisonRay300";
const char *password = "CamilleEmilie";
//const char *ssid = "Vatan2.4";
//const char *password = "Vascjbb5";

const int led = 2;
const int capteurLuminosite = 34;
const char *version= "0.1";

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

EEPROM_Data memory;

/*String processor(const String& var){
  Serial.println(var);
  if(var == "ADRESSEIP"){
    String val=String(WiFi.localIP());
    Serial.println(val);
    return val;
  }
  return String();
}*/

String XML_ConnectionData(void){
  String ch=String("<?xml version = \"1.0\" ?>")+String("<inputs>");
    ch=ch+String("<version>")+String(version)+String("</version>");
    ch=ch+String("<ipadresse>")+WiFi.localIP().toString()+String("</ipadresse>"); 
    ch=ch+String("<namessid>")+String(memory.ssid)+String("</ssid>"); 
    ch=ch+String("<hostname>")+String(memory.hostname)+String("</hostname>");
    if (memory.http_enable) 
      {ch=ch+String("<http_enable>")+"1"+String("</http_enable>");}
      else
      {ch=ch+String("<http_enable>")+"0"+String("</http_enable>");}
    if (memory.rtsp_enable)
      {ch=ch+String("<rtsp_enable>")+"1"+String("</rtsp_enable>");}
      else
      {ch=ch+String("<rtsp_enable>")+"0"+String("</rtsp_enable>");}
    ch=ch+String("<portrtsp>")+String(memory.rtsp_port)+String("</portrtsp>");
    ch=ch+String("</inputs>");
  return ch; 
};
void setup()
{
  //----------------------------------------------------Serial
  Serial.begin(115200);
  Serial.println("\n");
  memory=INITIAL_VALUE;
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
    request->send(SPIFFS, "/index.html","text/html");
  });
  server.on("/w3.css", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/w3.css", "text/css");
  });
  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/script.js", "text/javascript");
  }); 
  server.on("/jquery-3.6.1.min.js", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/jquery-3.6.1.min.js", "text/javascript");
  });
  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS,"/favicon.ico","image/ico");
  });
server.on("/Camera_img.png", HTTP_GET, [](AsyncWebServerRequest *request)
  {
  request->send(200,"/Camera_img.png","image/png");
  });

  server.on("/receivedData", HTTP_POST, [](AsyncWebServerRequest *request) {
    if(request->hasParam("nomssid", true))
    {
      String message;
      message = request->getParam("nomssid", true)->value();
      Serial.print("Wifi Nom reçu :");
      Serial.println(message);
    }
    request->send(204);
  });

  server.on("/envoid1", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    String val = String(WiFi.localIP());
    request->send(200, "text/plain", WiFi.localIP().toString());
    Serial.println("envoid1");
    });

//Envoie les data de la connection en XML
//via la fonction chaine
  server.on("/sendData", HTTP_GET, [](AsyncWebServerRequest *request)
  {
  request->send(200,"text/xml",XML_ConnectionData());
  Serial.println("envoiData");
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
