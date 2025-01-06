

#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include <HTTPClient.h>
#include <PubSubClient.h>

const char* ssid = "POS_Server";    // WiFi connection ssid
const char* password = "asdffdsa";  // WiFi connection password

const char* mqtt_server = "192.168.0.100";  //  mqtt broker ip address
const char* mqtt_username = "detpos";       //  mqtt username
const char* mqtt_password = "asdffdsa";     //  mqtt password
const int mqtt_port = 1883;                 //  mqtt port

WiFiClient espClient;
PubSubClient client(espClient);  //   client from PubSubClient library


/*
  *** ATG Protocol Explain
  ** Reference from Aliwatt_Atg_protocol
  ** Command Format = <SOH>i201TT<EXT>
  * SOH = 0x01 
  * i = 0x69
  * 2 = 0x32
  * 0 = 0x30
  * 1 = 0x31
  * T = Tank Address
  * T = Tank Address
  *ETX = 0x03

  ** Example tank address
  * For Tank address 1 = T,T = 0x30, 0x31
  * For Tank address 2 = T,T = 0x30, 0x32
  .
  .
  .
  * For Tank address 12 = T,T = 0x31, 0x32
*/

unsigned char tank_1[8] = { 0x01, 0x69, 0x32, 0x30, 0x31, 0x30, 0x31, 0x03 };
unsigned char tank_2[8] = { 0x01, 0x69, 0x32, 0x30, 0x31, 0x30, 0x32, 0x03 };
unsigned char tank_3[8] = { 0x01, 0x69, 0x32, 0x30, 0x31, 0x30, 0x33, 0x03 };
unsigned char tank_4[8] = { 0x01, 0x69, 0x32, 0x30, 0x31, 0x30, 0x34, 0x03 };
unsigned char tank_5[8] = { 0x01, 0x69, 0x32, 0x30, 0x31, 0x30, 0x35, 0x03 };
unsigned char tank_6[8] = { 0x01, 0x69, 0x32, 0x30, 0x31, 0x30, 0x36, 0x03 };
unsigned char tank_7[8] = { 0x01, 0x69, 0x32, 0x30, 0x31, 0x30, 0x37, 0x03 };
unsigned char tank_8[8] = { 0x01, 0x69, 0x32, 0x30, 0x31, 0x30, 0x38, 0x03 };
unsigned char tank_9[8] = { 0x01, 0x69, 0x32, 0x30, 0x31, 0x30, 0x39, 0x03 };
unsigned char tank_10[8] = { 0x01, 0x69, 0x32, 0x30, 0x31, 0x31, 0x30, 0x03 };
unsigned char tank_11[8] = { 0x01, 0x69, 0x32, 0x30, 0x31, 0x31, 0x31, 0x03 };
unsigned char tank_12[8] = { 0x01, 0x69, 0x32, 0x30, 0x31, 0x31, 0x32, 0x03 };

unsigned char buffer[100];  // To read reply Serial data

char tank_product_ary[8];   // declare to split tank
char tank_volume_ary[8];    // declare to split tank Volume data
char remain_volume_ary[8];  // declare to split tank remain volume data
char oil_height_ary[8];     // declare to split tank Oil height level data
char temperature_ary[8];    // declare to split tank Temperature data
char water_level_ary[8];    // declare to split tank Water level data
char water_volume_ary[8];   // declare to split tank Water Volume data

/*All Tank Data Variable for Tank 1 */
float tank_volume_1;
float tank_product_1;
float tank_remain_1;
float oil_height_1;
float atg_temp_1;
float water_height_1;
float water_volume_1;

/*All Tank Data Variable for Tank 2 */
float tank_volume_2;
float tank_product_2;
float tank_remain_2;
float oil_height_2;
float atg_temp_2;
float water_height_2;
float water_volume_2;

/*All Tank Data Variable for Tank 3 */
float tank_volume_3;
float tank_product_3;
float tank_remain_3;
float oil_height_3;
float atg_temp_3;
float water_height_3;
float water_volume_3;

/*All Tank Data Variable for Tank 4 */
float tank_volume_4;
float tank_product_4;
float tank_remain_4;
float oil_height_4;
float atg_temp_4;
float water_height_4;
float water_volume_4;

