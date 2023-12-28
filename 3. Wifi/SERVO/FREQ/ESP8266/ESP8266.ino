#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "oad";
const char *password = "00001111";

ESP8266WebServer server(80);

float distanceValue = 0.0; // Giá trị khoảng cách mặc định

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
  server.on("/getDistance", HTTP_GET, handleGetDistance); // Thêm đường dẫn để nhận giá trị khoảng cách
  server.on("/setServo", HTTP_GET, handleSetServo); // Thêm đường dẫn để điều khiển servo từ web

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  // Đọc dữ liệu từ Serial để nhận giá trị từ Arduino
  while (Serial.available()) {
    String data = Serial.readStringUntil('\n');
    
    if (data.startsWith("dist")) {
      // Giữ lại chỉ số từ "dist" đến hết chuỗi để lấy giá trị khoảng cách
      String distanceStr = data.substring(4);
      
      // Chuyển giá trị khoảng cách thành số thực và cập nhật biến distanceValue
      distanceValue = distanceStr.toFloat();
    }
  }
}

void handleRoot() {
  String html = "<html><head><script>function updateValue() { var xhttp = new XMLHttpRequest(); xhttp.onreadystatechange = function() { if (this.readyState == 4 && this.status == 200) { document.getElementById('distanceValue').innerHTML = this.responseText; } }; xhttp.open('GET', '/getDistance', true); xhttp.send(); } setInterval(updateValue, 1000);</script></head>";
  html += "<body><h1>Distance Sensor Value</h1>";
  html += "<p>Distance: <span id='distanceValue'>" + String(distanceValue) + " cm</span></p>"; // Hiển thị giá trị khoảng cách lên trang web
  html += "<p>Enter servo angle: <input type='text' id='servoValue'></p>";                      // Thêm ô input cho góc của servo
  html += "<button onclick='sendServoValue()'>Move Servo</button>";                              // Thêm nút gửi giá trị cho servo
  html += "</body><script>function sendServoValue() { var servoValue = document.getElementById('servoValue').value; var xhttp = new XMLHttpRequest(); xhttp.open('GET', '/setServo?value=' + servoValue, true); xhttp.send(); }</script></html>";
  server.send(200, "text/html", html);
}

void handleGetDistance() {
  // Gửi giá trị khoảng cách về trang web
  server.send(200, "text/plain", String(distanceValue));
}

void handleSetServo() {
  String servoValue = server.arg("value");
  // Gửi lệnh điều khiển servo đến Arduino thông qua Serial
  Serial.println("servo " + servoValue);
  server.send(200, "text/plain", "Moved servo to angle: " + servoValue);
}
