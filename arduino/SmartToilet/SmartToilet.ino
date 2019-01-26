/*
   TODO update instructions and duplicate (or move) to Markdown
*/

#include <WiFiNINA.h>
#include <ArduinoBearSSL.h>
#include <ArduinoECCX08.h>
#include <ArduinoMqttClient.h>

#include <Servo.h>

// Temperature and Humidity Sensor
#include <DHT.h>

#include "config.h"

WiFiClient wifiClient;
BearSSLClient sslClient(wifiClient);
MqttClient mqttClient(sslClient);

const int dhtPin = 2;
const int buttonPin = 5;
const int servoPin = 7;
const int photoresistorPin = A0;

Servo servo;
DHT dht(dhtPin, DHT22);

String clientId;

// Publish every 10 seconds for the workshop. Real world apps need this data every 5 or 10 minutes.
unsigned long publishInterval = 10 * 1000;
unsigned long lastMillis = 0;

int buttonState;
int lastButtonState;
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the double flushes

bool outOfService = true;

unsigned long getTime() {
  return WiFi.getTime();
}

void setup() {
  Serial.begin(9600);

  // Comment the next line to NOT wait for a serial connection for debugging
  while (!Serial);

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  // initialize digital button as an input using the internal pulldown resistor
  pinMode(buttonPin, INPUT_PULLDOWN);

  // initialize temperature sensor
  dht.begin();

  servo.attach(servoPin);
  servo.write(0);

  // set a callback to get the current time
  // used for certification validation
  ArduinoBearSSL.onGetTime(getTime);

  if (!ECCX08.begin()) {
    Serial.println("No ECCX08 present!");
    while (1);
  }


  // use the device id from config.h as the client id
  clientId = DEVICE_ID;
  // alternatively we could use the serial number of the ECCx08 chip
  // if the certificate is configure to use that as the common name
  // clientId = ECCX08.serialNumber();

  Serial.print("Client id = ");
  Serial.println(clientId);

  // set the ECCX08 slot to use for the private key
  // and the accompanying public cert for it
  sslClient.setEccSlot(0, CERTIFICATE);

  // set the client id
  mqttClient.setId(clientId);

  // set the message callback
  mqttClient.onMessage(messageReceived);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    setOutOfService(true);
    connectWiFi();
  }

  if (!mqttClient.connected()) {
    setOutOfService(true);
    
    // MQTT client is disconnected, connect
    connectMQTT();

    setOutOfService(false);
  }

  // poll for new MQTT messages and send keep alives
  mqttClient.poll();

  if (!outOfService) {
    checkButton();
  }

  if (millis() - lastMillis > publishInterval) {
    lastMillis = millis();

    sendSensorData();
  }
}

void connectWiFi() {
  Serial.print("Attempting to connect to SSID: ");
  Serial.print(WIFI_SSID);
  Serial.print(" ");

  while (WiFi.begin(WIFI_SSID, WIFI_PASS) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(3000);
  }
  Serial.println();

  Serial.println("You're connected to the network");
  Serial.println();

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void connectMQTT() {
  Serial.print("Attempting to MQTT broker: ");
  Serial.print(MQTT_BROKER);
  Serial.println(" ");

  while (!mqttClient.connect(MQTT_BROKER, 8883)) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }
  Serial.println();

  Serial.println("You're connected to the MQTT broker");
  Serial.println();


  mqttClient.subscribe("things/" + clientId + "/outofservice");
  mqttClient.subscribe("things/" + clientId + "/remoteflush");
}

void checkButton() {
  int reading = digitalRead(buttonPin);

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == HIGH) {  //button is pressed
        Serial.print("Button pressed ");
        flush();
      }
    }
  }

  lastButtonState = reading;
}

void messageReceived(int messageSize) {
  String topic = mqttClient.messageTopic();

  // we received a message, print out the topic and contents
  Serial.print("Received a message with topic '");
  Serial.print(topic);
  Serial.print("', length ");
  Serial.print(messageSize);
  Serial.println(" bytes:");

  if (topic.endsWith("/outofservice")) {
    String payload = mqttClient.readString();

    if (payload == "on") {
      setOutOfService(true);
    } else if (payload == "off") {
      setOutOfService(false);
    }
  } else if (topic.endsWith("/remoteflush")) {
    flush();
  } else {
    Serial.print("Ignoring message on topic ");
    Serial.println(topic);
  }
}

void flush() {
  Serial.println("flushing");
  servo.write(45);
  mqttClient.beginMessage("things/" + clientId + "/flush");
  mqttClient.endMessage();
  delay(300);
  servo.write(0);
}

void setOutOfService(bool val) {
  outOfService = val;

  if (outOfService) {
    // turn the LED on
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    // turn the LED off
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void sendSensorData() {
  float temperature = dht.readTemperature(true); // true for °F
  float humidity = dht.readHumidity();
  int lightLevel = analogRead(photoresistorPin);

  Serial.print(temperature);
  Serial.print("°F ");
  Serial.print(humidity);
  Serial.println("% RH");
  Serial.print("CdS ");
  Serial.println(lightLevel);

  // AWS prefers JSON
  mqttClient.beginMessage("things/" + clientId + "/environment");
  mqttClient.print("{");
  mqttClient.print("\"temperature\":");
  mqttClient.print(temperature);
  mqttClient.print(", ");
  mqttClient.print("\"humidity\":");
  mqttClient.print(humidity);
  mqttClient.print(", ");
  mqttClient.print("\"light\":");
  mqttClient.print(lightLevel);
  mqttClient.print("}");
  mqttClient.endMessage();
}
