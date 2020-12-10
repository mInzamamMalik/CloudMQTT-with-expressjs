#include <WiFi.h>
#include <PubSubClient.h>
WiFiClient wifiClient;
PubSubClient pusSubClient(wifiClient);


// Replace these with your SSID/Password
const char* ssid = "phone";
const char* password = "ios14beta";


// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqttServer = "fantastic-hairdresser.cloudmqtt.com";
const int mqttPort = 1883; // dont know yet how to connect with secure port or wss port
const char* mqttUser = "axrmohhl";
const char* mqttPassword = "B6Qo1CCA-qSl";



long lastMsg = 0;
char msg[50];
int value = 0;

// LED Pin
const int ledPin = 2 ;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println(WiFi.status());
    Serial.println(WL_CONNECTED);
    Serial.println("trying to connect to wifi...");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  pinMode(ledPin, OUTPUT);

  pusSubClient.setServer(mqttServer, mqttPort);
  pusSubClient.setCallback(callback);
}

void loop() {

  if (!pusSubClient.connected()) {
    Serial.println("disconnected MQTT");
    reconnect();
  }
  pusSubClient.loop();

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    
    
  }
}


void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == "turnBulb") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(ledPin, HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(ledPin, LOW);
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!pusSubClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (pusSubClient.connect("ESP8266Client", mqttUser, mqttPassword )) {
      Serial.println("connected");
      // Subscribe
      pusSubClient.subscribe("turnBulb");
    } else {
      Serial.print("failed, rc=");
      Serial.print(pusSubClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
