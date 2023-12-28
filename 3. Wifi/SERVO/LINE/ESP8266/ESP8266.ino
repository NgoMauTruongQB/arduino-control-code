/* 
Dò line: A0
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "oad";
const char *password = "00001111";

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

  server.on("/setServo", HTTP_GET, handleSetServo); // Thêm đường dẫn để điều khiển servo

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
  html += "<p>Enter servo angle: <input type='text' id='servoValue'></p>";                      // Thêm ô input cho góc của servo
  html += "<button onclick='sendServoValue()'>Move Servo</button>";                              // Thêm nút gửi giá trị cho servo
  html += "</body><script>function sendServoValue() { var servoValue = document.getElementById('servoValue').value; var xhttp = new XMLHttpRequest(); xhttp.open('GET', '/setServo?value=' + servoValue, true); xhttp.send(); }</script></html>";
  server.send(200, "text/html", html);
}

void handleReadSensor() {
  int lineValue = analogRead(LINE_SENSOR); // Đọc giá trị từ cảm biến dò line
  int digitalLineValue = (lineValue > 500) ? 1 : 0; // Giả sử ngưỡng là 500, bạn có thể điều chỉnh ngưỡng này tùy thuộc vào cảm biến cụ thể của bạn
  server.send(200, "text/plain", String(digitalLineValue)); // Gửi giá trị số học 1 hoặc 0 về cho JavaScript cập nhật trang web
}

void handleSetServo() {
  String servoValue = server.arg("value");
  // Gửi lệnh điều khiển servo đến Arduino thông qua Serial
  Serial.println("servo " + servoValue);
  server.send(200, "text/plain", "Moved servo to angle: " + servoValue);
}
