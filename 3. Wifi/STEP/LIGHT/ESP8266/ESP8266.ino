#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "OchaHouse Tang 2";
const char *password = "ochahouse";

ESP8266WebServer server(80);

#define LIGHT_SENSOR A0

void setup() {
  Serial.begin(115200);

  // Kết nối đến WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, handleRoot);
  server.on("/lightValue", HTTP_GET, handleReadSensor);

  server.on("/setForward", HTTP_GET, handleSetForward);
  server.on("/setReverse", HTTP_GET, handleSetReverse);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  int lightValue = analogRead(LIGHT_SENSOR);
  String html = "<html><head><script>function updateValue() { var xhttp = new XMLHttpRequest(); xhttp.onreadystatechange = function() { if (this.readyState == 4 && this.status == 200) { document.getElementById('lightValue').innerHTML = this.responseText; } }; xhttp.open('GET', '/lightValue', true); xhttp.send(); } setInterval(updateValue, 1000);</script></head>";
  html += "<body><h1>Light Sensor Value</h1>";
  html += "<p>Value: <span id='lightValue'>" + String(lightValue) + "</span></p>";

  html += "<p>Enter the forward angle: <input type='text' id='forwardValue'></p>";
  html += "<button onclick='sendForwardValue()'>Move Forward</button>";

  html += "<p>Enter the reverse angle: <input type='text' id='reverseValue'></p>";
  html += "<button onclick='sendReverseValue()'>Move Reverse</button>";

  html += "</body><script>function sendForwardValue() { var forwardValue = document.getElementById('forwardValue').value; var xhttp = new XMLHttpRequest(); xhttp.open('GET', '/setForward?value=' + forwardValue, true); xhttp.send(); }</script>";
  html += "<script>function sendReverseValue() { var reverseValue = document.getElementById('reverseValue').value; var xhttp = new XMLHttpRequest(); xhttp.open('GET', '/setReverse?value=' + reverseValue, true); xhttp.send(); }</script></html>";
  server.send(200, "text/html", html);
}

void handleReadSensor() {
  int lightValue = analogRead(LIGHT_SENSOR);
  server.send(200, "text/plain", String(lightValue));
}

void handleSetForward() {
  String forwardValue = server.arg("value");
  Serial.println("t" + forwardValue);
  server.send(200, "text/plain", "Moved step to forward angle: " + forwardValue);
}

void handleSetReverse() {
  String reverseValue = server.arg("value");
  Serial.println("n" + reverseValue);
  server.send(200, "text/plain", "Moved step to reverse angle: " + reverseValue);
}
