#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

const char *ssid = "oad";
const char *password = "00001111";

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
  server.on("/setServo", HTTP_GET, handleSetServo);

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
  html += "<p>Enter servo angle: <input type='text' id='servoValue'></p>";                     // Thêm ô input cho góc của servo
  html += "<button onclick='sendServoValue()'>Move Servo</button>";                             // Thêm nút gửi giá trị cho servo
  html += "</body><script>function sendServoValue() { var servoValue = document.getElementById('servoValue').value; var xhttp = new XMLHttpRequest(); xhttp.open('GET', '/setServo?value=' + servoValue, true); xhttp.send(); }</script></html>";
  server.send(200, "text/html", html);
}

void handleReadSensor() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  String sensorValues = String(temperature) + "°C, " + String(humidity) + "% ";
  server.send(200, "text/plain", sensorValues); // Gửi giá trị về cho JavaScript cập nhật trang web
}

void handleSetServo() {
  String servoValue = server.arg("value");
  // Gửi lệnh điều khiển servo đến Arduino thông qua Serial
  Serial.println("servo " + servoValue);
  server.send(200, "text/plain", "Moved servo to angle: " + servoValue);
}
