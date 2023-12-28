// Them thu vien
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "DHT.h"

// Pin
#define DHTPIN 5 
//chan D1 tren esp8266

// Su dung cam bien DHT11 
#define DHTTYPE DHT11

// Thiet lap DHT
DHT dht(DHTPIN, DHTTYPE, 15);

// Thong so WiFi nha ban
const char *ssid = "OchaHouse Tang 2";
const char *password = "ochahouse";

// Tao server
ESP8266WebServer server(80);

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

  dht.begin();

  server.on("/", HTTP_GET, handleRoot);
  server.on("/sensorValue", HTTP_GET, handleReadSensor);

  server.on("/setForward", HTTP_GET, handleSetForward);
  server.on("/setReverse", HTTP_GET, handleSetReverse);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  // Doc do am
  float humidity = dht.readHumidity();
  // Doc nhiet do o do C
  float temperature = dht.readTemperature();


  String html = "<html><head><script>function updateValue() { var xhttp = new XMLHttpRequest(); xhttp.onreadystatechange = function() { if (this.readyState == 4 && this.status == 200) { document.getElementById('sensorValue').innerHTML = this.responseText; } }; xhttp.open('GET', '/sensorValue', true); xhttp.send(); } setInterval(updateValue, 1000);</script></head>";
  html += "<body><h1>DHT11 Sensor Values</h1>";
  html += "<p>Temperature, Humidity: <span id='sensorValue'>" + String(temperature) + "°C, " + String(humidity) + "%</span></p>"; // Hiển thị giá trị nhiệt độ lên trang web

  html += "<p>Enter the forward angle: <input type='text' id='forwardValue'></p>";
  html += "<button onclick='sendForwardValue()'>Move Forward</button>";

  html += "<p>Enter the reverse angle: <input type='text' id='reverseValue'></p>";
  html += "<button onclick='sendReverseValue()'>Move Reverse</button>";

  html += "</body><script>function sendForwardValue() { var forwardValue = document.getElementById('forwardValue').value; var xhttp = new XMLHttpRequest(); xhttp.open('GET', '/setForward?value=' + forwardValue, true); xhttp.send(); }</script>";
  html += "<script>function sendReverseValue() { var reverseValue = document.getElementById('reverseValue').value; var xhttp = new XMLHttpRequest(); xhttp.open('GET', '/setReverse?value=' + reverseValue, true); xhttp.send(); }</script></html>";
  server.send(200, "text/html", html);
}

void handleReadSensor() {
  // Doc do am
  float humidity = dht.readHumidity();
  // Doc nhiet do o do C
  float temperature = dht.readTemperature();

  String sensorValues = String(temperature) + "°C, " + String(humidity) + "% ";
  server.send(200, "text/plain", sensorValues); // Gửi giá trị về cho JavaScript cập nhật trang web
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
