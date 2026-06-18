#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ======================================
// WIFI
// ======================================
const char* ssid = "********";
const char* password = "********";

// ======================================
// MQTT CLOUD
// ======================================
const char* mqtt_server =
"********.s1.eu.hivemq.cloud";

const int mqtt_port = 8883;

const char* mqtt_user = "********";

const char* mqtt_pass = "********";

// ======================================
// CLIENTS
// ======================================
WiFiClientSecure espClient;

PubSubClient mqttClient(
  espClient
);

// ======================================
// OLED
// ======================================
#define SCREEN_W 128
#define SCREEN_H 64

Adafruit_SSD1306 display(
  SCREEN_W,
  SCREEN_H,
  &Wire,
  -1
);

// ======================================
// UART
// ======================================
char buffer[128];

uint8_t idx = 0;

bool reading = false;

// ======================================
// DATA
// ======================================
float tempC = 0;

int vib = 0;

int rpm = 0;

float amp = 0;

int status = 0;

// ======================================
// TIMERS
// ======================================
uint32_t lastOLED = 0;

// ======================================
// WIFI
// ======================================
void connectWiFi() {

  Serial.println();
  Serial.println(
    "CONNECTING WIFI..."
  );

  WiFi.mode(
    WIFI_STA
  );

  WiFi.begin(
    ssid,
    password
  );

  while (
    WiFi.status() != WL_CONNECTED
  ) {

    delay(500);

    Serial.print(".");
  }

  Serial.println();

  Serial.println(
    "WIFI OK"
  );

  Serial.print(
    "IP: "
  );

  Serial.println(
    WiFi.localIP()
  );
}

// ======================================
// MQTT
// ======================================
void mqttReconnect() {

  while (
    !mqttClient.connected()
  ) {

    Serial.println(
      "CONNECTING MQTT..."
    );

    if (
      mqttClient.connect(
        "TonyFactoryNode",
        mqtt_user,
        mqtt_pass
      )
    ) {

      Serial.println(
        "MQTT CLOUD OK"
      );

    } else {

      Serial.print(
        "MQTT ERROR: "
      );

      Serial.println(
        mqttClient.state()
      );

      delay(3000);
    }
  }
}

// ======================================
// SETUP
// ======================================
void setup() {

  Serial.begin(115200);

  Serial2.begin(
    115200,
    SERIAL_8N1,
    16,
    17
  );

  // OLED
  Wire.begin(
    21,
    22
  );

  if (
    !display.begin(
      SSD1306_SWITCHCAPVCC,
      0x3C
    )
  ) {

    Serial.println(
      "OLED FAIL"
    );

    while (true);
  }

  display.clearDisplay();

  display.setTextSize(1);

  display.setTextColor(
    SSD1306_WHITE
  );

  display.setCursor(
    0,
    0
  );

  display.println(
    "SMART GUARDIAN"
  );

  display.println(
    "BOOTING..."
  );

  display.display();

  // WIFI
  connectWiFi();

  // TLS
  espClient.setInsecure();

  // MQTT
  mqttClient.setServer(
    mqtt_server,
    mqtt_port
  );

  delay(1000);

  Serial.println(
    "ESP32 READY"
  );
}

// ======================================
// LOOP
// ======================================
void loop() {

  if (
    !mqttClient.connected()
  ) {

    mqttReconnect();
  }

  mqttClient.loop();

  readUART();

  if (
    millis() - lastOLED > 150
  ) {

    lastOLED = millis();

    drawOLED();
  }
}

// ======================================
// UART
// ======================================
void readUART() {

  while (
    Serial2.available()
  ) {

    char c = Serial2.read();

    if (
      c == '<'
    ) {

      reading = true;

      idx = 0;

      continue;
    }

    if (
      c == '>'
    ) {

      reading = false;

      buffer[idx] = '\0';

      parseFrame();

      continue;
    }

    if (
      reading &&
      idx < sizeof(buffer)-1
    ) {

      buffer[idx++] = c;
    }
  }
}

// ======================================
// PARSER
// ======================================
void parseFrame() {

  sscanf(
    buffer,
    "%f,%d,%d,%f,%d",
    &tempC,
    &vib,
    &rpm,
    &amp,
    &status
  );

  Serial.print(
    "FRAME: "
  );

  Serial.println(
    buffer
  );

  publishMQTT();
}

// ======================================
// MQTT PUBLISH
// ======================================
void publishMQTT() {

  char msg[20];

  dtostrf(
    tempC,
    1,
    2,
    msg
  );

  mqttClient.publish(
    "tony/factory/temp",
    msg
  );

  itoa(
    vib,
    msg,
    10
  );

  mqttClient.publish(
    "tony/factory/vib",
    msg
  );

  itoa(
    rpm,
    msg,
    10
  );

  mqttClient.publish(
    "tony/factory/rpm",
    msg
  );

  dtostrf(
    amp,
    1,
    2,
    msg
  );

  mqttClient.publish(
    "tony/factory/amp",
    msg
  );

  itoa(
    status,
    msg,
    10
  );

  mqttClient.publish(
    "tony/factory/status",
    msg
  );
}

// ======================================
// OLED
// ======================================
void drawOLED() {

  display.clearDisplay();

  display.setCursor(
    0,
    0
  );

  display.print(
    "TEMP:"
  );

  display.print(
    tempC,
    1
  );

  display.println(
    "C"
  );

  display.setCursor(
    0,
    12
  );

  display.print(
    "VIB :"
  );

  display.println(
    vib ? "ON" : "OFF"
  );

  display.setCursor(
    0,
    24
  );

  display.print(
    "RPM :"
  );

  display.println(
    rpm
  );

  display.setCursor(
    0,
    36
  );

  display.print(
    "AMP :"
  );

  display.println(
    amp,
    2
  );

  display.setCursor(
    0,
    50
  );

  if (
    status == 0
  ) {

    display.print(
      "NORMAL"
    );

  }
  else if (
    status == 1
  ) {

    display.print(
      "WARNING"
    );

  }
  else {

    display.print(
      "ALERT"
    );
  }

  display.display();
}
