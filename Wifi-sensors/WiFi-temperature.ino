#include <ESP8266WiFi.h>

// valeurs pour le WiFi
const char* ssid     = "";        // Nom du réseau
const char* password = "";        // clef 

// valeurs pour le serveur Web
const char* host     = "api-iot.croak.fr";

void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.print("Connexion au WiFi ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);    // On se connecte
  
  while (WiFi.status() != WL_CONNECTED) { // On attend
    delay(500);
    Serial.print(".");
  }

  Serial.println("");  // on affiche les paramètres 
  Serial.println("WiFi connecté");  
  Serial.print("Adresse IP du module EPC: ");  
  Serial.println(WiFi.localIP());
  Serial.print("Adresse IP de la box : ");
  Serial.println(WiFi.gatewayIP());
}


void loop() {
  float temperature = 32.3;

  Serial.print("Connexion au serveur : ");
  Serial.println(host);
  
  // On se place dans le rôle du  client en utilisant WifiClient
  WiFiClient client;

  // le serveur Web attend tradionnellement sur le port 80
  const int httpPort = 80;

  // Si la connexion échoue ca sera pour la prochaine fois
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  
  // La connexion a réussie on forme le chemin 
  // URL  complexe composé du chemin et de deux 
  // questions contenant le nom de ville et l'API key
  
  String get_url = String("/src/index.php/profile/0000");
  String put_url = String("/src/index.php/add/0000");
  Serial.println("**********************************************");
  Serial.print("get with URL: ");
  Serial.println(get_url);
  
  client.println(String("GET ") + get_url + " HTTP/1.1");
  client.println(String("Host: ") + host);
  client.println("Connection: Keep-Alive");
  client.println();

  // On attend 1 seconde
  delay(1000);
  
  // On lit les données reçues, s'il y en a
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.println (line);

  } /* fin data avalaible */


  Serial.println("//////////////////////////////////////////////////////");
  Serial.print("put with URL: ");
  Serial.println(put_url);

  client.println(String("PUT ") + put_url + " HTTP/1.1");
  client.println(String("Host: ") + host);
  client.println("Connection: close");
  client.println("Content-Type: application/x-www-form-urlencoded");
  client.println("Content-Length: 15");
  client.println();
  client.println("{\"temp\":33.5}");

  // On attend 1 seconde
  delay(1000);
  
  // On lit les données reçues, s'il y en a
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.println (line);

  } /* fin data avalaible */

  Serial.println("connexion closed");
  // On attend 10 secondes
  delay(10000);
}
