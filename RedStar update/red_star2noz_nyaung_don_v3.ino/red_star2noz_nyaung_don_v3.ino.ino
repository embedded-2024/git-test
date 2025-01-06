/* 
red star final production
added mqtt final send twice, reload
update totalizer count
Installed Nyaung Don
*/

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
String incommingmsg1;

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

int addresscount = 1;

unsigned char read_state_1[4] = { 0x01, 0x03, 0x12, 0xEA };
unsigned char read_state_2[4] = { 0x02, 0x03, 0x12, 0xE9 };
unsigned char read_state_3[4] = { 0x01, 0x03, 0x12, 0xE8 };
unsigned char read_state_4[4] = { 0x02, 0x03, 0x12, 0xE7 };

unsigned char read_price_1[4] = { 0x01, 0x03, 0x8C, 0x70 };
unsigned char read_price_2[4] = { 0x02, 0x03, 0x8C, 0x6F };
unsigned char read_price_3[4] = { 0x01, 0x03, 0x8C, 0x6E };
unsigned char read_price_4[4] = { 0x02, 0x03, 0x8C, 0x6D };

unsigned char read_total_1[4] = { 0x01, 0x03, 0x8E, 0x6E };
unsigned char read_total_2[4] = { 0x02, 0x03, 0x8E, 0x6D };

unsigned char send_app1[4] = { 0x01, 0x03, 0x08, 0xF4 };
unsigned char send_app2[4] = { 0x02, 0x03, 0x08, 0xF3 };
unsigned char send_fuel1[4] = { 0x01, 0x03, 0x8F, 0x6D };
unsigned char send_fuel2[4] = { 0x02, 0x03, 0x8F, 0x6C };

#define wifiled 33
#define powerled 32
#define TXled 27
#define RXled 26

String pump2_status = "ide";
String pump1_status = "ide";

unsigned char Buffer[30];
unsigned char Serial_Buffer[30];

char volume_chr[4];
char amount_chr[4];

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

