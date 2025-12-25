/*
 * created by: abhishekkr (https://abhishekkr.github.io)
 *
 * This sample helps test your Boards WiFi connectivity.
 * On booting the board would try connect to given WiFi SSID.
 * The make HTTP request configured IP:PORT.
 * Then does it on a loop with delay...
 * while checking WiFi connectivity & retires if not connected.
 *
 */
#include <NetworkClientSecure.h>
#include <WiFi.h>


const char *ssid = "WIFI_NAME_MACHINES_CONNECT_TO";         // your network SSID (name of wifi network)
const char *password = "PASSWORD_USED_TO_CONNECT_TO_WIFI";  // your network password

// Initialize the client library

WiFiClient client;
const IPAddress server = IPAddress(192, 168, 4, 2);   // IP Address to Connect to
const int port = 8080;                                // Server's Port
const char *httpReqStr = "GET /check HTTP/1.0";       // HTTP Request String


void setup() {
  Serial.begin(115200);  // YOU SELECT THIS IN SERIAL MONITOR TO CHECK OUTPUT

  delay(1000);

  connectWiFi();

  wifiStatus();
}

void loop() {
    wifiStatus();
    delay(15000);
}

void connectWiFi(){
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi Connected: ");
    Serial.println(ssid);
    return;
  }
  Serial.print("WiFi Connection Test: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  int connectionAttempt = 0;
  while (WiFi.status() != WL_CONNECTED && connectionAttempt < 10) {
    Serial.print(" :( ");
    connectionAttempt++;
    delay(2000);
    WiFi.begin(ssid, password);
  }
}

void wifiStatus() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Connected to: ");
    Serial.println(ssid);
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    httpClient();
  } else {
    Serial.print("WIFI not connected: ");
    Serial.println(ssid);
    Serial.println("Attempting Reconnect...");
    connectWiFi();
  }
}

void httpClient() {
  Serial.println("\nStarting connection to server...");
  if (!client.connect(server, port)) {
    Serial.println("Connection failed!");
    return;
  }
  Serial.println("Connected to server!");

  client.println(httpReqStr);
  client.print("Host: ");
  client.println(server);
  client.println("Connection: close");
  client.println();

  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }

  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  client.stop();
}
