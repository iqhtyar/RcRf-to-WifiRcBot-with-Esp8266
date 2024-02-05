#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "iqhtyar";
const char* password = "12345678";

const int motorPin1 = D1;
const int motorPin2 = D2;
const int motorPin3 = D3;
const int motorPin4 = D4;
const int motorSpeedPin1 = D5;
const int motorSpeedPin2 = D6;

AsyncWebServer server(80);
// Kode HTML untuk halaman web kendali motor
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Kendali Motor</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      text-align: center;
    }
    .button {
      display: inline-block;
      margin: 10px;
      padding: 10px 20px;
      font-size: 16px;
      cursor: pointer;
      border-radius: 5px;
      background-color: #3498db;
      color: white;
      text-decoration: none;
    }
    .button:hover {
      background-color: #2980b9;
    }
    .slider-container {
      margin: 20px auto;
      width: 200px;
    }
    .slider {
      -webkit-appearance: none;
      width: 100%;
      height: 15px;
      border-radius: 10px;
      background: #d3d3d3;
      outline: none;
      opacity: 0.7;
      -webkit-transition: .2s;
      transition: opacity .2s;
    }
    .slider::-webkit-slider-thumb {
      -webkit-appearance: none;
      appearance: none;
      width: 25px;
      height: 25px;
      border-radius: 50%;
      background: #4caf50;
      cursor: pointer;
    }
    .slider::-moz-range-thumb {
      width: 25px;
      height: 25px;
      border-radius: 50%;
      background: #4caf50;
      cursor: pointer;
    }
  </style>
</head>
<body>
  <h1>Kendali Motor</h1>
  <div>
    <button class="button" onclick="controlMotor('left')">Kiri</button>
    <button class="button" onclick="controlMotor('right')">Kanan</button>
  </div>
  <div>
    <button class="button" onclick="controlMotor('forward')">Maju</button>
    <button class="button" onclick="controlMotor('reverse')">Mundur</button>
  </div>
  <div>
    <button class="button"
onclick="controlMotor('stop')">Stop</button>
  </div>
  <div class="slider-container">
    <input type="range" min="0" max="255" value="127" class="slider" id="speedSlider">
  </div>
  <script>
    function controlMotor(command) {
      var speed = document.getElementById("speedSlider").value;
      fetch('/motor', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/x-www-form-urlencoded'
        },
        body: `command=${command}&speed=${speed}`
      })
      .then(response => {
        console.log(response);
      })
      .catch(error => {
        console.error('Error:', error);
      });
    }
  </script>
  </script>
    <p>Mau tanya apa d^_^b:</p>
    <input id="input" type="text" placeholder="Type here">
    <button onclick="askQuestion()">Ask</button>
    <p id="response"></p>
</script>  
    <script>
        // Load data from localStorage
        var storedAnswers = JSON.parse(localStorage.getItem("chatbotAnswers")) || {};
        function saveToLocalStorage() {
            localStorage.setItem("chatbotAnswers", JSON.stringify(storedAnswers));
        }
        function askQuestion() {
            // get the question from the user
            var question = document.getElementById("input").value;
            // your extended AI logic goes here
            if (storedAnswers.hasOwnProperty(question)) { document.getElementById("response").innerHTML = storedAnswers[question];
            } else {
                var userAnswer = prompt("Maaf, aku enggak mengerti pertanyaan kamu. Aku harus jawab apa ya? :");
                storedAnswers[question] = "d^_^b  " + userAnswer;                document.getElementById("response").innerHTML = storedAnswers[question];
                // Save updated data to localStorage
                saveToLocalStorage();
            }
        }
    </script>
</body>
</html>
)rawliteral";

void setup() {
   Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    Serial.println("WiFi connected");
    Serial.println("IP address: " + WiFi.localIP().toString());
if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}
else {
    Serial.println("WiFi not connected");
// Aktifkan WiFi sebagai hotspot
WiFi.softAP("Iqhtyar", "23456789");
}
}

  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);

  pinMode(motorSpeedPin1, OUTPUT);
  pinMode(motorSpeedPin2, OUTPUT);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  server.on("/motor", HTTP_POST, [](AsyncWebServerRequest *request){
    String command = request->arg("command");
    int speed = request->arg("speed").toInt();

    if (command.equals("forward")) {
      digitalWrite(motorPin1, HIGH);
      digitalWrite(motorPin2, LOW);
      analogWrite(motorSpeedPin1, speed);
      analogWrite(motorSpeedPin2, speed);
    } else if (command.equals("reverse")) {
      digitalWrite(motorPin1, LOW);
      digitalWrite(motorPin2, HIGH);
      analogWrite(motorSpeedPin1, speed);
      analogWrite(motorSpeedPin2, speed);
  } else if (command.equals("left")) {
      digitalWrite(motorPin1, HIGH);
      digitalWrite(motorPin2, LOW);
      digitalWrite(motorPin3, HIGH);
      digitalWrite(motorPin4, LOW);
      analogWrite(motorSpeedPin1, speed);
      analogWrite(motorSpeedPin2, speed);
  } else if (command.equals("right")) {
      digitalWrite(motorPin1, HIGH);
      digitalWrite(motorPin2, LOW);
      digitalWrite(motorPin3, LOW);
      digitalWrite(motorPin4, HIGH);
      analogWrite(motorSpeedPin1, speed);
      analogWrite(motorSpeedPin2, speed);
    } else if (command.equals("stop")) {
      digitalWrite(motorPin1, LOW);
      digitalWrite(motorPin2, LOW);
      digitalWrite(motorPin3, LOW);
      digitalWrite(motorPin4, LOW);
      analogWrite(motorSpeedPin1, 0);
      analogWrite(motorSpeedPin2, 0);
    }

    request->send(200, "text/plain", "Motor diatur: " + command);
  });

  server.begin();
}

void loop() {
  // Tidak ada yang perlu dilakukan di loop utama untuk contoh ini
}