/*All Tank Data Variable for Tank 5 */
float tank_volume_5;
float tank_product_5;
float tank_remain_5;
float oil_height_5;
float atg_temp_5;
float water_height_5;
float water_volume_5;

/*All Tank Data Variable for Tank 6 */
float tank_volume_6;
float tank_product_6;
float tank_remain_6;
float oil_height_6;
float atg_temp_6;
float water_height_6;
float water_volume_6;

/*All Tank Data Variable for Tank 7 */
float tank_volume_7;
float tank_product_7;
float tank_remain_7;
float oil_height_7;
float atg_temp_7;
float water_height_7;
float water_volume_7;

/*All Tank Data Variable for Tank 8 */
float tank_volume_8;
float tank_product_8;
float tank_remain_8;
float oil_height_8;
float atg_temp_8;
float water_height_8;
float water_volume_8;

/*All Tank Data Variable for Tank 9 */
float tank_volume_9;
float tank_product_9;
float tank_remain_9;
float oil_height_9;
float atg_temp_9;
float water_height_9;
float water_volume_9;

/*All Tank Data Variable for Tank 10 */
float tank_volume_10;
float tank_product_10;
float tank_remain_10;
float oil_height_10;
float atg_temp_10;
float water_height_10;
float water_volume_10;

/*All Tank Data Variable for Tank 11 */
float tank_volume_11;
float tank_product_11;
float tank_remain_11;
float oil_height_11;
float atg_temp_11;
float water_height_11;
float water_volume_11;

/*All Tank Data Variable for Tank 12 */
float tank_volume_12;
float tank_product_12;
float tank_remain_12;
float oil_height_12;
float atg_temp_12;
float water_height_12;
float water_volume_12;


#define wifiled 33   //   to know wifi connected led
#define powerled 32  // to know power led
#define TXled 27     // to know transmit data
#define RXled 26     //  to know receive data
#define RXD2 16      // Set UART RX pin
#define TXD2 17      // Set UART TX pin

int count;            // Buffer array index count
int state_count = 1;  // Tank number count
int wifi_try;         // wifi connecting count

int device_id = 0;   // device id
int tank_count = 0;  // number of tank
// int sensor_id = 0;

int fuel_id_1 = 0;   // to store fuel name with id for Tank 1
int fuel_id_2 = 0;   // to store fuel name with id for Tank 2
int fuel_id_3 = 0;   // to store fuel name with id for Tank 3
int fuel_id_4 = 0;   // to store fuel name with id for Tank 4
int fuel_id_5 = 0;   // to store fuel name with id for Tank 5
int fuel_id_6 = 0;   // to store fuel name with id for Tank 6
int fuel_id_7 = 0;   // to store fuel name with id for Tank 7
int fuel_id_8 = 0;   // to store fuel name with id for Tank 8
int fuel_id_9 = 0;   // to store fuel name with id for Tank 9
int fuel_id_10 = 0;  // to store fuel name with id for Tank 10
int fuel_id_11 = 0;  // to store fuel name with id for Tank 11
int fuel_id_12 = 0;  // to store fuel name with id for Tank 12

String Fuel_type = "";     // Fuel type for Tank
String Fuel_type_1 = "";   // Fuel type for Tank 1
String Fuel_type_2 = "";   // Fuel type for Tank 2
String Fuel_type_3 = "";   // Fuel type for Tank 3
String Fuel_type_4 = "";   // Fuel type for Tank 4
String Fuel_type_5 = "";   // Fuel type for Tank 5
String Fuel_type_6 = "";   // Fuel type for Tank 6
String Fuel_type_7 = "";   // Fuel type for Tank 7
String Fuel_type_8 = "";   // Fuel type for Tank 8
String Fuel_type_9 = "";   // Fuel type for Tank 9
String Fuel_type_10 = "";  // Fuel type for Tank 10
String Fuel_type_11 = "";  // Fuel type for Tank 11
String Fuel_type_12 = "";  // Fuel type for Tank 12

char atg_topic[20] = "detpos/atg/initial";  // Atg Mqtt initial setup topic
String incommingMessage;                    // To store message

bool atg_check;  // Tank active check

unsigned long start_time;  // Tank inactive send every 1 second

