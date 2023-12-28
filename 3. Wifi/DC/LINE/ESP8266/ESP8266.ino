/* 
Dò line: A0
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "OchaHouse Tang 2";
const char *password = "ochahouse";

ESP8266WebServer server(80);

#define LINE_SENSOR A0

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
  server.on("/lineValue", HTTP_GET, handleReadSensor); // Đổi tên đường dẫn thành /lineValue

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
  int lineValue = analogRead(LINE_SENSOR); // Đọc giá trị từ cảm biến dò line
  int digitalLineValue = (lineValue > 500) ? 1 : 0; // Giả sử ngưỡng là 500, bạn có thể điều chỉnh ngưỡng này tùy thuộc vào cảm biến cụ thể của bạn
  String html = "<html><head><script>function updateValue() { var xhttp = new XMLHttpRequest(); xhttp.onreadystatechange = function() { if (this.readyState == 4 && this.status == 200) { document.getElementById('lineValue').innerHTML = this.responseText; } }; xhttp.open('GET', '/lineValue', true); xhttp.send(); } setInterval(updateValue, 1000);</script></head>";
  html += "<body><h1>Line Sensor Value</h1>";
  html += "<p>Digital Value: <span id='lineValue'>" + String(digitalLineValue) + "</span></p>"; // Hiển thị giá trị 1 hoặc 0 lên trang web

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
  int lineValue = analogRead(LINE_SENSOR); // Đọc giá trị từ cảm biến dò line
  int digitalLineValue = (lineValue > 500) ? 1 : 0; // Giả sử ngưỡng là 500, bạn có thể điều chỉnh ngưỡng này tùy thuộc vào cảm biến cụ thể của bạn
  server.send(200, "text/plain", String(digitalLineValue)); // Gửi giá trị số học 1 hoặc 0 về cho JavaScript cập nhật trang web
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
