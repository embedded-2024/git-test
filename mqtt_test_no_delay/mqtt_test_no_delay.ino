#include <PubSubClient.h>
#include <WiFi.h>

String incommingMessage;

const char* mqtt_server = "192.168.0.100";
const char* mqtt_username = "detpos";
const char* mqtt_password = "asdffdsa";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

char pplive1[25] = "detpos/device/livedata/1";
char pplive2[25] = "detpos/device/livedata/2";
char pplive3[25] = "detpos/device/livedata/3";
char pplive4[25] = "detpos/device/livedata/4";

char pumpreqbuf[23] = "detpos/device/permit/1";
char pumppresetbuf[28] = "detpos/local_server/preset";

void setup() {

  Serial.begin(115200);

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}


void loop() {

  if (WiFi.status() == WL_CONNECTED) {

    if (!client.connected()) {
      reconnect();
    } else {
      main_fun();
    }

  } else wifi_init();

  client.loop();
}


void main_fun() {
  client.publish(pplive1, "01live");
  client.publish(pplive2, "02live");
  client.publish(pplive3, "03live");
  client.publish(pplive4, "04live");
  delay(20);

}


void callback(char* topic, byte* payload, unsigned int length) {
  incommingMessage = "";

  for (int i = 0; i < length; i++) {
    incommingMessage += (char)payload[i];
  }

  Serial.println(topic);
  // delay(2000);
  if (String(topic) == String(pumppresetbuf)) {
    Serial.println("Permit");
    delay(2000);
  }

  if (String(topic) == String(pumpreqbuf)) {
    Serial.println("Appro");
    delay(2000);
  }
}



void reconnect() {
  while (!client.connected()) {

    Serial.println("MQtt is connecting ");
    String clientId = "Rfid_client-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Mqtt connected");

      client.subscribe("Connect");                    // subscribe the topics here
      // client.publish("connect", "Device1Connected");  // subscribe the topics here
      client.subscribe("detpos/#");

    } else {
      Serial.print("mqtt connection failed, rc=");
      Serial.print(client.state());
      Serial.print(" try again in 2 seconds");
      delay(2000);
    }
  }
}

void wifi_init() {

  Serial.println("wifi init function");


  WiFi.begin("POS_Server", "asdffdsa");  ///akkkkk
  
  while (WiFi.status() != WL_CONNECTED) {

    Serial.print(".");
    delay(200);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.println("Wifi is connected..!");

    Serial.println(WiFi.localIP());
  }
}