# esp32_MQTT_ZE03
![cvbcvvb](https://user-images.githubusercontent.com/32832715/116376370-257e9b80-a84b-11eb-8fb2-a6792777a022.PNG)
This is how to use MQTT communication protocol with the ESP32 to publish messages and subscribe to topics.

In this example, there’s a Node-RED application that controls multiple ESP32 and receives O3 sensor readings from the ESP32 using MQTT communication protocol. The Node-RED application is running on a Raspberry Pi.

We’ll use the Mosquitto broker installed on the same Raspberry Pi. The broker is responsible for receiving all messages, filtering the messages, decide who is interested in them and publishing the messages to all subscribed clients.

The Node-RED application publishes messages “on”, “off“ in the topic esp32_001/output.
The ESP32 is subscribed to that topic. So, it receives the message with “on” or “off” to communicate with O3 sensor.
The ESP32 publishes O3 value on the esp32_001/O3 topic.
The Node-RED application is subscribed to those topics. So, it receives O3 readings that can be displayed on a chart or gauge, for example.

# Preparing Installing the PubSubClient Library
The PubSubClient library provides a client for doing simple publish/subscribe messaging with a server that supports MQTT (basically allows your ESP32 to talk with Node-RED)  
https://github.com/knolleary/pubsubclient  
Go to Sketch > Include Library > Add . ZIP library and select the library you’ve just downloaded.

# Preparing Node-RED Dashboard
The ESP32 is publishing O3 sensor readings every 5 seconds on four MQTT topics. Now, you can use any dashboard that supports MQTT or any other device that supports MQTT to subscribe to those topics and receive the readings.

## enter the following commands to install Node-RED:
bash <(curl -sL https://raw.githubusercontent.com/node-red/raspbian-deb-package/master/resources/update-nodejs-and-nodered)   

## To automatically run Node-RED when the Pi boots up, you need to enter the following command:
sudo systemctl enable nodered.service  

## Installing Node-RED Dashboard
To install the Node-RED Dashboard run the following commands:   
node-red-stop   
cd ~/.node-red   
npm install node-red-dashboard   

## Now, restart your Pi so the autostart takes effect:
sudo reboot  

To open the Node-RED UI, type your Raspberry Pi IP address in a web browser followed by :1880/ui as shown below:   
http://Your_RPi_IP_address:1880/ui

## Mosquitto Broker
To install the Mosquitto Broker enter these next commands:  
sudo apt update  
sudo apt install -y mosquitto mosquitto-clients  

You’ll have to type Y and press Enter to confirm the installation. To make Mosquitto auto start on boot up enter:  
sudo systemctl enable mosquitto.service
