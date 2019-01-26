Code for testing that the hardware has been built correctly.

Ideally we'll have the user run one sketch, HardwareTest.ino, to test all hardware functionality and then run NetworkTest.ino to confirm the wireless works. 

 * HardwareTest.ino is intended to test all the component.
   * **TODO** instructions here.
 * NetworkTest.ino downloads a plaintext webpage.
   * Update the SSID and password in arduino_secrets.h.
   * Run the sketch and look for the text "IT WORKS!" in the serial terminal to confirm success.

These sketches test individual components
 * Blink.ino is the default Arduino blink sketch.
 * Button.ino is a modified version the Arduino button sketch.
 * Photoresistor.ino tests the CdS. (It doesn't need the 12-bit ADC, I was just testing it out.)
 * DHT22.ino is the hardware test from my other workshops. It test temperature, humidity, and the LED.
 * Servo.ino moves the servo motor back and forth.
