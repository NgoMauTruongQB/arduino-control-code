#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "OchaHouse Tang 2";
const char *password = "ochahouse";

ESP8266WebServer server(80);

String positionValue = "GENTER"; // Change from float to String

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
  server.on("/getPosition", HTTP_GET, handleGetPosition);
  
  server.on("/setNhanhDan", HTTP_GET, handleSetNhanhDan);
  server.on("/setChamDan", HTTP_GET, handleSetChamDan);
  server.on("/setXuoi", HTTP_GET, handleSetXuoi);
  server.on("/setNguoc", HTTP_GET, handleSetNguoc);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  // Đọc dữ liệu từ Serial để nhận giá trị từ Arduino
  while (Serial.available()) {
    String data = Serial.readStringUntil('\n');

    if (data.startsWith("joy")) {
      // Giữ lại chỉ số từ "dist" đến hết chuỗi để lấy giá trị khoảng cách
      String distanceStr = data.substring(3);

      // Cập nhật biến positionValue với chuỗi
      positionValue = distanceStr;
    }
  }
}

void handleRoot() {
  String html = "<html><head><script>function updateValue() { var xhttp = new XMLHttpRequest(); xhttp.onreadystatechange = function() { if (this.readyState == 4 && this.status == 200) { document.getElementById('positionValue').innerHTML = this.responseText; } }; xhttp.open('GET', '/getPosition', true); xhttp.send(); } setInterval(updateValue, 1000);</script></head>";
  html += "<body><h1>Joystick Value</h1>";
  html += "<p>Position: <span id='positionValue'>" + positionValue + "</span></p>";

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

void handleGetPosition() {
  // Gửi giá trị khoảng cách về trang web
  server.send(200, "text/plain", positionValue);
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