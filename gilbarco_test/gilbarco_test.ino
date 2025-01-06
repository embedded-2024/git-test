#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <EEPROM.h>
#include <ArduinoJson.h>

const char* mqtt_server = "192.168.0.100";
const char* mqtt_username = "detpos";
const char* mqtt_password = "asdffdsa";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

String incommingMessage;

// mqtt topic
char pumpapprobuf[22] = "detpos/local_server/1";
char pumpreqbuf[23] = "detpos/device/permit/1";
char pumppresetbuf[28] = "detpos/local_server/preset";
char pplive[25] = "detpos/device/livedata/1";
char ppfinal[22] = "detpos/device/Final/1";
char pricechange[26] = "detpos/local_server/price";
char pricereqbuf[25] = "detpos/device/pricereq/1";
char activebuf[23] = "detpos/device/active/1";
char devicebuf[2] = "1";
char device_Id_topic[40] = "detpos/local_server/initial1/det/0A0000";
char Reset_topic[17] = "detpos/hmi/reset";
char server_rpy[20] = "detpos/local_server";      //ak
char server_err[25] = "detpos/local_server/err";  //ak
char myreload[29] = "detpos/local_server/reload/1";
char pumpfinalreloadbuf[23] = "detpos/device/Reload/1";  // reload add

bool pumpapprocount1 = false;
bool pumpapprocount2 = false;

bool zerocount = true;

char mqttdatabuf[50];


#define wifiled 33
#define powerled 32
#define TXled 27
#define RXled 26
#define RXD2 16  // Set your RX pin
#define TXD2 17  // Set your TX pin


String pump2_status = "ide";
String pump1_status = "ide";


String price;
String price_state;
String total_state;
String total_amount;
String liter;

int bufcount = 0;

int pumpid1 = 1;
int pumpid2 = 2;
int pumpid3;
int pumpid4;
int pumpid5;
int pumpid6;
int pumpid7;
int pumpid8;
int nozzlenum;
int devicenum;

char presetmqttary[11];
char pricechangeary[7];
char charArray[4];

int pumpmqttnum;
int pumpmqttnum2;
int pumpmqttnum3;

bool presetcount = false;

bool lastpresetsend = false;

long datatime1 = 0;
long datatime2 = 0;


char buffer[150];
int i;
int count;

unsigned long start_time;
unsigned long start_time_1;

unsigned char state_req_1[1] = { 0x01 };
unsigned char state_req_2[1] = { 0x02 };

unsigned char pump_stop_1[1] = { 0x31 };
unsigned char pump_stop_2[1] = { 0x32 };

unsigned char tran_data[1] = { 0x41 };
unsigned char tran_data_1[1] = { 0x42 };

unsigned char auth_1[1] = { 0x11 };
unsigned char auth_2[1] = { 0x12 };

unsigned char live_1[1] = { 0x61 };
unsigned char live_2[1] = { 0x62 };

unsigned char final_data[1] = { 0x51 };
unsigned char final_data_1[1] = { 0x52 };


bool active_check = false;

// unsigned char dd[2] = {0x31, 0x31};

void setup() {

  Serial.begin(115200);
  Serial2.begin(5787, SERIAL_8E1, 16, 17);

  EEPROM.begin(512);

  pinMode(15, OUTPUT);
  pinMode(wifiled, OUTPUT);
  pinMode(powerled, OUTPUT);
  pinMode(TXled, OUTPUT);
  pinMode(RXled, OUTPUT);

  // EEPROMWrite();   // to change

  initEEPROMdatashow();
  EEPROMinit();

  initWiFi();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);


  delay(2000);

  // digitalWrite(15, HIGH);

  // Serial2.write(state_req_1, 1);
  // delay(2);
  // digitalWrite(15, LOW);
}

