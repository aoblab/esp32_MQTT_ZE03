# esp32_MQTT_ZE03
![cvbcvvb](https://user-images.githubusercontent.com/32832715/116376370-257e9b80-a84b-11eb-8fb2-a6792777a022.PNG)

This is how to use MQTT communication protocol with the ESP32 to publish messages and subscribe to topics.

In this example, there’s a Node-RED application that controls multiple ESP32 and receives O3 sensor readings from the ESP32 using MQTT communication protocol. The Node-RED application is running on a Raspberry Pi.

We’ll use the Mosquitto broker installed on the same Raspberry Pi. The broker is responsible for receiving all messages, filtering the messages, decide who is interested in them and publishing the messages to all subscribed clients.

The Node-RED application publishes messages “on”, “off“ in the topic esp32_001/output.
The ESP32 is subscribed to that topic. So, it receives the message with “on” or “off” to communicate with O3 sensor.
The ESP32 publishes O3 value on the esp32_001/O3 topic.
The Node-RED application is subscribed to those topics. So, it receives O3 readings that can be displayed on a chart or gauge, for example.