WebServer server(8080);  // Web Server PORT 8080

void setup() {
  Serial.begin(115200);                         // Starts the Serial communication at a baud rate of 115200 to print to the Serial Monitor.
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);  // Configures Serial2 communication with a baud rate of 9600, 8 data bits, None parity, 1 stop bit, RX on pin 16, and TX on pin 17

  EEPROM.begin(512);  // Initializes eeprom with a size for read and write operation

  pinMode(wifiled, OUTPUT);   // setup wifi led pin
  pinMode(powerled, OUTPUT);  // setup power led pin
  pinMode(TXled, OUTPUT);     // setup TX led pin
  pinMode(RXled, OUTPUT);     // setup RX led pin

  EEPROM_init();  // Read all initial Data from EEPROM

  digitalWrite(powerled, HIGH);  // turn on power Led

  server.on("/baseOilcan", handleJsonData);  // Web Serve API Route
  server.onNotFound(handleNotFound);         // Route Not Found API Route

  server.begin();  // ESP Web Server Host

  Serial2.write(tank_1, 8);  // initial request data
}

void loop() {

  if (WiFi.status() == WL_CONNECTED) {  // WiFi connection Condition
  atg_check = true;
    
  } else wifi_init();  // WiFi Reconnect Function

  server.handleClient();  // ESP Web Server Handle
}

void EEPROMWrite() {  // ATG initial Setup

  /*
    *** Fuel Type Setup each Tank
    ** if Tank 1 is 92 write 102 in 1 
    ** if Tank 2 is Preminum Diesel Write 103 in 4
    * 1 represent 92
    * 2 represent 95
    * 3 represent 97
    * 4 represent Premium Diesel 
    * 5 represent Diesel
  */
  EEPROM.write(100, 1);  // Device Id
  EEPROM.commit();
  EEPROM.write(101, 5);  // Tank Total Count
  EEPROM.commit();

  EEPROM.write(102, 1);  // Tank 1 fuel type =>(1=92,2=95,3=97,4=PD,5=D)
  EEPROM.commit();
  EEPROM.write(103, 2);  // Tank 2 fuel type =>(1=92,2=95,3=97,4=PD,5=D)
  EEPROM.commit();
  EEPROM.write(104, 3);  // Tank 3 fuel type =>(1=92,2=95,3=97,4=PD,5=D)
  EEPROM.commit();
  EEPROM.write(105, 4);  // Tank 4 fuel type =>(1=92,2=95,3=97,4=PD,5=D)
  EEPROM.commit();
  EEPROM.write(106, 5);  // Tank 5 fuel type =>(1=92,2=95,3=97,4=PD,5=D)
  EEPROM.commit();
  EEPROM.write(107, 0);  // Tank 6 fuel type =>(1=92,2=95,3=97,4=PD,5=D)
  EEPROM.commit();
  EEPROM.write(108, 0);  // Tank 7 fuel type =>(1=92,2=95,3=97,4=PD,5=D)
  EEPROM.commit();
  EEPROM.write(109, 0);  // Tank 8 fuel type =>(1=92,2=95,3=97,4=PD,5=D)
  EEPROM.commit();
  EEPROM.write(110, 0);  // Tank 9 fuel type =>(1=92,2=95,3=97,4=PD,5=D)
  EEPROM.commit();
  EEPROM.write(111, 0);  // Tank 10 fuel type =>(1=92,2=95,3=97,4=PD,5=D)
  EEPROM.commit();
  EEPROM.write(112, 0);  // Tank 11 fuel type =>(1=92,2=95,3=97,4=PD,5=D)
  EEPROM.commit();
  EEPROM.write(113, 0);  // Tank 12 fuel type =>(1=92,2=95,3=97,4=PD,5=D)
  EEPROM.commit();

  Serial.println("Yepp Save#############################################################");
}

