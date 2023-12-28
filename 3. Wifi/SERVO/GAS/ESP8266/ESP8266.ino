#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "Nac danh";
const char *password = "kimhieu290103";

ESP8266WebServer server(80);

#define GAS_SENSOR A0

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
  server.on("/gasValue", HTTP_GET, handleReadSensor); // Đổi tên đường dẫn thành /gasValue

  server.on("/setServo", HTTP_GET, handleSetServo); // Thêm đường dẫn để điều khiển servo

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  int gasValue = analogRead(GAS_SENSOR); // Đọc giá trị từ cảm biến khí ga (sử dụng giao tiếp analog)
  String html = "<html><head><script>function updateValue() { var xhttp = new XMLHttpRequest(); xhttp.onreadystatechange = function() { if (this.readyState == 4 && this.status == 200) { document.getElementById('gasValue').innerHTML = this.responseText; } }; xhttp.open('GET', '/gasValue', true); xhttp.send(); } setInterval(updateValue, 1000);</script></head>";
  html += "<body><h1>Gas Sensor Value</h1>";
  html += "<p>Digital Value: <span id='gasValue'>" + String(gasValue) + "</span></p>"; // Hiển thị giá trị số học 1 hoặc 0 lên trang web
  html += "<p>Enter servo angle: <input type='text' id='servoValue'></p>";                      // Thêm ô input cho góc của servo
  html += "<button onclick='sendServoValue()'>Move Servo</button>";                              // Thêm nút gửi giá trị cho servo
  html += "</body><script>function sendServoValue() { var servoValue = document.getElementById('servoValue').value; var xhttp = new XMLHttpRequest(); xhttp.open('GET', '/setServo?value=' + servoValue, true); xhttp.send(); }</script></html>";
  server.send(200, "text/html", html);
}

void handleReadSensor() {
  int gasValue = analogRead(GAS_SENSOR); // Đọc giá trị từ cảm biến khí ga (sử dụng giao tiếp analog)
  server.send(200, "text/plain", String(gasValue)); // Gửi giá trị số học 1 hoặc 0 về cho JavaScript cập nhật trang web
}

void handleSetServo() {
  String servoValue = server.arg("value");
  // Gửi lệnh điều khiển servo đến Arduino thông qua Serial
  Serial.println("servo " + servoValue);
  server.send(200, "text/plain", "Moved servo to angle: " + servoValue);
}