void EEPROMWrite() {
  EEPROM.write(109, 1);  //device id
  EEPROM.commit();
  EEPROM.write(110, 2);  // nozzle count
  EEPROM.commit();
  EEPROM.write(101, 1);  // nozzle id
  EEPROM.commit();
  EEPROM.write(102, 2);
  EEPROM.commit();
  EEPROM.write(103, 0);
  EEPROM.commit();
  EEPROM.write(104, 0);
  EEPROM.commit();
  EEPROM.write(105, 0);
  EEPROM.commit();
  EEPROM.write(106, 0);
  EEPROM.commit();
  EEPROM.write(107, 0);
  EEPROM.commit();
  EEPROM.write(108, 0);
  EEPROM.commit();

  Serial.println("yep all save $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
}

void loop() {

  delay(1);

  if (Serial2.available() > 0) {

    start_time = millis() / 1000;

    buffer[i] = Serial2.read();
    delay(2);
    // delay(2);

    Serial.print(i);
    Serial.print("/");
    Serial.print(buffer[i], HEX);
    Serial.print(" ");

    i++;

    // Serial.println();

    // if (buffer[i - 1] == 0x71 || buffer[i - 1] == 0x72) i = 0;

    if (buffer[i - 1] == 0xF0) i = 0;

    if (buffer[i - 1] == 0x61 || buffer[i - 1] == 0x62) i = 0;

    if (buffer[i - 1] == 0x91) {  // live one

      Serial.println();

      for (int i = 0; i < 151; i++) {
        buffer[i] = 0;
      }

      i = 0;

      digitalWrite(15, HIGH);
      Serial2.write(live_1, 1);
      delay(2);
      digitalWrite(15, LOW);

      while (!Serial2.available() > 0) {
        delay(2);
      }

      while (Serial2.available() > 0) {
        buffer[i] = Serial2.read();
        delay(2);

        Serial.print(i);
        Serial.print("/");
        Serial.print(buffer[i], HEX);
        Serial.print(" ");

        i++;

        if (buffer[i - 1] == 0xF0) i = 0;
      }
    }

    if (buffer[i - 1] == 0x92) {  // live two

      Serial.println();

      for (int i = 0; i < 151; i++) {
        buffer[i] = 0;
      }

      i = 0;

      digitalWrite(15, HIGH);
      Serial2.write(live_2, 1);
      delay(2);
      digitalWrite(15, LOW);

      while (!Serial2.available() > 0) {
        delay(2);
      }

      while (Serial2.available() > 0) {
        buffer[i] = Serial2.read();
        delay(2);

        Serial.print(i);
        Serial.print("/");
        Serial.print(buffer[i], HEX);
        Serial.print(" ");

        i++;

        if (buffer[i - 1] == 0xF0) i = 0;
      }
    }

    manage_msg();

    if (buffer[i - 1] == 0xB1 || buffer[i - 1] == 0xA1) {

      Serial.println();

      i = 0;

      digitalWrite(15, HIGH);
      Serial2.write(tran_data, 1);
      delay(2);
      digitalWrite(15, LOW);

      while (!Serial2.available()) {
        delay(2);
      }

      while (Serial2.available() > 0) {

        // Serial.print("i am B1 tran");
        buffer[i] = Serial2.read();
        delay(5);

        Serial.print(i);
        Serial.print("/");
        Serial.print(buffer[i], HEX);
        Serial.print(" ");

        i++;

        if (buffer[i - 1] == 0xF0) i = 0;
      }

      Serial.println();

      digitalWrite(15, HIGH);
      Serial2.write(final_data, 1);
      delay(2);
      digitalWrite(15, LOW);

      while (!Serial2.available()) {
        delay(2);
      }

      while (Serial2.available() > 0) {

        buffer[i] = Serial2.read();
        delay(5);

        Serial.print(i);
        Serial.print("/");
        Serial.print(buffer[i], HEX);
        Serial.print(" ");

        i++;

        if (buffer[i - 1] == 0xF0) i = 0;
      }
    }

    if (buffer[i - 1] == 0xB2 || buffer[i - 1] == 0xA2) {

      Serial.println();

      i = 0;

      digitalWrite(15, HIGH);
      Serial2.write(tran_data_1, 1);
      delay(2);
      digitalWrite(15, LOW);

      while (!Serial2.available()) {
        delay(2);
      }

      while (Serial2.available() > 0) {

        // Serial.print("i am B1 tran");
        buffer[i] = Serial2.read();
        delay(5);

        Serial.print(i);
        Serial.print("/");
        Serial.print(buffer[i], HEX);
        Serial.print(" ");

        i++;

        if (buffer[i - 1] == 0xF0) i = 0;
      }

      Serial.println();

      digitalWrite(15, HIGH);
      Serial2.write(final_data_1, 1);
      delay(2);
      digitalWrite(15, LOW);

      while (!Serial2.available()) {
        delay(2);
      }

      while (Serial2.available() > 0) {

        buffer[i] = Serial2.read();
        delay(5);

        Serial.print(i);
        Serial.print("/");
        Serial.print(buffer[i], HEX);
        Serial.print(" ");

        i++;

        if (buffer[i - 1] == 0xF0) i = 0;
      }
    }

    count++;
    main_fun();

  } else {

    if (millis() / 1000 - start_time > 1) {

      if (active_check) {
        Serial.println("Waiting reply from dispenser");

        digitalWrite(15, HIGH);
        Serial2.write(state_req_1, 1);
        delay(2);
        digitalWrite(15, LOW);
        active_check = false;
      }

      start_time = millis() / 1000;

    } else {
      // Serial.println("i am working");
      active_check = true;
    }
  }

  // Serial.println();
  // delay(10);
}

void manage_msg() {

  if (buffer[i - 1] == 0x71) preset_fun();
  else if (buffer[i - 1] == 0x72) preset_fun_2();
}


void preset_fun() {
  digitalWrite(15, HIGH);
  Serial2.write(auth_1, 1);
  delay(2);
  digitalWrite(15, LOW);
}


void preset_fun_2() {
  digitalWrite(15, HIGH);
  Serial2.write(auth_2, 1);
  delay(2);
  digitalWrite(15, LOW);
}


void main_fun() {

  // Serial.println();
  // Serial.print("count is => ");
  // Serial.println(count);

  delay(100);
  digitalWrite(15, HIGH);
  if (count == 1) Serial2.write(state_req_1, 1);
  else if (count == 2) Serial2.write(state_req_2, 1);
  delay(2);
  digitalWrite(15, LOW);

  Serial.println("send pump state");

  if (count == 2) count = 0;
}


void callback(char* topic, byte* payload, unsigned int length) {
  incommingMessage = "";

  for (int j = 0; j < length; j++) incommingMessage += (char)payload[j];

  // Serial.println();
  // Serial.println("Message arrived [" + String(topic) + "]" + incommingMessage);

  if (String(topic) == String(pumpapprobuf)) {  // pump req appro
    // rxledonoff();
    char pumpapproArray[13];
    incommingMessage.toCharArray(pumpapproArray, incommingMessage.length() + 1);
    Serial.print("Appro  is ");

    Serial.println(pumpapproArray);

    // Serial.println(pumpapproArray[1]);

    charArray[0] = pumpapproArray[0];
    charArray[1] = pumpapproArray[1];

    if (charArray[0] == 0x30 && charArray[1] == 0x31) {
      pumpmqttnum2 = 1;
    } else if (charArray[0] == 0x30 && charArray[1] == 0x32) {
      pumpmqttnum2 = 2;
    } else if (charArray[0] == 0x30 && charArray[1] == 0x33) {
      pumpmqttnum2 = 3;
    } else if (charArray[0] == 0x30 && charArray[1] == 0x34) {
      pumpmqttnum2 = 4;
    } else if (charArray[0] == 0x30 && charArray[1] == 0x35) {
      pumpmqttnum2 = 5;
    } else if (charArray[0] == 0x30 && charArray[1] == 0x36) {
      pumpmqttnum2 = 6;
    } else if (charArray[0] == 0x30 && charArray[1] == 0x37) {
      pumpmqttnum2 = 7;
    } else if (charArray[0] == 0x30 && charArray[1] == 0x38) {
      pumpmqttnum2 = 8;
    } else if (charArray[0] == 0x30 && charArray[1] == 0x39) {
      pumpmqttnum2 = 9;
    } else if (charArray[0] == 0x31 && charArray[1] == 0x30) {
      pumpmqttnum2 = 10;
    } else if (charArray[0] == 0x31 && charArray[1] == 0x31) {
      pumpmqttnum2 = 11;
    } else if (charArray[0] == 0x31 && charArray[1] == 0x32) {
      pumpmqttnum2 = 12;
    } else if (charArray[0] == 0x31 && charArray[1] == 0x33) {
      pumpmqttnum2 = 13;
    } else if (charArray[0] == 0x31 && charArray[1] == 0x34) {
      pumpmqttnum2 = 14;
    } else if (charArray[0] == 0x31 && charArray[1] == 0x35) {
      pumpmqttnum2 = 15;
    } else if (charArray[0] == 0x31 && charArray[1] == 0x36) {
      pumpmqttnum2 = 16;
    } else if (charArray[0] == 0x31 && charArray[1] == 0x37) {
      pumpmqttnum2 = 17;
    } else if (charArray[0] == 0x31 && charArray[1] == 0x38) {
      pumpmqttnum2 = 18;
    } else if (charArray[0] == 0x31 && charArray[1] == 0x39) {
      pumpmqttnum2 = 19;
    } else if (charArray[0] == 0x32 && charArray[1] == 0x30) {
      pumpmqttnum2 = 20;
    } else if (charArray[0] == 0x32 && charArray[1] == 0x31) {
      pumpmqttnum2 = 21;
    } else if (charArray[0] == 0x32 && charArray[1] == 0x32) {
      pumpmqttnum2 = 22;
    } else if (charArray[0] == 0x32 && charArray[1] == 0x33) {
      pumpmqttnum2 = 23;
    } else if (charArray[0] == 0x32 && charArray[1] == 0x34) {
      pumpmqttnum2 = 24;
    } else if (charArray[0] == 0x32 && charArray[1] == 0x35) {
      pumpmqttnum2 = 25;
    } else if (charArray[0] == 0x32 && charArray[1] == 0x36) {
      pumpmqttnum2 = 26;
    } else if (charArray[0] == 0x32 && charArray[1] == 0x37) {
      pumpmqttnum2 = 27;
    } else if (charArray[0] == 0x32 && charArray[1] == 0x38) {
      pumpmqttnum2 = 28;
    } else if (charArray[0] == 0x32 && charArray[1] == 0x39) {
      pumpmqttnum2 = 29;
    } else if (charArray[0] == 0x33 && charArray[1] == 0x30) {
      pumpmqttnum2 = 30;
    } else if (charArray[0] == 0x33 && charArray[1] == 0x31) {
      pumpmqttnum2 = 31;
    } else if (charArray[0] == 0x33 && charArray[1] == 0x32) {
      pumpmqttnum2 = 32;
    }

    if (pumpid1 == pumpmqttnum2) {
      pumpapprocount1 = true;
      // pumpapprocount2 = false;

    } else if (pumpid2 == pumpmqttnum2) {
      pumpapprocount2 = true;
      // pumpapprocount1 = false;
    }

    // if (pumpapproArray[1] == '1') pumpapprocount1 = true;
    // else if (pumpapproArray[1] == '2') pumpapprocount2 = true;

    incommingmsg1 = incommingMessage;
  }

  if (String(topic) == String(pumppresetbuf)) {  // preset change
    // rxledonoff(); // red
    incommingMessage.toCharArray(presetmqttary, incommingMessage.length() + 1);
    Serial.print("preset is ");
    Serial.println(presetmqttary);

    // Serial.print(presetmqttary[0], HEX);
    // Serial.print(presetmqttary[1], HEX);

    charArray[0] = presetmqttary[0];
    charArray[1] = presetmqttary[1];

    pumpmqttnumchange();

    charArray[3] = presetmqttary[2];


    if (presetcount) {

      char priceArray[6];
      char literArray[3];

      if (charArray[3] == 'P') {

        preset_data_arr[2] = 0x89;

        for (int i = 0; i < 6; i++) {
          priceArray[i] = presetmqttary[4 + i];
        }

        int preset_price = atoi(priceArray);

        // Serial.print("Preset price =========================> ");
        // Serial.println(preset_price);

        preset_data_arr[3] = (preset_price >> 24) & 0xff;
        preset_data_arr[4] = (preset_price >> 16) & 0xff;
        preset_data_arr[5] = (preset_price >> 8) & 0xff;
        preset_data_arr[6] = preset_price & 0xff;


      } else if (charArray[3] == 'L') {

        Serial.println(" this is liter preset");
        preset_data_arr[2] = 0x8B;


        for (int i = 0; i < 3; i++) {
          literArray[i] = presetmqttary[4 + i];
        }

        int preset_liter = atoi(literArray);

        Serial.print("Preset liter => ");
        Serial.println(preset_liter);

        preset_liter = preset_liter * 1000;

        preset_data_arr[3] = (preset_liter >> 24) & 0xff;
        preset_data_arr[4] = (preset_liter >> 16) & 0xff;
        preset_data_arr[5] = (preset_liter >> 8) & 0xff;
        preset_data_arr[6] = preset_liter & 0xff;
      }

      preset_check = true;
      lastpresetsend = true;


      presetcount = false;
    }


  }

  // if (String(topic) == String(myreload)) {  // reload change

  //   incommingMessage.toCharArray(reloadArry, incommingMessage.length() + 1);
  //   charArray[0] = reloadArry[0];
  //   charArray[1] = reloadArry[1];

  //   reload_pumpmqttnumchange();

  //   if (charArray[2] == 0x40) {
  //     reload_check_1 = true;
  //   } else if (charArray[2] == 0x41) {
  //     reload_check_2 = true;
  //   }
  // }

  if (String(topic) == String(pricechange)) {  // price change

    pricechangefun();
    // rxledonoff();
  }

  if (String(topic) == device_Id_topic) {
    DynamicJsonDocument doc(4096);  // Adjust the size based on your JSON data size
    DeserializationError error = deserializeJson(doc, incommingMessage);

    if (error) {
      Serial.print(F("JSON parsing failed: "));
      Serial.println(error.c_str());
      return;
    }

    int pumpid1 = 0;
    int pumpid2 = 0;
    int pumpid3 = 0;
    int pumpid4 = 0;
    int pumpid5 = 0;
    int pumpid6 = 0;
    int pumpid7 = 0;
    int pumpid8 = 0;

    devicenum = doc["devicenum"].as<const int>();
    nozzlenum = doc["nozzlenum"].as<const int>();
    pumpid1 = doc["pumpid1"].as<const int>();
    pumpid2 = doc["pumpid2"].as<const int>();
    pumpid3 = doc["pumpid3"].as<const int>();
    pumpid4 = doc["pumpid4"].as<const int>();
    pumpid5 = doc["pumpid5"].as<const int>();
    pumpid6 = doc["pumpid6"].as<const int>();
    pumpid7 = doc["pumpid7"].as<const int>();
    pumpid8 = doc["pumpid8"].as<const int>();

    Serial.println(devicenum);
    Serial.println(nozzlenum);
    Serial.println(pumpid1);
    Serial.println(pumpid2);
    Serial.println(pumpid3);
    Serial.println(pumpid4);
    Serial.println(pumpid5);
    Serial.println(pumpid6);
    Serial.println(pumpid7);
    Serial.println(pumpid8);

    EEPROM.write(101, pumpid1);
    EEPROM.commit();
    EEPROM.write(102, pumpid2);
    EEPROM.commit();
    EEPROM.write(103, pumpid3);
    EEPROM.commit();
    EEPROM.write(104, pumpid4);
    EEPROM.commit();
    EEPROM.write(105, pumpid5);
    EEPROM.commit();
    EEPROM.write(106, pumpid6);
    EEPROM.commit();
    EEPROM.write(107, pumpid7);
    EEPROM.commit();
    EEPROM.write(108, pumpid8);
    EEPROM.commit();
    EEPROM.write(109, devicenum);
    EEPROM.commit();
    EEPROM.write(110, nozzlenum);
    EEPROM.commit();
    Serial.println("yep all save");
  }

  if (String(topic) == String(server_rpy)) {
    incommingMessage.toCharArray(server_rpy_ary, incommingMessage.length() + 1);
    // Serial.println("hey i am working");
    char temp_rp[4];

    for (int i = 0; i < 5; i++) {
      temp_rp[i] = server_rpy_ary[3 + i];
    }

    final_str = "";
    final_str = String(temp_rp);
  }

  if (String(topic) == String(Reset_topic)) {  // esp reset topic
    char resetid[3];
    incommingMessage.toCharArray(resetid, incommingMessage.length() + 1);
    Serial.println(resetid[1]);
    // delay(4000);
    if (atoi(resetid) == devicenum) {
      Serial.println("yep");
      // ESP.restart();
    }
  }
}


void pumpmqttnumchange() {  //change incomming pumpid(0 - 32) from Mqtt to Device pumpid(0 - 2)

  if (charArray[0] == 0x30 && charArray[1] == 0x31) {
    pumpmqttnum = 1;
  } else if (charArray[0] == 0x30 && charArray[1] == 0x32) {
    pumpmqttnum = 2;
  } else if (charArray[0] == 0x30 && charArray[1] == 0x33) {
    pumpmqttnum = 3;
  } else if (charArray[0] == 0x30 && charArray[1] == 0x34) {
    pumpmqttnum = 4;
  } else if (charArray[0] == 0x30 && charArray[1] == 0x35) {
    pumpmqttnum = 5;
  } else if (charArray[0] == 0x30 && charArray[1] == 0x36) {
    pumpmqttnum = 6;
  } else if (charArray[0] == 0x30 && charArray[1] == 0x37) {
    pumpmqttnum = 7;
  } else if (charArray[0] == 0x30 && charArray[1] == 0x38) {
    pumpmqttnum = 8;
  } else if (charArray[0] == 0x30 && charArray[1] == 0x39) {
    pumpmqttnum = 9;
  } else if (charArray[0] == 0x31 && charArray[1] == 0x30) {
    pumpmqttnum = 10;
  } else if (charArray[0] == 0x31 && charArray[1] == 0x31) {
    pumpmqttnum = 11;
  } else if (charArray[0] == 0x31 && charArray[1] == 0x32) {
    pumpmqttnum = 12;
  } else if (charArray[0] == 0x31 && charArray[1] == 0x33) {
    pumpmqttnum = 13;
  } else if (charArray[0] == 0x31 && charArray[1] == 0x34) {
    pumpmqttnum = 14;
  } else if (charArray[0] == 0x31 && charArray[1] == 0x35) {
    pumpmqttnum = 15;
  } else if (charArray[0] == 0x31 && charArray[1] == 0x36) {
    pumpmqttnum = 16;
  } else if (charArray[0] == 0x31 && charArray[1] == 0x37) {
    pumpmqttnum = 17;
  } else if (charArray[0] == 0x31 && charArray[1] == 0x38) {
    pumpmqttnum = 18;
  } else if (charArray[0] == 0x31 && charArray[1] == 0x39) {
    pumpmqttnum = 19;
  } else if (charArray[0] == 0x32 && charArray[1] == 0x30) {
    pumpmqttnum = 20;
  } else if (charArray[0] == 0x32 && charArray[1] == 0x31) {
    pumpmqttnum = 21;
  } else if (charArray[0] == 0x32 && charArray[1] == 0x32) {
    pumpmqttnum = 22;
  } else if (charArray[0] == 0x32 && charArray[1] == 0x33) {
    pumpmqttnum = 23;
  } else if (charArray[0] == 0x32 && charArray[1] == 0x34) {
    pumpmqttnum = 24;
  } else if (charArray[0] == 0x32 && charArray[1] == 0x35) {
    pumpmqttnum = 25;
  } else if (charArray[0] == 0x32 && charArray[1] == 0x36) {
    pumpmqttnum = 26;
  } else if (charArray[0] == 0x32 && charArray[1] == 0x37) {
    pumpmqttnum = 27;
  } else if (charArray[0] == 0x32 && charArray[1] == 0x38) {
    pumpmqttnum = 28;
  } else if (charArray[0] == 0x32 && charArray[1] == 0x39) {
    pumpmqttnum = 29;
  } else if (charArray[0] == 0x33 && charArray[1] == 0x30) {
    pumpmqttnum = 30;
  } else if (charArray[0] == 0x33 && charArray[1] == 0x31) {
    pumpmqttnum = 31;
  } else if (charArray[0] == 0x33 && charArray[1] == 0x32) {
    pumpmqttnum = 32;
  }

  charArray[2] = 0x00;

  Serial.printf("pump mqtt number is %d\n", pumpmqttnum);

  if (pumpid1 == pumpmqttnum) {
    // Serial.println("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
    preset_data_arr[0] = 0x01;
    // charArray[2] = 0x30;
    presetcount = true;

  } else if (pumpid2 == pumpmqttnum) {
    preset_data_arr[0] = 0x02;
    // charArray[2] = 0x31;
    presetcount = true;
  }
  //    else if (pumpid3 == pumpmqttnum) {
  //     charArray[3] = 0x42;

  //   } else if (pumpid4 == pumpmqttnum) {
  //     charArray[3] = 0x30;

  //   } else if (pumpid5 == pumpmqttnum) {
  //     charArray[3] = 0x30;

  //   } else if (pumpid6 == pumpmqttnum) {
  //     charArray[3] = 0x30;

  //   } else if (pumpid7 == pumpmqttnum) {
  //     charArray[3] = 0x30;

  //   } else if (pumpid8 == pumpmqttnum) {
  //     charArray[3] = 0x30;
  //   }
}

void pumpidchange() {  //change incomming pumpid(0 - 8) from device to mqtt pumpid(0 - 32)
  if (Buffer[0] == 0x01) mqttpumpidchange(pumpid1);
  else if (Buffer[0] == 0x02) mqttpumpidchange(pumpid2);
}

void mqttpumpidchange(int pumpid) {
  if (pumpid == 1) {
    mqttdatabuf[0] = 0x30;
    mqttdatabuf[1] = 0x31;
  } else if (pumpid == 2) {
    mqttdatabuf[0] = 0x30;
    mqttdatabuf[1] = 0x32;
  } else if (pumpid == 3) {
    mqttdatabuf[0] = 0x30;
    mqttdatabuf[1] = 0x33;
  } else if (pumpid == 4) {
    mqttdatabuf[0] = 0x30;
    mqttdatabuf[1] = 0x34;
  } else if (pumpid == 5) {
    mqttdatabuf[0] = 0x30;
    mqttdatabuf[1] = 0x35;
  } else if (pumpid == 6) {
    mqttdatabuf[0] = 0x30;
    mqttdatabuf[1] = 0x36;
  } else if (pumpid == 7) {
    mqttdatabuf[0] = 0x30;
    mqttdatabuf[1] = 0x37;
  } else if (pumpid == 8) {
    mqttdatabuf[0] = 0x30;
    mqttdatabuf[1] = 0x38;
  } else if (pumpid == 9) {
    mqttdatabuf[0] = 0x30;
    mqttdatabuf[1] = 0x39;
  } else if (pumpid == 10) {
    mqttdatabuf[0] = 0x31;
    mqttdatabuf[1] = 0x30;
  } else if (pumpid == 11) {
    mqttdatabuf[0] = 0x31;
    mqttdatabuf[1] = 0x31;
  } else if (pumpid == 12) {
    mqttdatabuf[0] = 0x31;
    mqttdatabuf[1] = 0x32;
  } else if (pumpid == 13) {
    mqttdatabuf[0] = 0x31;
    mqttdatabuf[1] = 0x33;
  } else if (pumpid == 14) {
    mqttdatabuf[0] = 0x31;
    mqttdatabuf[1] = 0x34;
  } else if (pumpid == 15) {
    mqttdatabuf[0] = 0x31;
    mqttdatabuf[1] = 0x35;
  } else if (pumpid == 16) {
    mqttdatabuf[0] = 0x31;
    mqttdatabuf[1] = 0x36;
  } else if (pumpid == 17) {
    mqttdatabuf[0] = 0x31;
    mqttdatabuf[1] = 0x37;
  } else if (pumpid == 18) {
    mqttdatabuf[0] = 0x31;
    mqttdatabuf[1] = 0x38;
  } else if (pumpid == 19) {
    mqttdatabuf[0] = 0x31;
    mqttdatabuf[1] = 0x39;
  } else if (pumpid == 20) {
    mqttdatabuf[0] = 0x32;
    mqttdatabuf[1] = 0x30;
  } else if (pumpid == 21) {
    mqttdatabuf[0] = 0x32;
    mqttdatabuf[1] = 0x31;
  } else if (pumpid == 22) {
    mqttdatabuf[0] = 0x32;
    mqttdatabuf[1] = 0x32;
  } else if (pumpid == 23) {
    mqttdatabuf[0] = 0x32;
    mqttdatabuf[1] = 0x33;
  } else if (pumpid == 24) {
    mqttdatabuf[0] = 0x32;
    mqttdatabuf[1] = 0x34;
  } else if (pumpid == 25) {
    mqttdatabuf[0] = 0x32;
    mqttdatabuf[1] = 0x35;
  } else if (pumpid == 26) {
    mqttdatabuf[0] = 0x32;
    mqttdatabuf[1] = 0x36;
  } else if (pumpid == 27) {
    mqttdatabuf[0] = 0x32;
    mqttdatabuf[1] = 0x37;
  } else if (pumpid == 28) {
    mqttdatabuf[0] = 0x32;
    mqttdatabuf[1] = 0x38;
  } else if (pumpid == 29) {
    mqttdatabuf[0] = 0x32;
    mqttdatabuf[1] = 0x39;
  } else if (pumpid == 33) {
    mqttdatabuf[0] = 0x33;
    mqttdatabuf[1] = 0x30;
  } else if (pumpid == 31) {
    mqttdatabuf[0] = 0x33;
    mqttdatabuf[1] = 0x31;
  } else if (pumpid = 32) {
    mqttdatabuf[0] = 0x33;
    mqttdatabuf[1] = 0x32;
  }
}

void initEEPROMdatashow() {
  // for (int j = 0; j < 50; j++) {
  //   ssidBuf[j] = EEPROM.read(j);
  // }
  // for (int j = 50; j < 100; j++) {
  //   passBuf[j - 50] = EEPROM.read(j);
  // }
  // Serial.print(ssidBuf);
  // Serial.print("  ");
  // Serial.println(passBuf);

  pumpid1 = EEPROM.read(101);
  pumpid2 = EEPROM.read(102);
  pumpid3 = EEPROM.read(103);
  pumpid4 = EEPROM.read(104);
  pumpid5 = EEPROM.read(105);
  pumpid6 = EEPROM.read(106);
  pumpid7 = EEPROM.read(107);
  pumpid8 = EEPROM.read(108);

  devicenum = EEPROM.read(109);
  nozzlenum = EEPROM.read(110);

  Serial.print(devicenum);
  Serial.print("  ");
  Serial.print(nozzlenum);
  Serial.print("  ");
  Serial.print(pumpid1);
  Serial.print("  ");
  Serial.print(pumpid2);
  Serial.print("  ");
  Serial.print(pumpid3);
  Serial.print("  ");
  Serial.print(pumpid4);
  Serial.print("  ");
  Serial.print(pumpid5);
  Serial.print("  ");
  Serial.print(pumpid6);
  Serial.print("  ");
  Serial.print(pumpid7);
  Serial.print("  ");
  Serial.print(pumpid8);
  Serial.println("  ");
}

void EEPROMinit() {

  if (devicenum == 1) {
    //pumpreqbuf

    pumpreqbuf[21] = '1';
    pumpapprobuf[20] = '1';
    ppfinal[20] = '1';
    pplive[23] = '1';
    devicebuf[0] = '1';
    activebuf[21] = '1';
    pricereqbuf[23] = '1';
    myreload[27] = '1';
    pumpfinalreloadbuf[21] = '1';
    // rdybuf[20] = '1';
    device_Id_topic[38] = '1';
  } else if (devicenum == 2) {
    pumpreqbuf[21] = '2';
    pumpapprobuf[20] = '2';
    ppfinal[20] = '2';
    pplive[23] = '2';
    devicebuf[0] = '2';
    activebuf[21] = '2';
    pricereqbuf[23] = '2';
    myreload[27] = '2';
    pumpfinalreloadbuf[21] = '2';
    // rdybuf[20] = '2';
    device_Id_topic[38] = '2';
  } else if (devicenum == 3) {
    pumpreqbuf[21] = '3';
    pumpapprobuf[20] = '3';
    ppfinal[20] = '3';
    pplive[23] = '3';
    devicebuf[0] = '3';
    activebuf[21] = '3';
    pricereqbuf[23] = '3';
    myreload[27] = '3';
    pumpfinalreloadbuf[21] = '3';
    // rdybuf[20] = '3';
    device_Id_topic[38] = '3';
  } else if (devicenum == 4) {
    pumpreqbuf[21] = '4';
    pumpapprobuf[20] = '4';
    ppfinal[20] = '4';
    pplive[23] = '4';
    devicebuf[0] = '4';
    activebuf[21] = '4';
    pricereqbuf[23] = '4';
    myreload[27] = '4';
    pumpfinalreloadbuf[21] = '4';
    // rdybuf[20] = '4';
    device_Id_topic[38] = '4';
  } else if (devicenum == 5) {
    pumpreqbuf[21] = '5';
    pumpapprobuf[20] = '5';
    ppfinal[20] = '5';
    pplive[23] = '5';
    devicebuf[0] = '5';
    activebuf[21] = '5';
    pricereqbuf[23] = '5';
    myreload[27] = '5';
    pumpfinalreloadbuf[21] = '5';
    // rdybuf[20] = '5';
    device_Id_topic[38] = '5';
  } else if (devicenum == 6) {
    pumpreqbuf[21] = '6';
    pumpapprobuf[20] = '6';
    ppfinal[20] = '6';
    pplive[23] = '6';
    devicebuf[0] = '6';
    activebuf[21] = '6';
    pricereqbuf[23] = '6';
    myreload[27] = '6';
    pumpfinalreloadbuf[21] = '6';
    // rdybuf[20] = '6';
    device_Id_topic[38] = '6';
  } else if (devicenum == 7) {
    pumpreqbuf[21] = '7';
    pumpapprobuf[20] = '7';
    ppfinal[20] = '7';
    pplive[23] = '7';
    devicebuf[0] = '7';
    activebuf[21] = '7';
    pricereqbuf[23] = '7';
    myreload[27] = '7';
    pumpfinalreloadbuf[21] = '7';
    // rdybuf[20] = '7';
    device_Id_topic[38] = '7';
  } else if (devicenum == 8) {
    pumpreqbuf[21] = '8';
    pumpapprobuf[20] = '8';
    ppfinal[20] = '8';
    pplive[23] = '8';
    devicebuf[0] = '8';
    activebuf[21] = '8';
    pricereqbuf[23] = '8';
    myreload[27] = '8';
    pumpfinalreloadbuf[21] = '8';
    // rdybuf[20] = '8';
    device_Id_topic[38] = '8';
  }
}


void initWiFi() {

  // for (int i = 0; i < 50; i++) {
  //   ssidtemp[i] = EEPROM.read(i);
  //   // Serial.print(getDataEEPROM(i));
  // }
  // for (int i = 50; i < 100; i++) {
  //   passtemp[i - 50] = EEPROM.read(i);
  //   // Serial.print(getDataEEPROM(i));
  // }
   
  WiFi.hostname("myesp");
  // WiFi.begin(ssidtemp, passtemp);
  WiFi.begin("POS_Server", "asdffdsa");
  Serial.print("Connecting to WiFi ..");
  int wifitrytime = 0;
  while (WiFi.status() != WL_CONNECTED && wifitrytime != 20) {

    Serial.print('.');
    digitalWrite(wifiled, HIGH);
    delay(500);
    digitalWrite(wifiled, LOW);
    delay(500);
    wifitrytime++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("You are connected in ");
    Serial.println(WiFi.localIP());
    // digitalWrite(wifiled, HIGH);
  }
}

void reconnect() {
  if (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";  // Create a random client ID
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("connected");

      client.subscribe("Connect");                    // subscribe the topics here
      client.publish("connect", "Device1Connected");  // subscribe the topics here
      client.subscribe("detpos/#");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");  // Wait 5 seconds before retrying
      delay(1000);
    }
  }
}
