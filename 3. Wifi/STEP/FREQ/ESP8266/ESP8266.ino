#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "OchaHouse Tang 2";
const char *password = "ochahouse";

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

  server.on("/setForward", HTTP_GET, handleSetForward);
  server.on("/setReverse", HTTP_GET, handleSetReverse);

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

  html += "<p>Enter the forward angle: <input type='text' id='forwardValue'></p>";
  html += "<button onclick='sendForwardValue()'>Move Forward</button>";

  html += "<p>Enter the reverse angle: <input type='text' id='reverseValue'></p>";
  html += "<button onclick='sendReverseValue()'>Move Reverse</button>";

  html += "</body><script>function sendForwardValue() { var forwardValue = document.getElementById('forwardValue').value; var xhttp = new XMLHttpRequest(); xhttp.open('GET', '/setForward?value=' + forwardValue, true); xhttp.send(); }</script>";
  html += "<script>function sendReverseValue() { var reverseValue = document.getElementById('reverseValue').value; var xhttp = new XMLHttpRequest(); xhttp.open('GET', '/setReverse?value=' + reverseValue, true); xhttp.send(); }</script></html>";
  server.send(200, "text/html", html);
}

void handleGetDistance() {
  // Gửi giá trị khoảng cách về trang web
  server.send(200, "text/plain", String(distanceValue));
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
