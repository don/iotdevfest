# Introduction

Following the latest announcements at CES 2019, the leadership team of the company you are working at has asked you to prototype the hardware and firmware for a commercial grade smart toilet fleet. As usual, the sales team said they needed this system yesterday and there is huge customer demand for it from the Fortune 500 companies they are in contact with.

![logo](../images/logo.svg)

The product manager in charge of the prototype has developed the following requirements:

* Each smart toilet must connect over WiFi to a secure cloud environment
* Push button flush (no one likes levers anymore, and cars have push button ignition)
* Real-time statistics for temperature and humidity must be sent every 30 seconds (who doesn’t like fancy graphs).
* Statistics on the number of flushes.
* Remote flush and out of service control.

There’s another team working on the backend system and dashboard, they’ve decided that the toilets need to use MQTT to communicate with backend. Along with certificate based TLS authorization (the de facto standard).

Your colleagues at the local makerspace have suggested you use the new Arduino MKR WiFi 1010 for the prototype along with the following components:

* MKR WiFi 1010
  * Arduino’s latest WiFi enabled board, featuring 32 kB of RAM, 256 kB of flash, built-in WiFi, crypto chip on board!
* Breadboard
* Wires and resistors
* LED
* DHT-22 sensor
* Photoresistor (for presence detection)
* Servo (to control flushing).

You haven’t used Arduino in a while, so first you decide to:

* Acquaint yourself with the hardware and get the basics going
* Learn how to connect the board to WiFi
* Learn what MQTT is and how to connect your board to the server
* Learn more about TLS and certificate based and how it applies
* Combine everything together for a fully working prototype.

Next [Exercise 1: Development Environment](exercise1.md)