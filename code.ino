  #include <ESP8266WiFi.h>
  #include <DHT.h>
  #include <ESP8266WebServer.h>
  #include <Wire.h>
  #include <LiquidCrystal_I2C.h>
  #include <Servo.h>

  // WiFi credentials
  char ssid[] = "Vodafone-7D58";
  char password[] = "d5a8kT4jsA7Ykyuf";

  // Initialize the web server on port 80
  ESP8266WebServer server(80);

  // Define the temperature sensor and LED pins
  const int tempSensorPin = A0;  // Pin for temperature sensor (Analog Pin)
  const int ledPin = 0;          // Pin for LED control (GPIO0 -> D3)
  const int servoPin = D6;       // Pin for servo motor control (GPIO12 -> D6)

  // Initialize the LCD display with the I2C address 0x27 and 20x4 characters
  LiquidCrystal_I2C lcd(0x27, 20, 4);

  DHT dht(D5, DHT11);
  Servo servo;

  // Initialize global variables for temperature
  float temperature = 0.0;

  int open = 0, closed = 1;

  // Global HTML page content
  const char* page = R"=====(
  <html>
    <head>
      <script src='https://code.jquery.com/jquery-3.3.1.min.js'></script>
      <title>Smart Home Interface</title>
    </head>
    <body>
      <h2>This is the interface to your smart home.</h2>
      <div>
        <button onclick="turnOnLED()">Turn On LED</button>
        <button onclick="turnOffLED()">Turn Off LED</button>
        <button onclick="rotate90()">Rotate 90°</button>
        <button onclick="rotateMinus90()">Rotate -90°</button>
      </div>
      <script>
        $(document).ready(function () {
          setInterval(refreshData, 1000);  // Adjust interval as needed
        });

        function refreshData() {
          $.get('/getStatus', function (data) {
            var status = JSON.parse(data);
            $('#Temperature').text(status.temperature);
          });
        }

        function turnOnLED() {
          $.post('/control', { action: 'turnOnLED' });
        }

        function turnOffLED() {
          $.post('/control', { action: 'turnOffLED' });
        }

        function rotate90() {
          $.post('/control', { action: 'rotate90' });
        }

        function rotateMinus90() {
          $.post('/control', { action: 'rotateMinus90' });
        }
      </script>
    </body>
  </html>
  )=====";

  void setup() {

    dht.begin();
    servo.attach(servoPin);

    Serial.begin(115200);
    delay(1000); // Wait for Serial Monitor to initialize
    Serial.println("Setup started");

    // Initialize the LCD display
    Wire.begin(4, 5); // SCL -> GPIO5 (D1), SDA -> GPIO4 (D2)
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Smart Home");
    lcd.setCursor(0, 1);
    lcd.print("Initializing...");

    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW); // Ensure LED is initially off

    // Connect to WiFi
    connectToWiFi();

    // Setup server routes
    server.on("/", HTTP_GET, handleRoot);
    server.on("/getStatus", HTTP_GET, handleGetStatus);
    server.on("/control", HTTP_POST, handleControl);
    server.begin();
    Serial.println("HTTP server started");

    // Print IP address to Serial Monitor
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // Clear the LCD display and show ready status
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Smart Home Ready");
  }

  void loop() {
    server.handleClient();
    // Check if 1 minute has passed since the last temperature update
    delay(2000);
    temperature = dht.readTemperature() + 10;
    updateLCD();
    // Check WiFi status
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("WiFi disconnected, attempting to reconnect...");
      connectToWiFi();
    }

  }

  void connectToWiFi() {
    Serial.println("Connecting to the WiFi");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("Waiting for connection");
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }

  void handleRoot() {
    Serial.println("Root requested");
    server.send(200, "text/html", page);
  }

  void handleGetStatus() {
    Serial.println("Status requested");
    String jsonResponse = "{ \"temperature\": \"" + String(temperature, 2) + "\" }";
    server.send(200, "application/json", jsonResponse);
  }

  void handleControl() {
    Serial.println("Control requested");
    if (server.hasArg("action")) {
      String action = server.arg("action");
      if (action == "turnOnLED") {
        digitalWrite(ledPin, HIGH);
        Serial.println("LED turned ON");
      } else if (action == "turnOffLED") {
        digitalWrite(ledPin, LOW);
        Serial.println("LED turned OFF");
      } 
      if (action == "rotate90" && closed == 1) {
        rotateServo(180);
        Serial.println("Servo rotated 90 degrees");
        closed = 0;
        open = 1;
      } else if (action == "rotateMinus90" && open == 1) {
        rotateServo(0);
        Serial.println("Servo rotated -90 degrees");
        closed = 1;
        open = 0;
      }
      server.send(200, "text/plain", "OK");
    } else {
      server.send(400, "text/plain", "Bad Request");
    }
  }

  void rotateServo(int angle) {
    servo.write(angle);
    delay(500); // Wait for the servo to reach the desired position
  }

  void updateLCD() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Smart Home");
    lcd.setCursor(0, 1);
    lcd.print("Temp: ");
    lcd.print(temperature, 1); // Print temperature with 1 decimal place
    lcd.print(" C"); // Add temperature unit

    //Serial.println("LCD updated");
  }
