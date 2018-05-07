#include <SPI.h>
#include <ESP8266WiFi.h>

char ssid[] = "your_network_SSID";      // change it!
char pass[] = "your_network_PASSWORD";  // change it!
const String myDevice = "myDeviceDefault@your_Username"; // change it!
char server[] = "api.favoriot.com";

int status = WL_IDLE_STATUS;
WiFiClient client;

void setup() {
  Serial.begin(115200);
  WiFi.disconnect();
  Serial.println("Connecting...");
  WiFi.begin(ssid,pass);
  
  while((!(WiFi.status() == WL_CONNECTED))){
    delay(300);
    Serial.print(".");
  }
  
  Serial.println(WiFi.status());
  Serial.println("Connected!");
  Serial.println("");
}

void loop() {
  String myData = String(analogRead(A0));
  String json = "{\"myDevice_developer_id\":\""+myDevice+"\",\"data\":{\"My Data\":\""+myData+"\"}}";
  Serial.println(json);
  if (client.connect(server, 80)) {
    client.println("POST /v1/streams HTTP/1.1");
    client.println("Host: api.favoriot.com");

    client.println(F("apikey: your_api_key"));  // change it!
    
    client.println("Content-Type: application/json");
    client.println("cache-control: no-cache");
    client.print("Content-Length: ");
    int thisLength = json.length();
    client.println(thisLength);
    client.println("Connection: close");
    client.println();
    client.println(json);
  }
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }
  if (!client.connected()) {
    client.stop();
  }
  delay(5000);
}