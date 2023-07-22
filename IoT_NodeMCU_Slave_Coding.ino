#include "WiFi.h"
#include <ThingSpeak.h>

#define RXp2 16
#define TXp2 17


WiFiClient client;

const char* ssid = "Hotspot"; //Enter SSID
const char* password = "12345678"; //Enter Password

unsigned long myChannelNumber = 2073199; //Your Channel Number (Without Brackets)
const char * myWriteAPIKey = "RBGOIGPNYQDA0VVF"; //Your Write API Key

String a;
float dat = 0.0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
     delay(500);
     Serial.print("*");
  }
  ThingSpeak.begin(client);
  Serial.println("");
  Serial.println("WiFi connection Successful");
  Serial.print("The IP Address of ESP32 Module is: ");
  Serial.println(WiFi.localIP());// Print the IP address
}
void loop() {
    
    if(Serial2.available() > 0) {
    a = Serial2.readString();
    Serial.println("Value Fetched");
    dat = a.toFloat();
    Serial.println(dat);    
    }
    ThingSpeak.writeField(myChannelNumber, 1,dat, myWriteAPIKey); //Update in ThingSpeak  
    delay(10000);  
}