unsigned char preset_data_arr[8] = { 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
unsigned char price_change_arr[8] = { 0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00 };
unsigned char read_price[4] = { 0x00, 0x03, 0x8C, 0x00 };

unsigned char ppp[8] = { 0x01, 0x07, 0x80, 0x00, 0x00, 0x04, 0xD2, 0xA2 };


bool presetcount = false;

bool lastpresetsend = false;

long datatime1 = 0;
long datatime2 = 0;

String final_str;                  //ak
unsigned long previousMillis = 0;  //ak
unsigned long currentMillis = 0;   //ak
char server_rpy_ary[7];            //ak
char err_buffer[9];                //ak
bool preset_check = false;
bool reload_check_1 = false;
bool reload_check_2 = false;
char reloadArry[3];

void setup() {
  Serial.begin(115200);
  Serial2.begin(19200, SERIAL_8N1);

  EEPROM.begin(512);

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

  delay(5000);

  // send_read_state();
}

void EEPROMWrite() {
  EEPROM.write(109, 3);  //device id
  EEPROM.commit();
  EEPROM.write(110, 2);  // nozzle count
  EEPROM.commit();
  EEPROM.write(101, 5);  // nozzle id
  EEPROM.commit();
  EEPROM.write(102, 6);
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

  if (WiFi.status() == WL_CONNECTED) {
    // digitalWrite(wifiled, HIGH);
    if (!client.connected()) {
      Serial.println("Cloud disconnect");
      digitalWrite(wifiled, LOW);
      reconnect();

    } else {
      // Serial.println("Connected to the Cloud");
      digitalWrite(wifiled, HIGH);
      main_fun();
    }

  } else {
    Serial.println("Not Connected");
    initWiFi();
  }
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  main_fun();
}



void main_fun() {

  delay(5);
  if (Serial2.available()) {
    datatime1 = millis() / 1000;

    Buffer[bufcount] = Serial2.read();
    delay(5);
    Serial.print(bufcount);
    Serial.print("//");
    Serial.print(Buffer[bufcount], HEX);
    Serial.print(" ");
    bufcount++;

    if (addresscount > 2) addresscount = 1;

    if (Buffer[bufcount - 2] == 0x12 && (Buffer[bufcount - 1] == 0x57 || Buffer[bufcount - 1] == 0x56 || Buffer[bufcount - 1] == 0x53 || Buffer[bufcount - 1] == 0x52)) {
      // if (Buffer[bufcount - 2] == 0x12 && (Buffer[bufcount - 1] == 0x57 || Buffer[bufcount - 1] == 0x56 || Buffer[bufcount - 1] == 0x53)) {

      for (int i = 0; i < 8; i++) {
        Buffer[bufcount] = Serial2.read();
        delay(5);
        Serial.print(bufcount);
        Serial.print("/");
        Serial.print(Buffer[bufcount], HEX);
        Serial.print(" ");
        bufcount++;
      }
      bufcount = 0;

      // send_read_state();

      if (Buffer[0] == 0x01 && pump1_status == "fuel" || reload_check_1) {
        addresscount = 1;
        send_fuel_fun();
        Serial.println("Finallllllllllllllllllllllllllllllllllllllllllllll     1");

        send_read_price();
        send_read_total();
        //// send final
        mqttfinal();

        pump1_status = "ide";
        reload_check_1 = false;

      } else if (Buffer[0] == 0x02 && pump2_status == "fuel" || reload_check_2) {
        addresscount = 2;
        send_fuel_fun();
        Serial.println("Finalllllllllllllllllllllllllllllllllllllllllllllll     2");
        send_read_price();
        //// send final
        send_read_total();
        mqttfinal();
        pump2_status = "ide";
        reload_check_2 = false;

      } else {


        // if (reload_check) {
        //   addresscount = 1;
        //   send_fuel_fun();
        //   Serial.println("Finallllllllllllllllllllllllllllllllllllllllllllll     reloading");

        //   send_read_price();
        //   send_read_total();
        //   //// send final
        //   mqttfinal();
        //   reload_check = false;
        // }
        send_read_state();
      }

      // else {
      //   //last  add
      //   if (lastpresetsend) {
      //     presetfun();
      //     lastpresetsend = false;
      //   } else
      //     send_read_state();
      // }

      // addresscount++;
    } else if (Buffer[bufcount - 2] == 0x12 && (Buffer[bufcount - 1] == 0x77 || Buffer[bufcount - 1] == 0x76 || Buffer[bufcount - 1] == 0x72 || Buffer[bufcount - 1] == 0x73)) {
      // if (Buffer[bufcount - 2] == 0x12 && (Buffer[bufcount - 1] == 0x77 || Buffer[bufcount - 1] == 0x76)) {
      for (int i = 0; i < 8; i++) {
        Buffer[bufcount] = Serial2.read();
        delay(5);
        Serial.print(bufcount);
        Serial.print("/ ");
        Serial.print(Buffer[bufcount], HEX);
        Serial.print(" ");
        bufcount++;
      }

      bufcount = 0;


      if (preset_check) {

        //last  add
        if (lastpresetsend) {
          presetfun();
          lastpresetsend = false;
          preset_check = false;

        } else {

          send_read_state();
          preset_check = false;
        }

      } else {

        if (pumpapprocount1 == true && Buffer[0] == 0x01) {
          send_app_fun();
          pumpapprocount1 = false;
        } else if (pumpapprocount2 && Buffer[0] == 0x02) {
          send_app_fun();
          pumpapprocount2 = false;
        } else send_read_state();
        // addresscount++;
        //pump req mqtt
        for (int y = 0; y < 50; y++) {
          mqttdatabuf[y] = 0;
        }

        mqttdatabuf[2] = 'p';
        mqttdatabuf[3] = 'e';
        mqttdatabuf[4] = 'r';
        mqttdatabuf[5] = 'm';
        mqttdatabuf[6] = 'i';
        mqttdatabuf[7] = 't';
        // mqttdatabuf[0] = '0';
        // if (Buffer[0] == 0x01) mqttdatabuf[1] = '1';
        // else if (Buffer[0] == 0x02) mqttdatabuf[1] = '2';
        pumpidchange();
        client.publish(pumpreqbuf, mqttdatabuf);

        preset_check = false;
        // txledonoff();
      }


    } else if (Buffer[bufcount - 2] == 0x12 && (Buffer[bufcount - 1] == 0x5F || Buffer[bufcount - 1] == 0x5B)) {
      for (int i = 0; i < 8; i++) {
        Buffer[bufcount] = Serial2.read();
        delay(5);
        Serial.print(bufcount);
        Serial.print("/");
        Serial.print(Buffer[bufcount], HEX);
        Serial.print(" ");
        bufcount++;
      }

      bufcount = 0;

      // Serial.println("i am working");  // del

      send_fuel_fun();

    } else if (Buffer[0] == 0xCD && Buffer[1] == 0x6C) {

      // delay(1000);
      for (int i = 0; i < 10; i++) {
        Buffer[bufcount] = Serial2.read();
        delay(5);
        Serial.print(bufcount);
        Serial.print("/");
        Serial.print(Buffer[bufcount], HEX);
        Serial.print(" ");
        bufcount++;
      }
      bufcount = 0;
      Serial.println("read state again");

      if (addresscount == 1) addresscount = 2;
      else if (addresscount == 2) addresscount = 1;

      send_read_state();
      // delay(80);
    }
  }
  //lateset add
  else {
    datatime2 = millis() / 1000;

    if ((datatime2 - datatime1) > 2) {

      addresscount = 1;
      send_read_state();
      datatime1 = datatime2;
    }
  }
}


void send_read_state() {

  if (addresscount > 2) addresscount = 1;

  delay(50);

  if (addresscount == 1) Serial2.write(read_state_1, 4);
  else if (addresscount == 2) Serial2.write(read_state_2, 4);
  else if (addresscount == 3) Serial2.write(read_state_3, 4);
  else if (addresscount == 4) Serial2.write(read_state_4, 4);

  delay(10);
  Serial.print("Send Read State");
  Serial.println(addresscount);

  //last add
  addresscount++;
}

void send_read_price() {

  while (!Serial2.available()) {
    delay(5);
  }

  // for (int i = 0; i <5; i++) {
  bufcount = 0;
  while (Serial2.available()) {

    Buffer[bufcount] = Serial2.read();
    delay(5);
    Serial.print(bufcount);
    Serial.print("/");
    Serial.print(Buffer[bufcount], HEX);
    Serial.print(" ");
    bufcount++;
  }
  bufcount = 0;

  if (Buffer[0] == 0x01) Serial2.write(read_price_1, 4);
  else if (Buffer[0] == 0x02) Serial2.write(read_price_2, 4);
  else if (Buffer[0] == 0x03) Serial2.write(read_price_3, 4);
  else if (Buffer[0] == 0x04) Serial2.write(read_price_4, 4);

  delay(10);
  Serial.print("Send Read Price");
  Serial.println(Buffer[0]);

  while (!Serial2.available()) {
    delay(5);
  }

  // for (int i = 0; i <5; i++) {
  bufcount = 0;
  while (Serial2.available()) {

    Buffer[bufcount] = Serial2.read();
    delay(5);
    Serial.print(bufcount);
    Serial.print("/");
    Serial.print(Buffer[bufcount], HEX);
    Serial.print(" ");
    bufcount++;
  }
  bufcount = 0;


  price_state = (Buffer[4] << 24 | Buffer[5] << 16) | (Buffer[6] << 8) | Buffer[7];
  Serial.print("price is => ");
  Serial.println(price_state);

  send_read_state();
}


void send_read_total() {

  while (!Serial2.available()) {
    delay(5);
  }

  // for (int i = 0; i <5; i++) {
  bufcount = 0;
  while (Serial2.available()) {

    Buffer[bufcount] = Serial2.read();
    delay(5);
    Serial.print(bufcount);
    Serial.print("/");
    Serial.print(Buffer[bufcount], HEX);
    Serial.print(" ");
    bufcount++;
  }
  bufcount = 0;

  if (addresscount > 2) addresscount = 1;

  if (addresscount == 1) Serial2.write(read_total_1, 4);
  else if (addresscount == 2) Serial2.write(read_total_2, 4);
  // else if (addresscount == 3) Serial2.write(read_total_3, 4);
  // else if (addresscount == 4) Serial2.write(read_total_4, 4);

  delay(10);
  Serial.print("Send Read Total ");
  Serial.println(addresscount);

  while (!Serial2.available()) {
    delay(5);
  }

  // for (int i = 0; i <5; i++) {
  bufcount = 0;
  while (Serial2.available()) {

    Buffer[bufcount] = Serial2.read();
    delay(5);
    Serial.print(bufcount);
    Serial.print("/");
    Serial.print(Buffer[bufcount], HEX);
    Serial.print(" ");
    bufcount++;
  }
  bufcount = 0;

  total_state = hexArrayToString(Buffer, sizeof(Buffer));
  Serial.println("your result is");
  Serial.println(total_state);
  total_amount = hexArrayToString1(Buffer, sizeof(Buffer));
  Serial.println("your result is");
  Serial.println(total_amount);
  send_read_state();
}

String hexArrayToString(byte hexArray[], int size) {
  String result = "";
  bool firsttime = true;
  for (int i = 9; i > 3; i--) {

    if (hexArray[i] == 0x00 && firsttime == true) continue;
    else firsttime = false; 

    if(String(hexArray[i], HEX).length()<2 && String(hexArray[i], HEX)!= "0") result += "0";

    result += String(hexArray[i], HEX);

    if(String(hexArray[i], HEX)== "0") result += "0";

  }
  return result;
}

String hexArrayToString1(byte hexArray[], int size) {
  String result = "";
  bool firsttime = true;
  for (int i = 15; i > 9; i--) {

    if (hexArray[i] == 0x00 && firsttime == true) continue;
    else firsttime = false; 

    if(String(hexArray[i], HEX).length()<2 && String(hexArray[i], HEX)!= "0") result += "0";

    result += String(hexArray[i], HEX);

    if(String(hexArray[i], HEX)== "0") result += "0";

  }
  return result;
}

void send_app_fun() {

  if (pumpapprocount1) {
    Serial.println("Send approve1 ");
    Serial2.write(send_app1, 4);
  } else if (pumpapprocount2) {
    Serial.println("Send approve2 ");
    Serial2.write(send_app2, 4);
  }

  delay(80);

  // while (!Serial2.available()) {
  //   delay(5);
  // }

  for (int i = 0; i < 5; i++) {
    Buffer[bufcount] = Serial2.read();
    delay(5);
    Serial.print(bufcount);
    Serial.print("/");
    Serial.print(Buffer[bufcount], HEX);
    Serial.print(" ");
    bufcount++;
  }
  bufcount = 0;

  if (Buffer[1] == 0x04 && Buffer[2] == 0x08) {
    Serial.println("app motor start");
  } else send_appro_again();

  if (Buffer[0] == 0x01) pump1_status = "fuel";
  else if (Buffer[0] == 0x02) pump2_status = "fuel";

  send_read_state();
}

void send_appro_again() {

  if (pumpapprocount1) {
    Serial.println("Send approve1 again");
    Serial2.write(send_app1, 4);
  } else if (pumpapprocount2) {
    Serial.println("Send approve2 again");
    Serial2.write(send_app2, 4);
  }

  delay(80);

  // while (!Serial2.available()) {
  //   delay(5);
  // }

  for (int i = 0; i < 5; i++) {
    Buffer[bufcount] = Serial2.read();
    delay(5);
    Serial.print(bufcount);
    Serial.print("/");
    Serial.print(Buffer[bufcount], HEX);
    Serial.print(" ");
    bufcount++;
  }
  bufcount = 0;
}

void send_preset_fun() {

  Serial.println("Send preset ");
  Serial2.write(preset_data_arr, 8);
  delay(80);


  // while (!Serial2.available()) {
  //   delay(5);
  //   Serial.println(".");
  // }

  for (int i = 0; i < 5; i++) {

    Buffer[bufcount] = Serial2.read();
    delay(5);
    Serial.print(bufcount);
    Serial.print("/");
    Serial.print(Buffer[bufcount], HEX);
    Serial.print(" ");
    bufcount++;
  }
  bufcount = 0;

  if (Buffer[1] == 0x04 && Buffer[2] == 0x89) {
    Serial.println("preset moter start");
  } else send_preset_again();


  if (Buffer[0] == 0x01) pump1_status = "fuel";
  else if (Buffer[0] == 0x02) pump2_status = "fuel";

  delay(100);
  send_read_state();
}

void send_preset_again() {

  Serial.println("Send preset again");
  Serial2.write(preset_data_arr, 8);
  delay(80);


  // while (!Serial2.available()) {
  //   delay(5);
  //   Serial.println(".");
  // }

  for (int i = 0; i < 5; i++) {

    Buffer[bufcount] = Serial2.read();
    delay(5);
    Serial.print(bufcount);
    Serial.print("/");
    Serial.print(Buffer[bufcount], HEX);
    Serial.print(" ");
    bufcount++;
  }
  bufcount = 0;
}


void send_fuel_fun() {
  // pump2_status = "fuel";

  if (Buffer[0] == 0x01) {
    Serial.println("Send fuel 1");
    Serial2.write(send_fuel1, 4);
  } else if (Buffer[0] == 0x02) {
    Serial2.write(send_fuel2, 4);
    Serial.println("Send fuel 2");
  }


  while (!Serial2.available()) {
    delay(5);
  }

  // for (int i = 0; i <5; i++) {
  bufcount = 0;
  while (Serial2.available()) {

    Buffer[bufcount] = Serial2.read();
    delay(5);
    Serial.print(bufcount);
    Serial.print("/");
    Serial.print(Buffer[bufcount], HEX);
    Serial.print(" ");
    bufcount++;
  }
  bufcount = 0;

  for (int i = 0; i < 4; i++) {
    amount_chr[i] = Buffer[4 + i];
  }

  for (int i = 0; i < 4; i++) {
    volume_chr[i] = Buffer[8 + i];
  }

  price = (amount_chr[0] << 24 | amount_chr[1] << 16) | (amount_chr[2] << 8) | amount_chr[3];
  Serial.print("price is => ");
  Serial.print(price);

  Serial.print("     and");

  liter = (volume_chr[0] << 24 | volume_chr[1] << 16) | (volume_chr[2] << 8) | volume_chr[3];
  Serial.print("liter is => ");
  Serial.println(liter);

  //// mqtt pp send
  if (!reload_check_2 && !reload_check_1) mqttpplive();

  send_read_state();
}


void mqttpplive() {

  // liter = "11211";
  // price = "1000";

  for (int y = 0; y < 50; y++) {
    mqttdatabuf[y] = 0;
  }

  int arycount = 2;

  pumpidchange();


  // mqttdatabuf[1] = '2';

  // pumpidchange
  // pumpidchange();
  // if (Buffer[0] == 0x01) mqttdatabuf[1] = '1';
  // else if (Buffer[0] == 0x02) mqttdatabuf[1] = '2';


  mqttdatabuf[2] = 'L';
  arycount++;


  char liter_ary[11];
  char price_ary[11];

  for (int y = 0; y < 11; y++) {
    liter_ary[y] = 0;
    price_ary[y] = 0;
  }

  liter.toCharArray(liter_ary, liter.length() + 1);
  price.toCharArray(price_ary, price.length() + 1);
  // Serial.println(liter.length());

  int lengthcout = 0;
  if (liter.length() < 4) {
    //
    mqttdatabuf[arycount++] = '.';
    for (int y = 0; y < liter.length(); y++) {
      mqttdatabuf[arycount++] = liter_ary[y];
    }
  } else {
    for (int y = 0; y < liter.length() - 3; y++) {
      mqttdatabuf[arycount++] = liter_ary[y];
      lengthcout++;
    }
    mqttdatabuf[arycount++] = '.';
    for (int y = 0; y < liter.length() - lengthcout; y++) {
      mqttdatabuf[arycount++] = liter_ary[y + lengthcout];
    }
  }

  mqttdatabuf[arycount++] = 'P';
  for (int y = 0; y < price.length(); y++) {
    mqttdatabuf[arycount++] = price_ary[y];
  }


  for (int y = 0; y < 50; y++) {
    Serial.print(mqttdatabuf[y]);
    Serial.print(" ");
  }


  client.publish(pplive, mqttdatabuf);

  // txledonoff();
}

void mqttfinal() {


  for (int y = 0; y < 50; y++) {
    mqttdatabuf[y] = 0;
  }

  int arycount = 2;

  // mqttdatabuf[arycount++] = '0';
  pumpidchange();

  // // mqttdatabuf[arycount++] = '2';
  // if (Buffer[0] == 0x01) mqttdatabuf[arycount++] = '1';
  // else if (Buffer[0] == 0x02) mqttdatabuf[arycount++] = '2';

  mqttdatabuf[arycount++] = 'S';


  char liter_ary[11];
  char price_ary[11];
  char price_state_ary[11];
  char total_ary[11];
  char total_amount_ary[11];

  for (int y = 0; y < 11; y++) {
    liter_ary[y] = 0;
    price_ary[y] = 0;
    price_state_ary[y] = 0;
    total_amount_ary[y] = 0;
  }

  liter.toCharArray(liter_ary, liter.length() + 1);
  price.toCharArray(price_ary, price.length() + 1);
  price_state.toCharArray(price_state_ary, price_state.length() + 1);
  total_state.toCharArray(total_ary, total_state.length() + 1);
  total_amount.toCharArray(total_amount_ary, total_amount.length() + 1);

  for (int y = 0; y < price_state.length(); y++) {
    mqttdatabuf[arycount++] = price_state_ary[y];
  }

  mqttdatabuf[arycount++] = 'L';


  Serial.println(liter.length());

  int lengthcout = 0;
  if (liter.length() < 4) {
    //
    mqttdatabuf[arycount++] = '.';
    for (int y = 0; y < liter.length(); y++) {
      mqttdatabuf[arycount++] = liter_ary[y];
    }
  } else {
    for (int y = 0; y < liter.length() - 3; y++) {
      mqttdatabuf[arycount++] = liter_ary[y];
      lengthcout++;
    }
    mqttdatabuf[arycount++] = '.';
    for (int y = 0; y < liter.length() - lengthcout; y++) {
      mqttdatabuf[arycount++] = liter_ary[y + lengthcout];
    }
  }

  mqttdatabuf[arycount++] = 'P';
  for (int y = 0; y < price.length(); y++) {
    mqttdatabuf[arycount++] = price_ary[y];
  }


  mqttdatabuf[arycount++] = 'T';


  Serial.print("before arr count is => ");   // ak add
  Serial.println(arycount);

  int y = total_state.length() - 3;

  for ( int i = 0 ; i < total_state.length(); i++) {

    if ( i == y ) {
      mqttdatabuf[arycount++] = '.';
       
    }
    mqttdatabuf[arycount++] = total_ary[i];
  }

  Serial.print("after arr count is => ");
  Serial.println(arycount);  

  // for (int y = total_state.length() - 1; y >= 0; y--) {
  //   if (y == 6) {
  //     mqttdatabuf[arycount + y] = '.';
  //     arycount--;
  //     Serial.println("ur is ");
  //     Serial.println(arycount + y);
  //   }
  //   Serial.print(total_ary[y]);
  //   mqttdatabuf[arycount + y] = total_ary[y];
  // }

   

  // arycount = arycount + total_state.length() + 1;

  mqttdatabuf[arycount] = 'A';
  arycount++;

  for (int y = total_amount.length() - 1; y >= 0; y--) {
    mqttdatabuf[arycount + y] = total_amount_ary[y];
  }

  for (int y = 0; y < 50; y++) {
    Serial.print(mqttdatabuf[y]);
    Serial.print(" ");
  }

  if (!reload_check_2 && !reload_check_1) {
    client.publish(ppfinal, mqttdatabuf);  /// akkkkk

    for (int i = 0; i < 10; i++) {
      err_buffer[i] = 0;
    }

    err_buffer[0] = mqttdatabuf[0];
    err_buffer[1] = mqttdatabuf[1];
    err_buffer[2] = '/';
    err_buffer[3] = 'e';
    err_buffer[4] = 'r';
    err_buffer[5] = 'r';
    err_buffer[6] = 'o';
    err_buffer[7] = 'r';

    previousMillis = millis() / 1000;
    int waitcount = 0;

    while (waitcount < 2) {


      if (mqttdatabuf[0] == server_rpy_ary[0] && mqttdatabuf[1] == server_rpy_ary[1] && final_str == "D1S1") {
        Serial.println("Bye ....................................");
        final_str = "";
        break;
      }

      if (millis() / 1000 - previousMillis >= 13) {

        if (waitcount < 1) {
          client.publish(ppfinal, mqttdatabuf);
        }

        previousMillis = millis() / 1000;
        waitcount++;
      }

      client.loop();
    }

    if (waitcount == 2) client.publish(server_err, err_buffer);
    // txledonoff();
  } else {

    client.publish(pumpfinalreloadbuf, mqttdatabuf);
  }
}


void callback(char* topic, byte* payload, unsigned int length) {
  incommingMessage = "";

  for (int j = 0; j < length; j++) incommingMessage += (char)payload[j];

  Serial.println();
  Serial.println("Message arrived [" + String(topic) + "]" + incommingMessage);

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

    if (pumpid1 == pumpmqttnum) {
      pumpapprocount1 = true;

    } else if (pumpid2 == pumpmqttnum) {
      pumpapprocount2 = true;
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

    preset_check = true;

    if (presetcount) {
      lastpresetsend = true;
      presetcount = false;
    }
  }

  if (String(topic) == String(myreload)) {  // reload change

    incommingMessage.toCharArray(reloadArry, incommingMessage.length() + 1);
    charArray[0] = reloadArry[0];
    charArray[1] = reloadArry[1];

    reload_pumpmqttnumchange();

    if (charArray[2] == 0x40) {
      reload_check_1 = true;
    } else if (charArray[2] == 0x41) {
      reload_check_2 = true;
    }
  }

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
      ESP.restart();
    }
  }
}

