#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include "FPS_GT511C3.h"
#include "SoftwareSerial.h"

/* Set these to your desired credentials. */
const char *ssid = "safe_women";
const char *password = "098safety098";

int val = 0;
#define led1 4
#define led2 5

ESP8266WebServer server(80);
FPS_GT511C3 fps(13, 15);

void handleRoot() 
{
	if (fps.IsPressFinger())
  {
    fps.CaptureFinger(false);
    int id = fps.Identify1_N();
    if (id <200)
    {
      digitalWrite(led1, HIGH);
      delay(250);
      digitalWrite(led1, LOW);
      val = 1;
    }
    else
    {  
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      delay(250);
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      val = 2;
    }
  }
  else
  {
    digitalWrite(led2, HIGH);
    delay(250);
    digitalWrite(led2, LOW);
    val = 3;
  }
	server.send(200, "text/html", String(val));
}

void setup() {
	delay(1000);
	Serial.begin(115200);
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
	Serial.println();
	Serial.print("Configuring access point...");
	WiFi.softAP(ssid, password);

	IPAddress myIP = WiFi.softAPIP();
	Serial.print("AP IP address: ");
	Serial.println(myIP);
	server.on("/check", handleRoot);
	server.begin();
	Serial.println("HTTP server started");

  fps.Open();
  fps.SetLED(true);

  digitalWrite(led1, HIGH);
  delay(250);
  digitalWrite(led1, LOW);
  digitalWrite(led2, HIGH);
  delay(250);
  digitalWrite(led2, LOW);
  
}

void loop() {
	server.handleClient();
}
