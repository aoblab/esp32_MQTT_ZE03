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

## Mosquitto Broker
To install the Mosquitto Broker enter these next commands:  
sudo apt update  
sudo apt install -y mosquitto mosquitto-clients  

You’ll have to type Y and press Enter to confirm the installation. To make Mosquitto auto start on boot up enter:  
sudo systemctl enable mosquitto.service

## Testing Installation Send the command:
pi@raspberry:~ $ mosquitto -v  
This returns the Mosquitto version that is currently running in your Raspberry Pi. It should be 1.4.X or above.  
![raspberr-pi-ip-address](https://user-images.githubusercontent.com/32832715/116501838-d426e880-a8ec-11eb-9671-6b361dacf701.png)  

## Raspberry Pi IP Address
To use Mosquitto broker later on your projects, you’ll need your Raspberry Pi IP address.
To retrieve your Raspberry Pi IP address, type the next command in your Terminal window:  
pi@raspberry:~ $ hostname -I

## Node-RED UI  
To open the Node-RED UI, type your Raspberry Pi IP address in a web browser followed by :1880/ui as shown below:   
http://Your_RPi_IP_address:1880/ui

## Creating the Node-RED flow  
Import the Node-RED flow provided. Go to the GitHub URL and copy the code provided  
https://github.com/aoblab/esp32_MQTT_ZE03/blob/f53780e583c471dbc47582efa20534fd5ab6abd8/flows.json#L1

Next, in the Node-RED window, at the top right corner, select the menu, and go to Import  > Clipboard  
![1](https://user-images.githubusercontent.com/32832715/116502988-f40bdb80-a8ef-11eb-8f8b-d6315cb9c3ab.png)

Then, paste the code provided and click Import  
![2](https://user-images.githubusercontent.com/32832715/116502991-f53d0880-a8ef-11eb-8246-3e286388ea66.PNG)

The following nodes should load:  
![3](https://user-images.githubusercontent.com/32832715/116503059-23224d00-a8f0-11eb-95c5-21bccc4003a2.PNG)

After making any changes, click the Deploy button to save all the changes  
![4](https://user-images.githubusercontent.com/32832715/116503069-29182e00-a8f0-11eb-8db1-75dde841e52a.png)

## Node-RED UI
Now, your Node-RED application is ready. To access Node-RED UI and see how your application looks, access any browser in your local network and type:  
http://Your_RPi_IP_address:1880/ui  

Your application should look as shown in the following figure. You can control the esp32 board "on/off" and  you can view O3 value readings in a chart  
![1](https://user-images.githubusercontent.com/32832715/116503291-a80d6680-a8f0-11eb-914c-a5515f1a3059.PNG)  
