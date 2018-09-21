#include <DFRobot_sim808.h>
#include <WiFi.h>
#include <DFMiniMp3.h>
#include <SoftwareSerial.h>

#define PHONE_NUMBER "8086723443"
#define PHONE_NUMBER1 "8891820794"
#define PHONE_NUMBER2 "8129126261"
#define PHONE_NUMBER3 "9544486561"
char* MESSAGE1 = "ALERT FROM WOMEN'S SAFETY DEVICE; LOCATION IN THE NEXT MESSAGE";
char MESSAGE2[20] = "";
char MESSAGE3[20] = "";

#define led1 27

float lat = 0;
float lon = 0;

#define ssid "safe_women"
#define password "098safety098"
#define host "192.168.4.1"
const int httpPort = 80;
int value = 0;

class Mp3Notify
{
public:
  static void OnError(uint16_t errorCode)
  {
    // see DfMp3_Error for code meaning
    Serial.println();
    Serial.print("Com Error ");
    Serial.println(errorCode);
  }

  static void OnPlayFinished(uint16_t globalTrack)
  {
    Serial.println();
    Serial.print("Play finished for #");
    Serial.println(globalTrack);   
  }

  static void OnCardOnline(uint16_t code)
  {
    Serial.println();
    Serial.print("Card online ");
    Serial.println(code);     
  }

  static void OnCardInserted(uint16_t code)
  {
    Serial.println();
    Serial.print("Card inserted ");
    Serial.println(code); 
  }

  static void OnCardRemoved(uint16_t code)
  {
    Serial.println();
    Serial.print("Card removed ");
    Serial.println(code);  
  }
};

HardwareSerial Serial1(1);
HardwareSerial Serial2(2); 
DFRobot_SIM808 sim808(&Serial1);
DFMiniMp3<HardwareSerial, Mp3Notify> mp3(Serial2);

void setup() 
{
  Serial1.begin(9600,SERIAL_8N1,26,22);
  Serial.begin(115200);
  delay(100);
  pinMode(led1, OUTPUT);
  Serial.println("Starting DFplayer");
  mp3.begin();
  uint16_t volume = mp3.getVolume();
  Serial.print("volume ");
  Serial.println(volume);
  mp3.setVolume(30);
  uint16_t count = mp3.getTotalTrackCount();
  Serial.print("files ");
  Serial.println(count);
  
  while(!sim808.init()) 
  { 
      delay(1000);
      Serial.print("Sim808 init error\r\n");
  }
  Serial.print("connecting to WiFi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
      delay(100);
      Serial.print(".");
  }
  Serial.println(" ");
  Serial.println("WiFi connected");
  digitalWrite(led1, HIGH);
  delay(250);
  digitalWrite(led1, LOW);
  delay(100);
  digitalWrite(led1, HIGH);
  delay(250);
  digitalWrite(led1, LOW);
  
}

void waitMilliseconds(uint16_t msWait)
{
  uint32_t start = millis();
  
  while ((millis() - start) < msWait)
  {
    // calling mp3.loop() periodically allows for notifications 
    // to be handled without interrupts
    mp3.loop(); 
    delay(1);
  }
}

void loop() 
{
    delay(1000);
    WiFiClient client;
    if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        Serial.print("Reconnecting to WiFi");
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED) 
        {
          delay(100);
          Serial.print(".");
        }
        return;
    }
    String url = "/check";
    
    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 5000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
        }
    }
    while(client.available()) 
    {
        String line = client.readStringUntil('\r');
        if(line.endsWith("2") || line.endsWith("3"))
        {
          Serial.print(line);
          ++value;
        }
        else if(line.endsWith("1"))
        {
          Serial.print(line);
          digitalWrite(led1, HIGH);
          delay(250);
          digitalWrite(led1, LOW);
        }
        
    }
    if(value > 5)
    {
      sendsms();
      value = 0;
    }
}

void sendsms()
{
  /*
  if(sim808.attachGPS())
  {
  Serial.println("Open the GPS power success");
  }
    sim808.getGPS();
  */
   
    sim808.sendSMS(PHONE_NUMBER,MESSAGE1);
    sim808.sendSMS(PHONE_NUMBER1,MESSAGE1);
    sim808.sendSMS(PHONE_NUMBER2,MESSAGE1);
    //sim808.sendSMS(PHONE_NUMBER3,MESSAGE1);
    
    //lat = sim808.GPSdata.lat,6;
    lat = 8.5059198;
    dtostrf(lat, 20, 6, MESSAGE2);
    MESSAGE2[10] = 'N';
    sim808.sendSMS(PHONE_NUMBER,MESSAGE2);
    sim808.sendSMS(PHONE_NUMBER1,MESSAGE2);
    sim808.sendSMS(PHONE_NUMBER2,MESSAGE2);
    //sim808.sendSMS(PHONE_NUMBER3,MESSAGE2);
    
    //lon = sim808.GPSdata.lon,6;
    lon = 76.9386909;
    dtostrf(lon, 20, 6, MESSAGE3);
    MESSAGE3[10] = 'E';
    sim808.sendSMS(PHONE_NUMBER,MESSAGE3);
    sim808.sendSMS(PHONE_NUMBER1,MESSAGE3);
    sim808.sendSMS(PHONE_NUMBER2,MESSAGE3);
    //sim808.sendSMS(PHONE_NUMBER3,MESSAGE3);
    
    Serial.println("Message send");
    //sim808.detachGPS();
    play1();
}
void play1()
{
  mp3.playMp3FolderTrack(1);  // sd:/mp3/0001.mp3
  waitMilliseconds(30000);
}


