#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>


const char* ssid = "POS_Server";
const char* password = "asdffdsa";


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

char buffer[100];

char tank_product_ary[8];
char tank_volume_ary[8];
char remain_volume_ary[8];
char oil_height_ary[8];
char temperature_ary[8];
char water_level_ary[8];
char water_volume_ary[8];

int count;
int wifi_try;

WebServer server(8080);


void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);

   
  server.on("/baseOilcan", handleJsonData);  // server api

  server.begin();
  // Serial.println("Server started");
}


void handleJsonData() {
  StaticJsonDocument<1024> jsonDoc;

  JsonArray data = jsonDoc.createNestedArray("data");

  // Fill the JSON array with each entry
  JsonObject entry1 = data.createNestedObject();
  entry1["stateInfo"] = "No alarm";
  entry1["oilType"] = "Petrol 92";
  entry1["weight"] = 0;
  entry1["level"] = 2222;
  entry1["oilRatio"] = 0.3333;
  entry1["waterRatio"] = 0;
  entry1["canAddOilWeight"] = 0;
  entry1["temperature"] = 32.33;
  entry1["volume"] = 4444;
  entry1["connect"] = 3;
  entry1["id"] = 1;

  JsonObject entry2 = data.createNestedObject();
  entry2["stateInfo"] = "No alarm";
  entry2["oilType"] = "95 Octane";
  entry2["weight"] = 0;
  entry2["level"] = 4444;
  entry2["oilRatio"] = 0.5555;
  entry2["waterRatio"] = 0;
  entry2["canAddOilWeight"] = 0;
  entry2["temperature"] = 34.33;
  entry2["volume"] = 6666;
  entry2["connect"] = 3;
  entry2["id"] = 2;

  JsonObject entry3 = data.createNestedObject();
  entry3["stateInfo"] = "No alarm";
  entry3["oilType"] = "Diesel";
  entry3["weight"] = 0;
  entry3["level"] = 3333;
  entry3["oilRatio"] = 0.4444;
  entry3["waterRatio"] = 0;
  entry3["canAddOilWeight"] = 0;
  entry3["temperature"] = 33.33;
  entry3["volume"] = 5555;
  entry3["connect"] = 3;
  entry3["id"] = 3;

  JsonObject entry4 = data.createNestedObject();
  entry4["stateInfo"] = "No alarm";
  entry4["oilType"] = "Super Diesel";
  entry4["weight"] = 0;
  entry4["level"] = 5555;
  entry4["oilRatio"] = 0.6666;
  entry4["waterRatio"] = 0;
  entry4["canAddOilWeight"] = 0;
  entry4["temperature"] = 35.33;
  entry4["volume"] = 7777;
  entry4["connect"] = 3;
  entry4["id"] = 4;

  // Convert JSON document to string
  String jsonData;
  serializeJson(jsonDoc, jsonData);

  // Send JSON response with content type application/json
  server.send(200, "application/json", jsonData);
}

void loop() {

  if (WiFi.status() == WL_CONNECTED) {

    main_fun();

    delay(10);

  } else wifi_init();

  server.handleClient();
}

void main_fun() {

  if (Serial2.available()) {

    buffer[count] = Serial2.read();
    delay(5);

    // Serial.print(count);
    // Serial.print("/");
    Serial.print(buffer[count]);
    Serial.print(" ");
    count++;

    if (buffer[count - 7] == 0x26 && buffer[count - 6] == 0x26 && buffer[count - 1] == 0x03) {

      Serial.println();


      for (int i = 0; i < 8; i++) {

        tank_volume_ary[i] = buffer[26 + i];
        tank_product_ary[i] = buffer[34 + i];
        remain_volume_ary[i] = buffer[42 + i];
        oil_height_ary[i] = buffer[50 + i];
        water_level_ary[i] = buffer[58 + i];
        temperature_ary[i] = buffer[66 + i];
        water_volume_ary[i] = buffer[74 + i];
      }

      Serial.println();

      float tank_volume = hexToIEEEFloat(tank_volume_ary);
      Serial.print("ATG tank volume is => ");
      Serial.println(tank_volume, 4);
 
      float tank_product = hexToIEEEFloat(tank_product_ary);
      Serial.print("ATG tank product is => ");
      Serial.println(tank_product, 4);

      float tank_remain = hexToIEEEFloat(remain_volume_ary);
      Serial.print("ATG tank remain is => ");
      Serial.println(tank_remain, 4);
      
      float oil_height = hexToIEEEFloat(oil_height_ary);
      Serial.print("ATG tank oil height is => ");
      Serial.println(oil_height, 4);

      float water_height = hexToIEEEFloat(water_level_ary);
      Serial.print("ATG water height is => ");
      Serial.println(water_height, 4);

      float atg_temp = hexToIEEEFloat(temperature_ary);
      Serial.print("ATG Temperatur is => ");
      Serial.println(atg_temp, 2);

      float water_volume = hexToIEEEFloat(water_volume_ary);
      Serial.print("ATG water volume is => ");
      Serial.println(water_volume, 2);

    }

  }

}

float hexToIEEEFloat(char* hexArray) {
  uint32_t hexValue = 0;

  // Convert each hex character to a single 32-bit integer
  for (int i = 0; i < 8; i++) {
    hexValue <<= 4;
    if (hexArray[i] >= '0' && hexArray[i] <= '9')
      hexValue |= (hexArray[i] - '0');
    else if (hexArray[i] >= 'A' && hexArray[i] <= 'F')
      hexValue |= (hexArray[i] - 'A' + 10);
  }

  // Interpret the integer as a float
  float result;
  memcpy(&result, &hexValue, sizeof(result));
  return result;
}
void wifi_init() {

  Serial.println("wifi init function");
  wifi_try = 0;

  WiFi.begin(ssid, password);  ///akkkkk
  while (WiFi.status() != WL_CONNECTED && wifi_try++ != 15) {
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
