/**
   Cloud infrastructure specification

   1. Devices must connect to the the MQTT broker with a client id matching
      the devices id in the common certificate.

   2. If device is disconnected from WiFi or the MQTT broker, the out of
      service light must be turned on and all flush triggers from the button
      should be ignored.

   3. When the button is pressed and the device is in service, the servo
      is moved and a empty MQTT message is sent to the "things/<device id>/flush" topic.

   4. Environment data must be sent to the "things/<device id>/environment" topic
      every 10 seconds, in JSON format:

      {
        "temperature": <number>, // °F
        "humidity": <number>, // %
        "light": <number> // raw value
      }

   5. When a MQTT message is received on the "things/<device id>/remoteflush" topic,
      a flush must be triggered the same way as if the button was pressed (see 3).

   6. When a MQTT message is received on the "things/<device id>/outofservice" topic,
      if the paylaod is "on" out of service must be enabled, if the payload is "off"
      out of service must be disabled.

*/

#include <WiFiNINA.h>
#include <ArduinoBearSSL.h>
#include <ArduinoECCX08.h>
#include <ArduinoMqttClient.h>

#include "config.h"

WiFiClient wifiClient;
BearSSLClient sslClient(wifiClient);
MqttClient mqttClient(sslClient);

String clientId;


unsigned long getTime() {
  return WiFi.getTime();
}

void setup() {
  Serial.begin(9600);

  // Comment the next line disable waiting for a serial connection for debugging
  while (!Serial);

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
    //
    // enable out of service
    //

    connectWiFi();

    //
    // disable out of service
    //
  }

  if (!mqttClient.connected()) {
    // MQTT client is disconnected, connect

    //
    // enable out of service
    //

    connectMQTT();

    //
    // disable out of service
    //
  }

  // poll for new MQTT messages and send keep alives
  mqttClient.poll();

  //
  // check if button is pressed
  //

  //
  // publish sensor data periodically ...
  //
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

  //
  // Subscribe to topics
  //
}

void messageReceived(int messageSize) {
  String topic = mqttClient.messageTopic();
  String payload = mqttClient.readString();

  // we received a message, print out the topic and contents
  Serial.print("Received a message with topic '");
  Serial.print(topic);
  Serial.print("', length ");
  Serial.print(messageSize);
  Serial.println(" bytes:");

  //
  // Process incoming message
  //
}
