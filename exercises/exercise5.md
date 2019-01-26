# Exercise 5: Graphing MQTT Data

1. Open www/chart/index.html in your web browser
1. Enter a username and password
1. Enter your device id
1. Press Connect
1. Data will be plotted on the charts as it is arrives via MQTT

    ![Temperature and Humidity chart](../images/chart.png)

    * If you're not seeing data, double check the device id.
    * Try breathing on the sensor to change the values.

1. Open [/www/chart/mqtt.js](/www/chart/mqtt.js) to see the MQTT connection code. The chart can be modified by editing [/www/chart/chart.js](/www/chart/chart.js).

Next [Exercise 6: Sending Data to Arduino](exercise6.md)