
#include <WiFi.h>
#include <WebServer.h>

// Wi-Fi credentials 
const char* ssid = "*SSID*";
const char* password = "*Password*";

// Wi-Fi part taken from
// https://deepbluembedded.com/esp32-wifi-library-examples-tutorial-arduino/
// https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/soft-access-point-class.html

IPAddress local_IP(192,168,4,22);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);

WebServer server(80);
 
const int led1Pin = 2; // Builtin pin
 
const char html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Inputs</title>
  <style>
    body {
      font-family: sans-serif;
    }
    .container {
      display: flex;
      flex-direction: column;
      align-items: center;
    }
    .row {
      display: flex;
      flex-direction: row;
      align-items: center;
    }
    button {
      font-size: 1.5em;
      margin: 10px;
      padding: 10px;
      background-color: lightgray; /* Set the initial background color */
    }
    button:active {
      animation-duration: 0.7s;
      background-color: lime;
    }
  </style>
</head>
<body>
  <div class="container">
    <h1>EPIC Input Web Server</h1>
    <div class="row">
      <button onclick="passInput(0)">0</button>
      <button onclick="passInput(1)">1</button>
      <button onclick="passInput(2)">2</button>
      <button onclick="passInput(3)">3</button>
    </div>
  </div>
  <script>
    var button = document.getElementById('button');
    function passInput(x) {
      var xhr = new XMLHttpRequest();
      xhr.open('GET', '/led?num=' + x, true);
      xhr.send();
    }
  </script>
</body>
</html>
)rawliteral";
 
void handleRoot() {
  server.send(200, "text/html", html);
}

void handleLED() {
  uint8_t num = server.arg("num").toInt();

  Serial.write(num);

  server.send(200, "text/plain", "OK");
  
  //digitalWrite(led1Pin, ledState);
  //server.send(200, "text/plain", String(ledState));
}
 
void setup() {
  // Serial Things
  Serial.begin(115200);

  // WiFi Configuration & Init
  
  // STA Mode
  //WiFi.mode(WIFI_STA);
  //WiFi.begin(ssid, password);
  //while (WiFi.status() != WL_CONNECTED) {
  //  delay(100);
  //}

  // AP Mode

  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP("ESP32 SERVER");

  // Attach Server Handler Functions & Start WebServer
  server.on("/", handleRoot);
  server.on("/led", handleLED);
  server.begin();
}
 
void loop() {
  server.handleClient();
}
