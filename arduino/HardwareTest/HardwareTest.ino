// IoT DevFest Arduino Workshop
// Hardware Test
//
// Run this sketch to test that your hardware is wired correctly. 
// This blinks the LED, moves the servo, prints the temperature, 
// humidity, CdS value, and button state to the serial console.

// Adafruit DHT Sensor Library https://github.com/adafruit/DHT-sensor-library
// Adafruit Unified Sensor Library https://github.com/adafruit/Adafruit_Sensor

#include <Servo.h>

const int buttonPin = 5;
const int servoPin = 7;
const int photoresistorPin = A0;

Servo servo; 
int servoPosition = 0;

#include <DHT.h>
#define DHTTYPE DHT22
#define DHTPIN  2
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // initialize the serial output
  Serial.begin(9600);

  servo.attach(servoPin);

  // initialize temperature sensor
  dht.begin();

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT_PULLDOWN);

  // turn the LED on
  digitalWrite(LED_BUILTIN, HIGH);

  // wait for the serial connection from the computer
  while (!Serial) {
  }

  Serial.println("IoT DevFest MKR 1010 Hardware Test");
  
}

void loop() {

  // turn LED on
  digitalWrite(LED_BUILTIN, HIGH);
  // wait 1 second
  delay(1000);

  // turn LED off
  digitalWrite(LED_BUILTIN, LOW);
  // wait one second
  delay(1000);

  // move the servo
  if (servoPosition > 180) {
    servoPosition = 0;
  }
  Serial.print("Servo ");
  Serial.println(servoPosition);
  servo.write(servoPosition);
  servoPosition += 180;
  
  // read temperature and humidity
  float temperature = dht.readTemperature(true);
  float humidity = dht.readHumidity();

  // print sensor data to the serial console
  Serial.print(temperature);
  Serial.print("°F ");
  Serial.print(humidity);
  Serial.println("% RH");
  
  Serial.print("CdS ");
  Serial.println(analogRead(photoresistorPin));

  int buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    Serial.println("Button is pressed.");
  } else {
    Serial.println("Button is released.");    
  }

  Serial.println();

}