void handleJsonData() {  // Convert JSON Data
  StaticJsonDocument<1024> jsonDoc;

  JsonArray data = jsonDoc.createNestedArray("data");  // Create a nested JSON array called "data"

  if (atg_check) {  // checks ATG active

    if (tank_count > 0) {  // Total Tank Count Control

      JsonObject entry1 = data.createNestedObject();
      entry1["stateInfo"] = "No alarm 1";
      entry1["weight"] = 0;
      entry1["oilRatio"] = 0;
      entry1["waterRatio"] = 0;
      entry1["canAddOilWeight"] = 0;
      entry1["id"] = 1;
    }
    if (tank_count > 1) {  // Total Tank Count Control
      JsonObject entry2 = data.createNestedObject();
      entry2["stateInfo"] = "No alarm 2";
      entry2["weight"] = 0;
      entry2["oilRatio"] = 0;
      entry2["waterRatio"] = 0;
      entry2["canAddOilWeight"] = 0; 
      entry2["id"] = 2;
    }

    if (tank_count > 2) {  // Total Tank Count Control
      JsonObject entry3 = data.createNestedObject();
      entry3["stateInfo"] = "No alarm 3";
      entry3["weight"] = 0;
      entry3["oilRatio"] = 0;
      entry3["waterRatio"] = 0;
      entry3["canAddOilWeight"] = 0; 
      entry3["id"] = 3;
    }
    if (tank_count > 3) {  // Total Tank Count Control
      JsonObject entry4 = data.createNestedObject();
      entry4["stateInfo"] = "No alarm 4";
      entry4["weight"] = 0;
      entry4["oilRatio"] = 0;
      entry4["waterRatio"] = 0;
      entry4["canAddOilWeight"] = 0; 
      entry4["id"] = 4;
    }

  } else {

    JsonObject entry1 = data.createNestedObject();
    entry1["condition"] = "ATG is not Active";
  }

  String jsonData;
  serializeJson(jsonDoc, jsonData);

  server.send(200, "application/json", jsonData);
}

void EEPROM_init() {  //  Read initial Setup data from EEPROM

  device_id = EEPROM.read(100);
  tank_count = EEPROM.read(101);
  fuel_id_1 = EEPROM.read(102);
  fuel_id_2 = EEPROM.read(103);
  fuel_id_3 = EEPROM.read(104);
  fuel_id_4 = EEPROM.read(105);
  fuel_id_5 = EEPROM.read(106);
  fuel_id_6 = EEPROM.read(107);
  fuel_id_7 = EEPROM.read(108);
  fuel_id_8 = EEPROM.read(109);
  fuel_id_9 = EEPROM.read(110);
  fuel_id_10 = EEPROM.read(111);
  fuel_id_11 = EEPROM.read(112);
  fuel_id_12 = EEPROM.read(113);

  Serial.printf("ATG Device is %d\n", device_id);
  Serial.printf("ATG tank count is %d\n", tank_count);
  Serial.printf("ATG Fuel type 1 id is %d\n", fuel_id_1);
  Serial.printf("ATG Fuel type 2 id is %d\n", fuel_id_2);
  Serial.printf("ATG Fuel type 3 id is %d\n", fuel_id_3);
  Serial.printf("ATG Fuel type 4 id is %d\n", fuel_id_4);
  Serial.printf("ATG Fuel type 5 id is %d\n", fuel_id_5);
  Serial.printf("ATG Fuel type 6 id is %d\n", fuel_id_6);
  Serial.printf("ATG Fuel type 7 id is %d\n", fuel_id_7);
  Serial.printf("ATG Fuel type 8 id is %d\n", fuel_id_8);
  Serial.printf("ATG Fuel type 9 id is %d\n", fuel_id_9);
  Serial.printf("ATG Fuel type 10 id is %d\n", fuel_id_10);
  Serial.printf("ATG Fuel type 11 id is %d\n", fuel_id_11);
  Serial.printf("ATG Fuel type 12 id is %d\n", fuel_id_12);

  Serial.println("EEPROM init Done#########################################");
}

void handleNotFound() {  // error route
  Serial.print("Unknown route requested: ");
  Serial.println(server.uri());

  String message = "{\"error\":\"ERROR 404\",\"message\":\"Route not found\"}";
  server.send(404, "application/json", message);
}

void wifi_init() {

  Serial.println("wifi init function");
  wifi_try = 0;

  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED && wifi_try++ != 15) {
    digitalWrite(wifiled, LOW);
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.print("Wifi is connected in ");
    Serial.println(ssid);

    Serial.println("IP Address: ");
    Serial.println(WiFi.localIP());
  }
}
