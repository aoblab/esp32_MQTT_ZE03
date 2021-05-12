#include <WiFi.h>
#include <PubSubClient.h>//Arduino Client for MQTT
#include <LiquidCrystal_I2C.h>//LCD
#include <ArduinoJson.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);//address, 20columns, 4rows

const char* ssid = "labin2.4";//Wi-Fi to access ID
const char* password = "12341234aa";//Wi-Fi to access PW
const char* mqtt_server = "192.168.0.23";//MQTT server address
const char* board_num = "esp32_001";//board number, If use multiple board, set the name differently ex)"esp32_002"
const char* board_num_topic = "esp32_001/output";//Node-RED -> esp32 board Topic
const char* board_num_O3 = "esp32_001/O3";//esp32 -> Node-RED Topic
byte command[] = {0xFF, 0x01, 0x78, 0x04, 0x00, 0x00, 0x00, 0x00, 0x83};//Q&A mode command
byte QnA[] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};//Q&A send command
byte readBuffer[9] = {};//Array of received data
float O3;//O3 Value
String messageO3 = "on";//char "message" -> string "messageO3" variable
long lastMsg = 0;//time
char O3msg[50];
IPAddress IP;//IP Address

unsigned char FucCheckSum(unsigned char *i, unsigned char ln)//O3 sensor checksum function
{
  unsigned char j, tempq = 0;
  i += 1;
  for (j = 0; j < (ln - 2); j++) {
    tempq += *i;
    i++;
  }
  tempq = (~tempq) + 1;
  return (tempq);
}

WiFiClient espClient;//client library
PubSubClient client(espClient);//Create initialized client instance
void setup() {
  Serial.begin(115200);//Serial speed 115200
  Serial2.begin(9600, SERIAL_8N1, 15, 13);//Communication with the ozone sensor, Serial2 speed9600, mode, RX, TX
  Serial2.write(command, sizeof(command));//QnA mode command send
  Serial2.readBytes(readBuffer, sizeof(readBuffer));//Receive QnA set data

  lcd.init();//LCD initialize
  lcd.backlight();//LCD backlight on

  setup_wifi(); //Connect to wifi
  client.setServer(mqtt_server, 1883);//MQTT server address, port num
  client.setCallback(callback);//message callback function called when a message arrives for a subscription created by this client
}

void setup_wifi() {//Wifi connection function
  WiFi.begin(ssid, password);//ID, PW
  while (WiFi.status() != WL_CONNECTED) {//Repeat until Wi-Fi is connected
    delay(500); Serial.println("Not connected");
  }
  Serial.print("WiFi connected IP : "); Serial.println(WiFi.localIP());//Wifi address print
  IP = WiFi.localIP();//Get wifi address
}

void callback(char* topic, byte* message, unsigned int length) {//allback function works only once when received
  Serial.print("Message arrived on topic: "); Serial.print(topic); Serial.print(". Message: ");//Received topic print
  messageO3 = ""; //Should be initialized every time a message is received

  for (int i = 0; i < length; i++) {//Convert byte unit message to string
    Serial.print((char)message[i]);
    messageO3 += (char)message[i];
  }
  Serial.println();

  if (String(topic) == board_num_topic) {//If Node-RED -> esp32 board Topic matches
    Serial.print("Changing output to ");
    if (messageO3 == "on") {//Action when received "on"
      Serial.println("on");
    }
    else if (messageO3 == "off") {//Action when received "off"
      Serial.println("off");
    }
  }
}

void reconnect() {//Reconnect function
  while (!client.connected()) {//Repeat until connected
    Serial.print("Attempting MQTT connection...");
    if (client.connect(board_num)) {//Connects to a specified IP address and port. The return value indicates success or failure
      Serial.println("connected");
      client.subscribe(board_num_topic);//Subscribes to messages published to the specified topic.
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

float makeJSON(float O3) { //float->JSON
  DynamicJsonDocument doc(200);
  doc["O3"] = O3;
  serializeJson(doc, O3msg);//Write the JSON document document without spaces or line break between values to O3msg
}

void loop() {
  if (!client.connected()) {//Call reconnect function until connected
    reconnect();
  }
  client.loop();

  Serial2.write(QnA, sizeof(QnA));//Q&A send
  Serial2.readBytes(readBuffer, sizeof(readBuffer));//Q&A read and save
  byte Check = FucCheckSum(readBuffer, sizeof(readBuffer));//Call checksum and compare
  if (Check == readBuffer[8]) {//Compare checksum
    O3 = ((readBuffer[2] * 256) + readBuffer[3]) * 0.1; //If checksum matches, save O3 value
    makeJSON(O3);//JSON func call
  }

  long now = millis();//the number of milliseconds passed since the board began running the current program
  if (now - lastMsg > 5000) {//Time period setting
    lastMsg = now;//save now time
    if (messageO3 == "on") {//if received message "on"
      lcd.setCursor(0, 0); lcd.print("AP:");
      lcd.setCursor(3, 0); lcd.print(IP);
      lcd.setCursor(0, 1); lcd.print("O3:");
      lcd.setCursor(3, 1); lcd.print(O3);
      client.publish(board_num_O3, O3msg);//Topic to send
      Serial.println(O3msg);
    }
    else if (messageO3 == "off") {
      lcd.clear();
      Serial.println("off");
      lcd.setCursor(0, 0); lcd.print("off");
    }
  }
}
