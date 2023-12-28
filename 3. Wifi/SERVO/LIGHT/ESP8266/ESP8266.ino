/* 
Cảm biến ánh sáng: A0
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "Nac danh";
const char *password = "kimhieu290103";

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

  server.on("/setServo", HTTP_GET, handleSetServo); // Thêm đường dẫn để điều khiển servo

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  int lightValue = analogRead(LIGHT_SENSOR); // Đọc giá trị từ cảm biến ánh sáng
  String html = "<html><head><script>function updateValue() { var xhttp = new XMLHttpRequest(); xhttp.onreadystatechange = function() { if (this.readyState == 4 && this.status == 200) { document.getElementById('lightValue').innerHTML = this.responseText; } }; xhttp.open('GET', '/lightValue', true); xhttp.send(); } setInterval(updateValue, 1000);</script></head>";
  html += "<body><h1>Light Sensor Value</h1>";
  html += "<p>Value: <span id='lightValue'>" + String(lightValue) + "</span></p>"; // Hiển thị giá trị lên trang web
  html += "<p>Enter servo angle: <input type='text' id='servoValue'></p>";          // Thêm ô input cho góc của servo
  html += "<button onclick='sendServoValue()'>Move Servo</button>";                  // Thêm nút gửi giá trị cho servo
  html += "</body><script>function sendValue() { var value = document.getElementById('inputValue').value; var xhttp = new XMLHttpRequest(); xhttp.open('GET', '/setLightValue?value=' + value, true); xhttp.send(); }</script>";
  html += "<script>function sendServoValue() { var servoValue = document.getElementById('servoValue').value; var xhttp = new XMLHttpRequest(); xhttp.open('GET', '/setServo?value=' + servoValue, true); xhttp.send(); }</script></html>";
  server.send(200, "text/html", html);
}

void handleSetLightValue() {
  String value = server.arg("value");
  Serial.println(value);
  server.send(200, "text/plain", "Received value: " + value);
}

void handleReadSensor() {
  int lightValue = analogRead(LIGHT_SENSOR); // Đọc giá trị từ cảm biến ánh sáng
  server.send(200, "text/plain", String(lightValue)); // Gửi giá trị về cho JavaScript cập nhật trang web
}


void handleSetServo() {
  String servoValue = server.arg("value");
  // Gửi lệnh điều khiển servo đến Arduino thông qua Serial
  Serial.println("servo " + servoValue);
  server.send(200, "text/plain", "Moved servo to angle: " + servoValue);
}