void reload_pumpmqttnumchange() {  // for reload mqtt num change

  Serial.println("reload mqtt num change");

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

  if (pumpid1 == pumpmqttnum) {
    charArray[2] = 0x40;

  } else if (pumpid2 == pumpmqttnum) {
    charArray[2] = 0x41;
  }
  //  else if (pumpid3 == pumpmqttnum) {
  //   charArray[3] = 0x42;

  // } else if (pumpid4 == pumpmqttnum) {
  //   charArray[3] = 0x30;

  // } else if (pumpid5 == pumpmqttnum) {
  //   charArray[3] = 0x30;

  // } else if (pumpid6 == pumpmqttnum) {
  //   charArray[3] = 0x30;

  // } else if (pumpid7 == pumpmqttnum) {
  //   charArray[3] = 0x30;

  // } else if (pumpid8 == pumpmqttnum) {
  //   charArray[3] = 0x30;

  // }
}

void pricechangefun() {

  incommingMessage.toCharArray(pricechangeary, incommingMessage.length() + 1);

  Serial.print("pc is ");
  Serial.println(pricechangeary);

  charArray[0] = pricechangeary[0];
  charArray[1] = pricechangeary[1];

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
  bool price1 = false;
  bool price2 = false;

  if (pumpid1 == pumpmqttnum) {
    price1 = true;
    price_change_arr[0] = 0x01;

  } else if (pumpid2 == pumpmqttnum) {
    price2 = true;
    price_change_arr[0] = 0x02;
  }

  // if (charArray[2] == 0x31) price_change_arr[0] = 0x01;
  // else if (charArray[2] == 0x32) price_change_arr[0] = 0x02;

  char pricechangeArr[4];

  for (int j = 0; j < 4; j++) {
    pricechangeArr[j] = pricechangeary[2 + j];
  }

  int price = atoi(pricechangeArr);

  Serial.print("Price change => ");
  Serial.println(price);



  price_change_arr[3] = (price >> 24) & 0xff;
  price_change_arr[4] = (price >> 16) & 0xff;
  price_change_arr[5] = (price >> 8) & 0xff;
  price_change_arr[6] = price & 0xff;



  size_t length = (sizeof(price_change_arr) / sizeof(price_change_arr[0])) - 1;
  Serial.print("Price change lenght is => ");
  Serial.println(length);

  byte OPP = calculate_checksum(price_change_arr, length);
  Serial.print("OPP check sun in price change is => ");
  Serial.println(OPP, HEX);

  price_change_arr[7] = OPP;

  for (int i = 0; i < 8; i++) {
    Serial.print(price_change_arr[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  if (price1) {
    Serial2.write(price_change_arr, 8);
    delay(100);
    Serial.println("Changed1 11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111");
  } else if (price2) {
    Serial2.write(price_change_arr, 8);
    delay(100);
    Serial.println("Changed222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222");
  }

  for (int i = 0; i < 5; i++) {

    Buffer[bufcount] = Serial2.read();
    delay(5);
    Serial.print(bufcount);
    Serial.print("/");
    Serial.print(Buffer[bufcount], HEX);
    Serial.print(" ");
    bufcount++;
  }
  bufcount = 0;


  delay(100);

  send_read_state();
}


void presetfun() {  // pump preset

  char priceArray[6];
  char literArray[3];

  if (charArray[2] == 0x31) preset_data_arr[0] = 0x01;
  else if (charArray[2] == 0x32) preset_data_arr[0] = 0x02;

  if (charArray[3] == 'P') {
    preset_data_arr[2] = 0x89;

    for (int i = 0; i < 6; i++) {
      priceArray[i] = presetmqttary[4 + i];
    }

    int preset_price = atoi(priceArray);

    // Serial.print("Preset price => ");
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

  size_t length = (sizeof(preset_data_arr) / sizeof(preset_data_arr[0])) - 1;
  // Serial.print("preset Array lenght is => ");
  // Serial.println(length);

  byte OPP = calculate_checksum(preset_data_arr, length);
  Serial.print("OPP check sun is => ");
  Serial.println(OPP, HEX);

  preset_data_arr[7] = OPP;

  for (int i = 0; i < 8; i++) {
    Serial.print(preset_data_arr[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  send_preset_fun();
}

byte calculate_checksum(byte* sequence, int length) {
  byte checksum = 0x00;
  for (int i = 0; i < length; i++) {
    checksum += sequence[i];  // Sum each byte
  }
  return ~checksum + 1;  // Return two's complement of the sum
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

  if (pumpid1 == pumpmqttnum) {
    preset_data_arr[0] = 0x01;
    presetcount = true;

  } else if (pumpid2 == pumpmqttnum) {
    preset_data_arr[0] = 0x02;
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
  // Serial.print(ssidtemp);
  // Serial.print("  ");
  // Serial.println(passtemp);
  // Serial.println(WiFi.macAddress());
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
