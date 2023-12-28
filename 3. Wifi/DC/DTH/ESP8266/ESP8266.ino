#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

const char *ssid = "OchaHouse Tang 2";
const char *password = "ochahouse";

ESP8266WebServer server(80);

#define DHT_PIN 2 // GPIO pin mà cảm biến DHT11 được kết nối đến (ví dụ: D4)
#define DHT_TYPE DHT11 // Loại cảm biến (DHT11 hoặc DHT22)

DHT dht(DHT_PIN, DHT_TYPE);

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

  server.on("/setNhanhDan", HTTP_GET, handleSetNhanhDan);
  server.on("/setChamDan", HTTP_GET, handleSetChamDan);
  server.on("/setXuoi", HTTP_GET, handleSetXuoi);
  server.on("/setNguoc", HTTP_GET, handleSetNguoc);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  String html = "<html><head><script>function updateValue() { var xhttp = new XMLHttpRequest(); xhttp.onreadystatechange = function() { if (this.readyState == 4 && this.status == 200) { document.getElementById('sensorValue').innerHTML = this.responseText; } }; xhttp.open('GET', '/sensorValue', true); xhttp.send(); } setInterval(updateValue, 1000);</script></head>";
  html += "<body><h1>DHT11 Sensor Values</h1>";
  html += "<p>Temperature, Humidity: <span id='sensorValue'>" + String(temperature) + "°C, " + String(humidity) + "%</span></p>"; // Hiển thị giá trị nhiệt độ lên trang web

  html += "<button onclick='sendNhanhDan()'>Nhanh dan</button>";
  html += "<button onclick='sendChamDan()'>Cham dan</button>";
  html += "<button onclick='sendXuoi()'>Quay xuoi</button>";
  html += "<button onclick='sendNguoc()'>Quay nguoc</button>";

  html += "</body><script>function sendNhanhDan() { var xhttp = new XMLHttpRequest(); xhttp.open('GET', '/setNhanhDan', true); xhttp.send(); }</script>";
  html += "<script>function sendChamDan() { var xhttp = new XMLHttpRequest(); xhttp.open('GET', '/setChamDan', true); xhttp.send(); }</script>";
  html += "<script>function sendXuoi() { var xhttp = new XMLHttpRequest(); xhttp.open('GET', '/setXuoi', true); xhttp.send(); }</script>";
  html += "<script>function sendNguoc() { var xhttp = new XMLHttpRequest(); xhttp.open('GET', '/setNguoc', true); xhttp.send(); }</script></html>";

  server.send(200, "text/html", html);
}

void handleReadSensor() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  String sensorValues = String(temperature) + "°C, " + String(humidity) + "% ";
  server.send(200, "text/plain", sensorValues); // Gửi giá trị về cho JavaScript cập nhật trang web
}

void handleSetNhanhDan() {
  // Xử lý điều khiển động cơ nhanh dần ở đây
  Serial.println("nhanh");
  server.send(200, "text/plain", "Nhanh Dan");
}

void handleSetChamDan() {
  // Xử lý điều khiển động cơ chậm dần ở đây
  Serial.println("cham");
  server.send(200, "text/plain", "Cham Dan");
}

void handleSetXuoi() {
  // Xử lý điều khiển động cơ quay xuôi ở đây
  Serial.println("thuan");
  server.send(200, "text/plain", "Quay Xuoi");
}

void handleSetNguoc() {
  // Xử lý điều khiển động cơ quay ngược ở đây
  Serial.println("nghich");
  server.send(200, "text/plain", "Quay Nguoc");
}

