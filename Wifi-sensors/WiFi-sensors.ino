#include <ESP8266WiFi.h>

//SN of device
const char* sn= "0000";

//WiFi data
const char* ssid     = "";        // network name
const char* password = "";        // wifi key

//web server to share data, and conifguration of http port 80
const char* host     = "api-iot.croak.fr";
const int httpPort = 80;

//http adress to PUT data
const String url = String("/src/index.php/devices/")+sn;

//setup manages wifi connection
void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network
  Serial.print("Connection to WiFi ");
  Serial.println(ssid);  
  WiFi.begin(ssid, password);
  
  //waiting for connection
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connection ok");  
  Serial.print("IP adress of device: ");  
  Serial.println(WiFi.localIP());
  Serial.print("IP adress of gateway : ");
  Serial.println(WiFi.gatewayIP());
}


void loop() {

  //test if new data are available from device. 
  //If not, wait for a delay before a new tentative
  if (!deviceDataAvailable()) {
      delay(10000);
      return;
  }

  //new data are availbale, get sensor values 
  //and build the JSON to send via the http request  
  String json = makeJSONsentence(getSensorValues());

  Serial.print("Connection to web server: ");
  Serial.println(host);
  
  //connection to web server
  WiFiClient client;  
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  Serial.print("put with URL: ");
  Serial.println(url);

  //connection ok, data wil be sent via a http request 
  //using a specific route (PUT request)
  client.println(String("PUT ") + url + " HTTP/1.1");
  client.println(String("Host: ") + host);
  client.println("Connection: close");
  client.println("Content-Type: application/x-www-form-urlencoded");
  client.println("Content-Length: 15");
  client.println();
  client.println(json);

  //Wait for a http response
  delay(1000);
  
  //if data are received from web server, data are displayed using Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.println (line);
  }

  Serial.println("connexion closed");
  
  //Wait for 10 secondes before a new connection
  delay(10000);
}