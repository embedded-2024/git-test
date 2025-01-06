/*
prime dongle firmware final production
added mqtt final twice
Installed Kywetatsone
*/

#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <EEPROM.h>
#include <ArduinoJson.h>

const char* mqtt_server = "192.168.0.100";

// const char* mqtt_server = "192.168.100.95";

const char* mqtt_username = "detpos";
const char* mqtt_password = "asdffdsa";

const int mqtt_port = 1883;


// WiFiClientSecure espClient;  // for no secure connection use WiFiClient instead of WiFiClientSecure
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;

unsigned char enq1[3] = { 0x05, 0x30, 0x31 };
unsigned char enq2[3] = { 0x05, 0x30, 0x32 };
unsigned char enq3[3] = { 0x05, 0x30, 0x33 };
unsigned char enq4[3] = { 0x05, 0x30, 0x34 };
unsigned char enq5[3] = { 0x05, 0x30, 0x35 };
unsigned char enq6[3] = { 0x05, 0x30, 0x36 };
unsigned char enq7[3] = { 0x05, 0x30, 0x37 };
unsigned char enq8[3] = { 0x05, 0x30, 0x38 };

unsigned char CQreq1[8] = { 0x01, 0x30, 0x31, 0x02, 0x43, 0x51, 0x03, 0x12 };
unsigned char CQreq2[8] = { 0x01, 0x30, 0x32, 0x02, 0x43, 0x51, 0x03, 0x11 };
unsigned char CQreq3[8] = { 0x01, 0x30, 0x33, 0x02, 0x43, 0x51, 0x03, 0x10 };
unsigned char CQreq4[8] = { 0x01, 0x30, 0x34, 0x02, 0x43, 0x51, 0x03, 0x17 };
unsigned char CQreq5[8] = { 0x01, 0x30, 0x35, 0x02, 0x43, 0x51, 0x03, 0x16 };
unsigned char CQreq6[8] = { 0x01, 0x30, 0x36, 0x02, 0x43, 0x51, 0x03, 0x15 };
unsigned char CQreq7[8] = { 0x01, 0x30, 0x37, 0x02, 0x43, 0x51, 0x03, 0x14 };
unsigned char CQreq8[8] = { 0x01, 0x30, 0x38, 0x02, 0x43, 0x51, 0x03, 0x1B };


unsigned char TQreq1[8] = { 0x01, 0x30, 0x31, 0x02, 0x54, 0x51, 0x03, 0x05 };
unsigned char TQreq2[8] = { 0x01, 0x30, 0x32, 0x02, 0x54, 0x51, 0x03, 0x06 };
unsigned char TQreq3[8] = { 0x01, 0x30, 0x33, 0x02, 0x54, 0x51, 0x03, 0x07 };
unsigned char TQreq4[8] = { 0x01, 0x30, 0x34, 0x02, 0x54, 0x51, 0x03, 0x00 };
unsigned char TQreq5[8] = { 0x01, 0x30, 0x35, 0x02, 0x54, 0x51, 0x03, 0x01 };
unsigned char TQreq6[8] = { 0x01, 0x30, 0x36, 0x02, 0x54, 0x51, 0x03, 0x02 };
unsigned char TQreq7[8] = { 0x01, 0x30, 0x37, 0x02, 0x54, 0x51, 0x03, 0x03 };
unsigned char TQreq8[8] = { 0x01, 0x30, 0x38, 0x02, 0x54, 0x51, 0x03, 0x0C };


unsigned char APappro1[8] = { 0x01, 0x30, 0x31, 0x02, 0x41, 0x50, 0x03, 0x11 };
unsigned char APappro2[8] = { 0x01, 0x30, 0x32, 0x02, 0x41, 0x50, 0x03, 0x12 };
unsigned char APappro3[8] = { 0x01, 0x30, 0x33, 0x02, 0x41, 0x50, 0x03, 0x13 };
unsigned char APappro4[8] = { 0x01, 0x30, 0x34, 0x02, 0x41, 0x50, 0x03, 0x14 };
unsigned char APappro5[8] = { 0x01, 0x30, 0x35, 0x02, 0x41, 0x50, 0x03, 0x15 };
unsigned char APappro6[8] = { 0x01, 0x30, 0x36, 0x02, 0x41, 0x50, 0x03, 0x16 };
unsigned char APappro7[8] = { 0x01, 0x30, 0x37, 0x02, 0x41, 0x50, 0x03, 0x17 };
unsigned char APappro8[8] = { 0x01, 0x30, 0x38, 0x02, 0x41, 0x50, 0x03, 0x18 };


unsigned char presetcancel1[8] = { 0x01, 0x30, 0x31, 0x02, 0x53, 0x43, 0x03, 0x10 };
unsigned char presetcancel2[8] = { 0x01, 0x30, 0x32, 0x02, 0x53, 0x43, 0x03, 0x13 };
unsigned char presetcancel3[8] = { 0x01, 0x30, 0x33, 0x02, 0x53, 0x43, 0x03, 0x12 };
unsigned char presetcancel4[8] = { 0x01, 0x30, 0x34, 0x02, 0x53, 0x43, 0x03, 0x15 };
unsigned char presetcancel5[8] = { 0x01, 0x30, 0x35, 0x02, 0x53, 0x43, 0x03, 0x14 };
unsigned char presetcancel6[8] = { 0x01, 0x30, 0x36, 0x02, 0x53, 0x43, 0x03, 0x17 };
unsigned char presetcancel7[8] = { 0x01, 0x30, 0x37, 0x02, 0x53, 0x43, 0x03, 0x16 };
unsigned char presetcancel8[8] = { 0x01, 0x30, 0x38, 0x02, 0x53, 0x43, 0x03, 0x19 };



char whreqbuf[20] = "detpos/device/whreq";
char devicebuf[2] = "1";

// char pumpreqbuf[16] = "detpos/device/1";
char pumpreqbuf[23] = "detpos/device/permit/1";
char pumpapprobuf[22] = "detpos/local_server/1";

char pricechange[26] = "detpos/local_server/price";

char pumpfinalbuf[22] = "detpos/device/Final/1";

char pplive[25] = "detpos/device/livedata/1";

char reqcancelbuf[18] = "detpos/device/req";

char activebuf[23] = "detpos/device/active/1";

char server_rpy[20] = "detpos/local_server";      // ak
char server_err[25] = "detpos/local_server/err";  // ak

char mqttbuffer[50];
char ppbuffer[50];

char msgbuffer[50];

char charArray[7];

char pumpfinalreloadbuf[23] = "detpos/device/Reload/1";  // reload add
char device_Id_topic[40] = "detpos/local_server/initial1/det/0A0000";

int i = 0;

int bufcount = 0;

String incommingMessage;
String sendMessage;

// const char* ssidtemp = "POS_Server";
// const char* passtemp = "asdffdsa";
char ssidtemp[50];  // for WiFi.begin (SSID,        )
char passtemp[50];  // for WiFi.begin (    , pass   )
// const char* ssidtemp = "Digital Engineering Tech";
// const char* passtemp = "131337329";

int wifitrytime = 0;

// bool sendpermission = false;
bool sendpermission = true;

int sendcount = 2;
bool whreqcount = true;
bool zerocount = true;

#define wifiled 33
#define powerled 25
#define TXled 27
#define RXled 26

int pumpid1;
int pumpid2;
int pumpid3;
int pumpid4;
int pumpid5;
int pumpid6;
int pumpid7;
int pumpid8;
int nozzlenum;
int devicenum;

int pumpmqttnum;

bool pp1 = false;
bool pp2 = false;
bool pp3 = false;
bool pp4 = false;
bool pp5 = false;
bool pp6 = false;
bool pp7 = false;
bool pp8 = false;


// pc change
char data_sequence[] = { 0x30, 0x30, 0x02, 0x50, 0x43, 0x30, 0x30, 0x30, 0x30, 0x03 };
char send_sequence[12] = { 0x01, 0xFF, 0xFF, 0x02, 0x50, 0x43, 0x30, 0x30, 0x30, 0x30, 0x03, 0x30 };
char lrc = 0x00;

// hmi

unsigned char Buffer[80];

char ssidBuf[50];
char passBuf[50];
unsigned char showSSID[6] = { 0X5A, 0XA5, 0X40, 0X82, 0X12, 0x00 };
unsigned char showPASS[6] = { 0X5A, 0XA5, 0X40, 0X82, 0X13, 0x00 };
unsigned char page[9] = { 0X5A, 0XA5, 0X07, 0X82, 0X00, 0X84, 0X5A, 0X01, 0X00 };  // Page change
unsigned char deviceary[8] = { 0x5A, 0XA5, 0x05, 0X82, 0x31, 0x00, 0x00, 0x00 };

#define hmi 23

bool hmivalue = false;


long lastdatatime;
long lastdatatimeout;
bool activecount = true;


long activetime;
long activetime1;

bool TQnocount = false;
bool CQnocount = false;


bool app1 = false;
bool app2 = false;
bool app3 = false;
bool app4 = false;
bool app5 = false;
bool app6 = false;
bool app7 = false;
bool app8 = false;
//setprice
char rdybuf[20] = "detpos/device/rdy/1";
char preset[27] = "detpos/local_server/preset";
char presetmqttary[11];
char stary[] = { 0x30, 0x31, 0x02, 0x53, 0x54, 0x41, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x03 };
char starysend_sequence[16] = { 0x01, 0xFF, 0xFF, 0x02, 0x53, 0x54, 0x41, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x03, 0x30 };

char myreload[29] = "detpos/local_server/reload/1";

char presetcancel[35] = "detpos/local_server/presetcancel/1";

bool just_test = false;  // delete

String final_str;
unsigned long previousMillis = 0;
char server_rpy_ary[7];
char err_buffer[9];

bool reloadcount = false;

void setup() {

  Serial.begin(115200);
  EEPROM.begin(512);

  Serial2.begin(9600, SERIAL_8E1);
  // Serial2.begin(9600);

  pinMode(wifiled, OUTPUT);
  pinMode(powerled, OUTPUT);
  pinMode(TXled, OUTPUT);
  pinMode(RXled, OUTPUT);

  pinMode(hmi, INPUT_PULLUP);

  // EEPROMWrite();

  initEEPROMdatashow();
  initWiFi();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);


  Serial2.write(enq1, 3);

  EEPROMinit();
}

void EEPROMWrite() {
  EEPROM.write(109, 5);  //devicenum
  EEPROM.commit();
  EEPROM.write(110, 4);  // nozzlenum
  EEPROM.commit();
  EEPROM.write(101, 9);
  EEPROM.commit();
  EEPROM.write(102, 10);
  EEPROM.commit();
  EEPROM.write(103, 11);
  EEPROM.commit();
  EEPROM.write(104, 12);
  EEPROM.commit();
  EEPROM.write(105, 0);
  EEPROM.commit();
  EEPROM.write(106, 0);
  EEPROM.commit();
  EEPROM.write(107, 0);
  EEPROM.commit();
  EEPROM.write(108, 0);
  EEPROM.commit();

  Serial.println("Yap save##################################");
}

void initEEPROMdatashow() {
  for (int j = 0; j < 50; j++) {
    ssidBuf[j] = EEPROM.read(j);
  }
  for (int j = 50; j < 100; j++) {
    passBuf[j - 50] = EEPROM.read(j);
  }
  Serial.print(ssidBuf);
  Serial.print("  ");
  Serial.println(passBuf);

  // devicenum = 1;

  devicenum = EEPROM.read(109);

  nozzlenum = EEPROM.read(110);
  pumpid1 = EEPROM.read(101);
  pumpid2 = EEPROM.read(102);
  pumpid3 = EEPROM.read(103);
  pumpid4 = EEPROM.read(104);
  pumpid5 = EEPROM.read(105);
  pumpid6 = EEPROM.read(106);
  pumpid7 = EEPROM.read(107);
  pumpid8 = EEPROM.read(108);

  // devicenum = 2;

  // nozzlenum = 4;
  // pumpid1 = 9;
  // pumpid2 = 10;
  // pumpid3 = 11;
  // pumpid4 = 12;
  // pumpid5 = 13;
  // pumpid6 = 14;
  // pumpid7 = 15;
  // pumpid8 = 16;


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
  devicenum = EEPROM.read(109);
  // devicenum = 1;

  if (devicenum == 1) {
    pumpreqbuf[21] = '1';
    pumpapprobuf[20] = '1';
    pumpfinalbuf[20] = '1';
    pplive[23] = '1';
    devicebuf[0] = '1';
    activebuf[21] = '1';
    rdybuf[20] = '1';
    device_Id_topic[38] = '1';
    myreload[27] = '1';            // reload add
    pumpfinalreloadbuf[21] = '1';  // reload add
    presetcancel[33] = '1';        // presetcance add
  } else if (devicenum == 2) {
    pumpreqbuf[21] = '2';
    pumpapprobuf[20] = '2';
    pumpfinalbuf[20] = '2';
    pplive[23] = '2';
    devicebuf[0] = '2';
    activebuf[21] = '2';
    rdybuf[20] = '2';
    device_Id_topic[38] = '2';
    myreload[27] = '2';            // reload add
    pumpfinalreloadbuf[21] = '2';  // reload add
    presetcancel[33] = '2';        // presetcance add
  } else if (devicenum == 3) {
    pumpreqbuf[21] = '3';
    pumpapprobuf[20] = '3';
    pumpfinalbuf[20] = '3';
    pplive[23] = '3';
    devicebuf[0] = '3';
    activebuf[21] = '3';
    rdybuf[20] = '3';
    device_Id_topic[38] = '3';
    myreload[27] = '3';            // reload add
    pumpfinalreloadbuf[21] = '3';  // reload add
    presetcancel[33] = '3';        // presetcance add
  } else if (devicenum == 4) {
    pumpreqbuf[21] = '4';
    pumpapprobuf[20] = '4';
    pumpfinalbuf[20] = '4';
    pplive[23] = '4';
    devicebuf[0] = '4';
    activebuf[21] = '4';
    rdybuf[20] = '4';
    device_Id_topic[38] = '4';
    myreload[27] = '4';  // reload add
    pumpfinalreloadbuf[21] = '4';  // reload add
    presetcancel[33] = '4';        // presetcance add
  } else if (devicenum == 5) {
    pumpreqbuf[21] = '5';
    pumpapprobuf[20] = '5';
    pumpfinalbuf[20] = '5';
    pplive[23] = '5';
    devicebuf[0] = '5';
    activebuf[21] = '5';
    rdybuf[20] = '5';
    device_Id_topic[38] = '5';
    myreload[27] = '5';            // reload add
    pumpfinalreloadbuf[21] = '5';  // reload add
    presetcancel[33] = '5';        // presetcance add

  } else if (devicenum == 6) {
    pumpreqbuf[21] = '6';
    pumpapprobuf[20] = '6';
    pumpfinalbuf[20] = '6';
    pplive[23] = '6';
    devicebuf[0] = '6';
    activebuf[21] = '6';
    rdybuf[20] = '6';
    device_Id_topic[38] = '6';
    myreload[27] = '6';            // reload add
    pumpfinalreloadbuf[21] = '6';  // reload add
    presetcancel[33] = '6';        // presetcance add
  } else if (devicenum == 7) {
    pumpreqbuf[21] = '7';
    pumpapprobuf[20] = '7';
    pumpfinalbuf[20] = '7';
    pplive[23] = '7';
    devicebuf[0] = '7';
    activebuf[21] = '7';
    rdybuf[20] = '7';
    device_Id_topic[38] = '7';
    myreload[27] = '7';            // reload add
    pumpfinalreloadbuf[21] = '7';  // reload add
    presetcancel[33] = '7';        // presetcance add

  } else if (devicenum == 8) {
    pumpreqbuf[21] = '8';
    pumpapprobuf[20] = '8';
    pumpfinalbuf[20] = '8';
    pplive[23] = '8';
    devicebuf[0] = '8';
    activebuf[21] = '8';
    rdybuf[20] = '8';
    device_Id_topic[38] = '8';
    myreload[27] = '8';            // reload add
    pumpfinalreloadbuf[21] = '8';  // reload add
    presetcancel[33] = '8';        // presetcance add
  }

  Serial.print("my data is ");
  for (int jj = 0; jj < 28; jj++) {
    Serial.print(myreload[jj]);
  }
  Serial.println();
  myreload[0] = 'd';

  Serial.print("my data is ");
  for (int jj = 0; jj < 28; jj++) {
    Serial.print(myreload[jj]);
  }
  Serial.println();

  Serial.print("my preset cancel is ");
  for (int jj = 0; jj < 34; jj++) {
    Serial.print(presetcancel[jj]);
  }

  Serial.println();
  presetcancel[0] = 'd';

  Serial.print("my data is ");
  for (int jj = 0; jj < 34; jj++) {
    Serial.print(presetcancel[jj]);
  }
  Serial.println();

  // Serial.println(pumpreqbuf);

  // delay(5000);

  // nozzlenum = EEPROM.read(110);

  // pumpid1 = EEPROM.read(101);
  // pumpid2 = EEPROM.read(102);
  // pumpid3 = EEPROM.read(103);
  // pumpid4 = EEPROM.read(104);
  // pumpid5 = EEPROM.read(105);
  // pumpid6 = EEPROM.read(106);
  // pumpid7 = EEPROM.read(107);
  // pumpid8 = EEPROM.read(108);

  if (nozzlenum == 2) {
    pumpid3 = 0;
    pumpid4 = 0;
    pumpid5 = 0;
    pumpid6 = 0;
    pumpid7 = 0;
    pumpid8 = 0;
  } else if (nozzlenum == 4) {
    pumpid5 = 0;
    pumpid6 = 0;
    pumpid7 = 0;
    pumpid8 = 0;
  }
}


void loop() {

  digitalWrite(powerled, HIGH);
  // digitalWrite(wifiled,HIGH);

  if (!digitalRead(hmi)) hmivalue = true;

  if (hmivalue) {
    Serial.println("HMI ");
    hmisetup();
  } else {

    if (WiFi.status() == WL_CONNECTED) {
      // digitalWrite(wifiled, HIGH);
      if (!client.connected()) {
        //  serverConnectionIcon("disconnected");
        Serial.println("Cloud disconnect");
        whreqcount = true;
        digitalWrite(wifiled, LOW);
      } else {
        // Serial.println("Connected to the Cloud");
        digitalWrite(wifiled, HIGH);
      }

    } else {
      Serial.println("Not Connected");
      initWiFi();
    }
    if (!client.connected()) {
      reconnect();
    }

    client.loop();

    fueldispenserdatarecive();
  }

  pumpactive();
}


void pumpactive() {

  activetime = millis() / 1000;

  if ((activetime - activetime1) > 5) {
    if (activecount) {
      // Serial.println("active");
      // Serial.println(activebuf);
      if (sendpermission) {
        client.publish(activebuf, "1");
        txledonoff();
      }
      activecount = false;
    }
    activetime1 = millis() / 1000;
  } else activecount = true;
}


void fueldispenserdatarecive() {

  // Serial.println("hello");

  if (Serial2.available()) {
    Buffer[bufcount] = Serial2.read();
    if (Buffer[bufcount] == 0x01) bufcount = 0;
    delay(2);
    // digitalWrite(RXled,HIGH);
    // Serial.print("B");
    // Serial.print(bufcount);
    // Serial.print(" ");
    // Serial.print(Buffer[bufcount], HEX);
    // Serial.print("  ");
    bufcount++;

    if (Buffer[bufcount - 1] == 0x03) {
      // digitalWrite(RXled, HIGH);
      Buffer[bufcount] = Serial2.read();
      delay(2);
      // Serial.print(Buffer[bufcount], HEX);
      // Serial.println(" from main loop");

      //ohnchawadd
      delay(300);

      encode(Buffer);

      //ohnchawadd
      delay(50);


      if (sendcount == 5 && nozzlenum == 4) sendcount = 1;
      else if (sendcount == 3 && nozzlenum == 2) sendcount = 1;
      else if (sendcount == 9 && nozzlenum == 8) sendcount = 1;


      if (sendcount == 1) Serial2.write(enq1, 3);
      else if (sendcount == 2) Serial2.write(enq2, 3);
      else if (sendcount == 3) Serial2.write(enq3, 3);
      else if (sendcount == 4) Serial2.write(enq4, 3);
      else if (sendcount == 5) Serial2.write(enq5, 3);
      else if (sendcount == 6) Serial2.write(enq6, 3);
      else if (sendcount == 7) Serial2.write(enq7, 3);
      else if (sendcount == 8) Serial2.write(enq8, 3);
      // Serial.print("u write ENQ ");
      // Serial.println(sendcount);
      // txledonoff();

      sendcount++;

      // if (sendpermission && client.connected()) {
      //   androidsend();

      // }



      bufcount = 0;
      lastdatatime = millis();
    } else if (Buffer[bufcount - 1] == 0x06) {
      // Serial.println("ACK");
      // digitalWrite(RXled, HIGH);

      reqcancel();  // if req cancel


      //ohnchawadd
      delay(100);

      //9/6/2024
      if(reloadcount){
        reloadfun();
        reloadcount = false;
      }

      if (sendcount == 5 && nozzlenum == 4) sendcount = 1;
      else if (sendcount == 3 && nozzlenum == 2) sendcount = 1;
      else if (sendcount == 9 && nozzlenum == 8) sendcount = 1;

      if (sendcount == 1) Serial2.write(enq1, 3);
      else if (sendcount == 2) Serial2.write(enq2, 3);
      else if (sendcount == 3) Serial2.write(enq3, 3);
      else if (sendcount == 4) Serial2.write(enq4, 3);
      else if (sendcount == 5) Serial2.write(enq5, 3);
      else if (sendcount == 6) Serial2.write(enq6, 3);
      else if (sendcount == 7) Serial2.write(enq7, 3);
      else if (sendcount == 8) Serial2.write(enq8, 3);

      // Serial.print("u writet ENQ ");
      // Serial.println(sendcount);
      // txledonoff();

      sendcount++;
      bufcount = 0;
      lastdatatime = millis();
    }


  }
  // extra add
  else {
    // digitalWrite(RXled, LOW);
    // Serial.print("No ");
    lastdatatimeout = millis();
    // Serial.println(lastdatatimeout - lastdatatime);

    if (lastdatatimeout - lastdatatime > 500) {
      // Serial.print("None ");
      // Serial.println(sendcount - 1);

    //ohnchawadd
      delay(50);

      if (sendcount == 5 && nozzlenum == 4) sendcount = 1;
      else if (sendcount == 3 && nozzlenum == 2) sendcount = 1;
      else if (sendcount == 9 && nozzlenum == 8) sendcount = 1;

      if (sendcount == 1) Serial2.write(enq1, 3);
      else if (sendcount == 2) Serial2.write(enq2, 3);
      else if (sendcount == 3) Serial2.write(enq3, 3);
      else if (sendcount == 4) Serial2.write(enq4, 3);
      else if (sendcount == 5) Serial2.write(enq5, 3);
      else if (sendcount == 6) Serial2.write(enq6, 3);
      else if (sendcount == 7) Serial2.write(enq7, 3);
      else if (sendcount == 8) Serial2.write(enq8, 3);

      // Serial.print("u writet ENQ ");
      // Serial.println(sendcount);
      // txledonoff();

      sendcount++;
      bufcount = 0;
      lastdatatime = millis();
    }
  }
}


void txledonoff() {
  digitalWrite(TXled, HIGH);

  delay(10);
  digitalWrite(TXled, LOW);
  // delay(10);
  // delay(10);
}

void rxledonoff() {
  digitalWrite(RXled, HIGH);
  delay(10);
  digitalWrite(RXled, LOW);
  // delay(10);
  // delay(10);
}


void encode(unsigned char* enq) {
  // Serial.print("Your data length is ");
  Serial.println(bufcount);

  if (Buffer[4] == 0x50 && Buffer[5] == 0x50 && bufcount == 21) {  //pp function
    PPfun();
  }

  else if (Buffer[4] == 0x41 && Buffer[5] == 0x51 && bufcount == 7) {  //AQ
    AQfun();
  }

  else if (Buffer[4] == 0x4C && Buffer[5] == 0x4B) {  //LK
    lkfun();
  }

  else if (Buffer[4] == 0x50 && Buffer[5] == 0x43) {  //PC
    pcfun();
  }

  else if (Buffer[4] == 0x55 && Buffer[5] == 0x4C) {  //PC
    ulfun();
  }
}


void PPfun() {
  for (int y = 0; y < 50; y++) {
    ppbuffer[y] = 0;
  }

  pumpidchange();
  ppbuffer[2] = 'L';

  int y = 0;


  for (int j = 6; j < 13; j++) {

    if (j == 10) {
      ppbuffer[y + 3] = '.';
      y++;
      zerocount = false;  //add
    }

    if (Buffer[j] == 0x30 && zerocount) {
      continue;
    }
    if (Buffer[j] != 0x30) {
      zerocount = false;
    }

    if (Buffer[j] == 0x03) break;

    // if (j == 10) {
    //   ppbuffer[y + 3] = '.';
    //   y++;
    //   zerocount = false;  //add
    // }
    ppbuffer[y + 3] = Buffer[j];
    y++;
  }

  zerocount = true;
  ppbuffer[y + 3] = 'P';
  y++;
  for (int j = 13; j < 20; j++) {
    if (Buffer[j] == 0x30 && zerocount) {
      continue;
    }

    if (Buffer[j] != 0x30) {
      zerocount = false;
    }

    if (Buffer[j] == 0x03) break;
    ppbuffer[y + 3] = Buffer[j];
    y++;
  }
  zerocount = true;
  Serial.println(ppbuffer);

  // client.publish("detpos/device/livedata/1", ppbuffer);
  if (sendpermission) {
    client.publish(pplive, ppbuffer);
    txledonoff();
  }

  if (Buffer[1] == 0x30 && Buffer[2] == 0x31) pp1 = false;
  else if (Buffer[1] == 0x30 && Buffer[2] == 0x32) pp2 = false;
  else if (Buffer[1] == 0x30 && Buffer[2] == 0x33) pp3 = false;
  else if (Buffer[1] == 0x30 && Buffer[2] == 0x34) pp4 = false;
  else if (Buffer[1] == 0x30 && Buffer[2] == 0x35) pp5 = false;
  else if (Buffer[1] == 0x30 && Buffer[2] == 0x36) pp6 = false;
  else if (Buffer[1] == 0x30 && Buffer[2] == 0x37) pp7 = false;
  else if (Buffer[1] == 0x30 && Buffer[2] == 0x38) pp8 = false;

  if (Buffer[1] == 0x30 && Buffer[2] == 0x31) app1 = false;  //last add
  else if (Buffer[1] == 0x30 && Buffer[2] == 0x32) app2 = false;
  else if (Buffer[1] == 0x30 && Buffer[2] == 0x33) app3 = false;
  else if (Buffer[1] == 0x30 && Buffer[2] == 0x34) app4 = false;
  else if (Buffer[1] == 0x30 && Buffer[2] == 0x35) app5 = false;
  else if (Buffer[1] == 0x30 && Buffer[2] == 0x36) app6 = false;
  else if (Buffer[1] == 0x30 && Buffer[2] == 0x37) app7 = false;
  else if (Buffer[1] == 0x30 && Buffer[2] == 0x38) app8 = false;
}

void pcfun() {
  for (int y = 0; y < 50; y++) {
    ppbuffer[y] = 0;
  }
  pumpidchange();

  Serial.print("PC ");
  Serial.println(ppbuffer);
  if (sendpermission) {
    client.publish("detpos/device/price", ppbuffer);
    txledonoff();
  }
}


void ulfun() {
  for (int y = 0; y < 50; y++) {
    ppbuffer[y] = 0;
  }
  pumpidchange();

  ppbuffer[2] = 'r';
  ppbuffer[3] = 'd';
  ppbuffer[4] = 'y';

  Serial.print("rdy ");
  Serial.println(ppbuffer);
  if (sendpermission) {
    client.publish(rdybuf, ppbuffer);
    txledonoff();
  }
}

void presetcancelfun() {

  char presetcancelArray[2];
  incommingMessage.toCharArray(presetcancelArray, incommingMessage.length() + 1);
  Serial.print("presetcancel is ");

  Serial.println(presetcancelArray);

  // Buffer[1] = presetcancelArray[0];
  // Buffer[2] = presetcancelArray[1];

  charArray[0] = presetcancelArray[0];
  charArray[1] = presetcancelArray[1];
  pumpmqttnumchange();
  Buffer[1] = data_sequence[0];
  Buffer[2] = data_sequence[1];

  presetcancelsend();
}


void presetcancelsend() {

  if (Buffer[1] == 0x30 && Buffer[2] == 0x31) {
    Serial2.write(presetcancel1, 8);
    Serial.println("u writed presetCancel1");
  } else if (Buffer[1] == 0x30 && Buffer[2] == 0x32) {
    Serial2.write(presetcancel2, 8);
    Serial.println("u writed presetCancel2");
  } else if (Buffer[1] == 0x30 && Buffer[2] == 0x33) {
    Serial2.write(presetcancel3, 8);
    Serial.println("u writed presetCancel3");
  } else if (Buffer[1] == 0x30 && Buffer[2] == 0x34) {
    Serial2.write(presetcancel4, 8);
    Serial.println("u writed presetCancel4");
  } else if (Buffer[1] == 0x30 && Buffer[2] == 0x35) {
    Serial2.write(presetcancel5, 8);
    Serial.println("u writed presetCancel5");
  } else if (Buffer[1] == 0x30 && Buffer[2] == 0x36) {
    Serial2.write(presetcancel6, 8);
    Serial.println("u writed presetCancel6");
  } else if (Buffer[1] == 0x30 && Buffer[2] == 0x37) {
    Serial2.write(presetcancel7, 8);
    Serial.println("u writed presetCancel7");
  } else if (Buffer[1] == 0x30 && Buffer[2] == 0x38) {
    Serial2.write(presetcancel8, 8);
    Serial.println("u writed presetCancel8");
  }
}


void reloadfun() {

  // reload extra add
  char reloadArray[2];
  incommingMessage.toCharArray(reloadArray, incommingMessage.length() + 1);
  Serial.print("reload is ");

  Serial.println(reloadArray);

  // Buffer[1] = reloadArray[0];
  // Buffer[2] = reloadArray[1];
  //end
  charArray[0] = reloadArray[0];
  charArray[1] = reloadArray[1];
  pumpmqttnumchange();
  Buffer[1] = data_sequence[0];
  Buffer[2] = data_sequence[1];
  int y = 0;

  TQsend();

  Serial.print("ur TQ buf1 is ");
  Serial.println(bufcount);


  if (bufcount != 25) {
    TQsend();
  }

  Serial.print("ur TQ buf2 is ");
  Serial.println(bufcount);

  if (bufcount != 25) {
    TQsend();
  }
  Serial.print("ur TQ buf3 is ");
  Serial.println(bufcount);

  // Serial.println(bufcount);


  if (Buffer[4] == 0x54 && Buffer[5] == 0x52 && bufcount == 25) {  //TR

    for (int y = 0; y < 50; y++) {
      ppbuffer[y] = 0;
    }
    // ppbuffer[0] = Buffer[1];
    // ppbuffer[1] = Buffer[2];
    pumpidchange();
    ppbuffer[2] = 'S';

    for (int j = 6; j < 10; j++) {
      if (Buffer[j] == 0x30 && zerocount) {
        continue;
      }

      if (Buffer[j] != 0x30) {
        zerocount = false;
      }

      if (Buffer[j] == 0x03) break;


      ppbuffer[y + 3] = Buffer[j];
      y++;
    }

    zerocount = true;
    ppbuffer[y + 3] = 'L';
    y++;
    for (int j = 10; j < 17; j++) {

      if (j == 14) {
        ppbuffer[y + 3] = '.';
        y++;
        zerocount = false;  //add
      }

      if (Buffer[j] == 0x30 && zerocount) {
        continue;
      }

      if (Buffer[j] != 0x30) {
        zerocount = false;
      }

      if (Buffer[j] == 0x03) break;

      // if (j == 14) {
      //   ppbuffer[y + 3] = '.';
      //   y++;
      //   zerocount = false;  //add
      // }
      ppbuffer[y + 3] = Buffer[j];
      y++;
    }

    zerocount = true;
    ppbuffer[y + 3] = 'P';
    y++;
    for (int j = 17; j <= 23; j++) {
      if (Buffer[j] == 0x30 && zerocount) {
        continue;
      }

      if (Buffer[j] != 0x30) {
        zerocount = false;
      }

      if (Buffer[j] == 0x03) break;
      ppbuffer[y + 3] = Buffer[j];
      y++;
    }
    zerocount = true;
    // Serial.println(ppbuffer);
    TQnocount = false;
  }

  else {
    TQnocount = true;
  }

  Serial.println("your ppbuffer is");
  for (int y = 0; y < 50; y++) {
    Serial.print(ppbuffer[y], HEX);
    Serial.print(" ");
  }
  Serial.println();
  //////

  // Buffer[1] = reloadArray[0];
  // Buffer[2] = reloadArray[1];
  charArray[0] = reloadArray[0];
  charArray[1] = reloadArray[1];
  pumpmqttnumchange();
  Buffer[1] = data_sequence[0];
  Buffer[2] = data_sequence[1];

  CQsend();
  if (bufcount != 17) {
    CQsend();
  }
  
  if (bufcount != 17) {
    CQsend();
  }

  // Serial.println(bufcount);
  if (Buffer[4] == 0x43 && Buffer[5] == 0x54 && bufcount == 17) {  //CT

    zerocount = true;
    ppbuffer[y + 3] = 'T';
    y++;
    for (int j = 6; j < 16; j++) {
      if (Buffer[j] == 0x30 && zerocount) {
        continue;
      }

      if (Buffer[j] != 0x30) {
        zerocount = false;
      }

      if (Buffer[j] == 0x03) break;

      if (j == 13) {
        ppbuffer[y + 3] = '.';
        y++;
        zerocount = false;  //add
      }


      ppbuffer[y + 3] = Buffer[j];
      y++;
    }


    zerocount = true;
    Serial.println(ppbuffer);
    // if (sendpermission){
    //   client.publish(pumpfinalbuf, ppbuffer);
    //   txledonoff();
    // }
    CQnocount = false;
  } else {
    CQnocount = true;
  }

  if (CQnocount || TQnocount) {
    TQno();
  }

  if (sendpermission) {
    client.publish(pumpfinalreloadbuf, ppbuffer);
    txledonoff();
    Serial.println("yep u got");
  }
  //   for (int i = 0 ; i < 10 ; i++) {
  //   err_buffer[i] = 0;
  // }
  
  // err_buffer[0] = ppbuffer[0];
  // err_buffer[1] = ppbuffer[1];
  // err_buffer[2] = '/';
  // err_buffer[3] = 'e';
  // err_buffer[4] = 'r';
  // err_buffer[5] = 'r';
  // err_buffer[6] = 'o';
  // err_buffer[7] = 'r';

  // int waitcount = 0;  //akkkk
  // previousMillis = millis() / 1000;

  // while (waitcount < 2) {


  //   if (ppbuffer[0] == server_rpy_ary[0] && ppbuffer[1] == server_rpy_ary[1] && final_str == "D1S1") {
  //     Serial.println("Bye ....................................");
  //     final_str = "";
  //     break;
  //   }


  //   if (millis() / 1000 - previousMillis >= 13) {
  //     previousMillis = millis() / 1000;
      
  //     if (waitcount < 1 ){
  //       client.publish(pumpfinalreloadbuf, ppbuffer); 
  //     }

  //     waitcount++;
  //     // Serial.println(waitcount);
  //   }

  //   client.loop();

  // }
  
  // if (waitcount == 2) client.publish(server_err, err_buffer);
}


void lkfun() {
  int y = 0;

  TQsend();

  Serial.print("ur TQ buf is ");
  Serial.println(bufcount);


  if (bufcount != 25) {
    TQsend();
  }

  Serial.print("ur TQ buf is ");
  Serial.println(bufcount);

  if (bufcount != 25) {
    TQsend();
  }
  Serial.print("ur TQ buf is ");
  Serial.println(bufcount);

  // Serial.println(bufcount);


  if (Buffer[4] == 0x54 && Buffer[5] == 0x52 && bufcount == 25) {  //TR

    for (int y = 0; y < 50; y++) {
      ppbuffer[y] = 0;
    }
    // ppbuffer[0] = Buffer[1];
    // ppbuffer[1] = Buffer[2];
    pumpidchange();
    ppbuffer[2] = 'S';

    for (int j = 6; j < 10; j++) {
      if (Buffer[j] == 0x30 && zerocount) {
        continue;
      }

      if (Buffer[j] != 0x30) {
        zerocount = false;
      }

      if (Buffer[j] == 0x03) break;


      ppbuffer[y + 3] = Buffer[j];
      y++;
    }

    zerocount = true;
    ppbuffer[y + 3] = 'L';
    y++;
    for (int j = 10; j < 17; j++) {

      if (j == 14) {
        ppbuffer[y + 3] = '.';
        y++;
        zerocount = false;  //add
      }

      if (Buffer[j] == 0x30 && zerocount) {
        continue;
      }

      if (Buffer[j] != 0x30) {
        zerocount = false;
      }

      if (Buffer[j] == 0x03) break;

      // if (j == 14) {
      //   ppbuffer[y + 3] = '.';
      //   y++;
      //   zerocount = false;  //add
      // }
      ppbuffer[y + 3] = Buffer[j];
      y++;
    }

    zerocount = true;
    ppbuffer[y + 3] = 'P';
    y++;
    for (int j = 17; j <= 23; j++) {
      if (Buffer[j] == 0x30 && zerocount) {
        continue;
      }

      if (Buffer[j] != 0x30) {
        zerocount = false;
      }

      if (Buffer[j] == 0x03) break;
      ppbuffer[y + 3] = Buffer[j];
      y++;
    }
    zerocount = true;
    // Serial.println(ppbuffer);
    TQnocount = false;


  }

  else {
    TQnocount = true;
  }

  //////

  CQsend();
  if (bufcount != 17) {
    CQsend();
  }
  if (bufcount != 17) {
    CQsend();
  }

  // Serial.println(bufcount);
  if (Buffer[4] == 0x43 && Buffer[5] == 0x54 && bufcount == 17) {  //CT

    zerocount = true;
    ppbuffer[y + 3] = 'T';
    y++;
    for (int j = 6; j < 16; j++) {
      if (Buffer[j] == 0x30 && zerocount) {
        continue;
      }

      if (Buffer[j] != 0x30) {
        zerocount = false;
      }

      if (Buffer[j] == 0x03) break;

      if (j == 13) {
        ppbuffer[y + 3] = '.';
        y++;
        zerocount = false;  //add
      }


      ppbuffer[y + 3] = Buffer[j];
      y++;
    }


    zerocount = true;
    Serial.println(ppbuffer);
    // if (sendpermission){
    //   client.publish(pumpfinalbuf, ppbuffer);
    //   txledonoff();
    // }
    CQnocount = false;
  } else {
    CQnocount = true;
  }

  if (CQnocount || TQnocount) {
    TQno();
  }


  if (sendpermission) {
    client.publish(pumpfinalbuf, ppbuffer);
    txledonoff();
  }

  for (int i = 0 ; i < 10 ; i++) {
    err_buffer[i] = 0;
  }
  
  err_buffer[0] = ppbuffer[0];
  err_buffer[1] = ppbuffer[1];
  err_buffer[2] = '/';
  err_buffer[3] = 'e';
  err_buffer[4] = 'r';
  err_buffer[5] = 'r';
  err_buffer[6] = 'o';
  err_buffer[7] = 'r';

  int waitcount = 0;  //akkkk
  previousMillis = millis() / 1000;

  while (waitcount < 2) {


    if (ppbuffer[0] == server_rpy_ary[0] && ppbuffer[1] == server_rpy_ary[1] && final_str == "D1S1") {
      Serial.println("Bye ....................................");
      final_str = "";
      break;
    }


    if (millis() / 1000 - previousMillis >= 13) {
      
      if (waitcount < 1 ){
        client.publish(pumpfinalbuf, ppbuffer); 
      }

      previousMillis = millis() / 1000;
      waitcount++;
      // Serial.println(waitcount);
    }

    client.loop();

  }
  
  if (waitcount == 2) client.publish(server_err, err_buffer);
    

}


void TQno() {
  for (int y = 0; y < 50; y++) {
    ppbuffer[y] = 0;
  }
  pumpidchange();
  ppbuffer[2] = '/';

  ppbuffer[3] = 'E';

  ppbuffer[4] = 'R';
  ppbuffer[5] = 'R';

  ppbuffer[6] = 'O';
  ppbuffer[7] = 'R';
}

void CQno() {
  for (int y = 0; y < 50; y++) {
    ppbuffer[y] = 0;
  }
  pumpidchange();
  ppbuffer[8] = 'T';
  ppbuffer[9] = '9';
}


void TQsend() {

  if (Buffer[1] == 0x30 && Buffer[2] == 0x31) {
    delay(100);
    Serial2.write(TQreq1, 8);
    Serial.println("u writed TQ1");
    bufcount = 0;

  } else if (Buffer[1] == 0x30 && Buffer[2] == 0x32) {
    Serial2.write(TQreq2, 8);
    Serial.println("u writed TQ2");
    bufcount = 0;
  } else if (Buffer[1] == 0x30 && Buffer[2] == 0x33) {
    Serial2.write(TQreq3, 8);
    Serial.println("u writed TQ3");
    bufcount = 0;
  } else if (Buffer[1] == 0x30 && Buffer[2] == 0x34) {
    Serial2.write(TQreq4, 8);
    Serial.println("u writed TQ4");
    bufcount = 0;
  } else if (Buffer[1] == 0x30 && Buffer[2] == 0x35) {
    Serial2.write(TQreq5, 8);
    Serial.println("u writed TQ5");
    bufcount = 0;
  } else if (Buffer[1] == 0x30 && Buffer[2] == 0x36) {
    Serial2.write(TQreq6, 8);
    Serial.println("u writed TQ6");
    bufcount = 0;
  } else if (Buffer[1] == 0x30 && Buffer[2] == 0x37) {
    Serial2.write(TQreq7, 8);
    Serial.println("u writed TQ7");
    bufcount = 0;
  } else if (Buffer[1] == 0x30 && Buffer[2] == 0x38) {
    Serial2.write(TQreq8, 8);
    Serial.println("u writed TQ8");
    bufcount = 0;
  }

  delay(50);
  while (Serial2.available()) {

    Buffer[bufcount] = Serial2.read();
    delay(2);
    // if(Buffer[bufcount] == 0x06) bufcount = 0;
    Serial.print(bufcount);
    Serial.print(" > ");
    Serial.print(Buffer[bufcount], HEX);
    Serial.print("  ");
    bufcount++;
    if (Buffer[bufcount - 1] == 0x06) bufcount--;

    if (Buffer[bufcount - 1] == 0x03) {
      Buffer[bufcount] = Serial2.read();
      delay(2);
      Serial.print(Buffer[bufcount], HEX);
      Serial.println(" ");
      break;


      bufcount = 0;
    }
  }
}

void CQsend() {
  if (Buffer[1] == 0x30 && Buffer[2] == 0x31) {

    Serial2.write(CQreq1, 8);
    Serial.println("u wr CQ1");
    bufcount = 0;
  } else if (Buffer[1] == 0x30 && Buffer[2] == 0x32) {

    Serial2.write(CQreq2, 8);
    Serial.println("u wr CQ2");
    bufcount = 0;
  } else if (Buffer[1] == 0x30 && Buffer[2] == 0x33) {
    Serial2.write(CQreq3, 8);
    Serial.println("u wr CQ3");
    bufcount = 0;
  } else if (Buffer[1] == 0x30 && Buffer[2] == 0x34) {
    Serial2.write(CQreq4, 8);
    Serial.println("u wr CQ4");
    bufcount = 0;
  } else if (Buffer[1] == 0x30 && Buffer[2] == 0x35) {
    Serial2.write(CQreq5, 8);
    Serial.println("u wr CQ5");
    bufcount = 0;
  } else if (Buffer[1] == 0x30 && Buffer[2] == 0x36) {
    Serial2.write(CQreq6, 8);
    Serial.println("u wr CQ6");
    bufcount = 0;
  } else if (Buffer[1] == 0x30 && Buffer[2] == 0x37) {
    Serial2.write(CQreq7, 8);
    Serial.println("u wr CQ7");
    bufcount = 0;
  } else if (Buffer[1] == 0x30 && Buffer[2] == 0x38) {
    Serial2.write(CQreq8, 8);
    Serial.println("u wr CQ8");
    bufcount = 0;
  }

  delay(50);
  while (Serial2.available()) {
    Buffer[bufcount] = Serial2.read();
    delay(2);
    Serial.print(bufcount);
    Serial.print(" < ");
    Serial.print(Buffer[bufcount], HEX);
    Serial.print("  ");
    bufcount++;
    if (Buffer[bufcount - 1] == 0x06) bufcount--;
    else if (Buffer[bufcount - 1] == 0x00) bufcount--;

    if (Buffer[bufcount - 1] == 0x03) {
      Buffer[bufcount] = Serial2.read();
      delay(2);
      Serial.print(Buffer[bufcount], HEX);
      Serial.println(" ");
      break;

      bufcount = 0;
    }
  }
}

void AQfun() {
  for (int y = 0; y < 50; y++) {
    ppbuffer[y] = 0;
  }

  ppbuffer[2] = 0x70;
  ppbuffer[3] = 0x65;
  ppbuffer[4] = 0x72;
  ppbuffer[5] = 0x6D;
  ppbuffer[6] = 0x69;
  ppbuffer[7] = 0x74;

  pumpidchange();

  Serial.print("AQ ");
  Serial.println(ppbuffer);
  if (sendpermission) {
    client.publish(pumpreqbuf, ppbuffer);
    txledonoff();
  }

  if (Buffer[1] == 0x30 && Buffer[2] == 0x31) pp1 = true;
  else if (Buffer[1] == 0x30 && Buffer[2] == 0x32) pp2 = true;
  else if (Buffer[1] == 0x30 && Buffer[2] == 0x33) pp3 = true;
  else if (Buffer[1] == 0x30 && Buffer[2] == 0x34) pp4 = true;
  else if (Buffer[1] == 0x30 && Buffer[2] == 0x35) pp5 = true;
  else if (Buffer[1] == 0x30 && Buffer[2] == 0x36) pp6 = true;
  else if (Buffer[1] == 0x30 && Buffer[2] == 0x37) pp7 = true;
  else if (Buffer[1] == 0x30 && Buffer[2] == 0x38) pp8 = true;
}


void reqcancel() {

  if (pp1 && (sendcount - 1) == 1) {
    mqttpumpidchange(pumpid1);
    ppbuffer[2] = 0x63;
    ppbuffer[3] = 0x61;
    ppbuffer[4] = 0x6E;
    ppbuffer[5] = 0x63;
    ppbuffer[6] = 0x65;
    ppbuffer[7] = 0x6C;
    Serial.print("reqcancel ");
    Serial.println(ppbuffer);
    if (sendpermission && !app1) {
      client.publish(reqcancelbuf, ppbuffer);
      txledonoff();
    }
    pp1 = false;
  }
  if (pp2 && (sendcount - 1) == 2) {
    mqttpumpidchange(pumpid2);
    ppbuffer[2] = 0x63;
    ppbuffer[3] = 0x61;
    ppbuffer[4] = 0x6E;
    ppbuffer[5] = 0x63;
    ppbuffer[6] = 0x65;
    ppbuffer[7] = 0x6C;
    Serial.print("reqcancel ");
    Serial.println(ppbuffer);
    if (sendpermission && !app2) {
      client.publish(reqcancelbuf, ppbuffer);
      txledonoff();
    }
    pp2 = false;
  }
  if (pp3 && (sendcount - 1) == 3) {
    mqttpumpidchange(pumpid3);
    ppbuffer[2] = 0x63;
    ppbuffer[3] = 0x61;
    ppbuffer[4] = 0x6E;
    ppbuffer[5] = 0x63;
    ppbuffer[6] = 0x65;
    ppbuffer[7] = 0x6C;
    Serial.print("reqcancel ");
    Serial.println(ppbuffer);
    if (sendpermission && !app3) {
      client.publish(reqcancelbuf, ppbuffer);
      txledonoff();
    }
    pp3 = false;
  }
  if (pp4 && (sendcount - 1) == 4) {
    mqttpumpidchange(pumpid4);
    ppbuffer[2] = 0x63;
    ppbuffer[3] = 0x61;
    ppbuffer[4] = 0x6E;
    ppbuffer[5] = 0x63;
    ppbuffer[6] = 0x65;
    ppbuffer[7] = 0x6C;
    Serial.print("reqcancel ");
    Serial.println(ppbuffer);
    if (sendpermission && !app4) {
      client.publish(reqcancelbuf, ppbuffer);
      txledonoff();
    }
    pp4 = false;
  }
  if (pp5 && (sendcount - 1) == 5) {
    mqttpumpidchange(pumpid5);
    ppbuffer[2] = 0x63;
    ppbuffer[3] = 0x61;
    ppbuffer[4] = 0x6E;
    ppbuffer[5] = 0x63;
    ppbuffer[6] = 0x65;
    ppbuffer[7] = 0x6C;
    Serial.print("reqcancel ");
    Serial.println(ppbuffer);
    if (sendpermission && !app5) {
      client.publish(reqcancelbuf, ppbuffer);
      txledonoff();
    }
    pp5 = false;
  }
  if (pp6 && (sendcount - 1) == 6) {
    mqttpumpidchange(pumpid6);
    ppbuffer[2] = 0x63;
    ppbuffer[3] = 0x61;
    ppbuffer[4] = 0x6E;
    ppbuffer[5] = 0x63;
    ppbuffer[6] = 0x65;
    ppbuffer[7] = 0x6C;
    Serial.print("reqcancel ");
    Serial.println(ppbuffer);
    if (sendpermission && !app6) {
      client.publish(reqcancelbuf, ppbuffer);
      txledonoff();
    }
    pp6 = false;
  }
  if (pp7 && (sendcount - 1) == 7) {
    mqttpumpidchange(pumpid7);
    ppbuffer[2] = 0x63;
    ppbuffer[3] = 0x61;
    ppbuffer[4] = 0x6E;
    ppbuffer[5] = 0x63;
    ppbuffer[6] = 0x65;
    ppbuffer[7] = 0x6C;
    Serial.print("reqcancel ");
    Serial.println(ppbuffer);
    if (sendpermission && !app7) {
      client.publish(reqcancelbuf, ppbuffer);
      txledonoff();
    }
    pp7 = false;
  }
  if (pp8 && (sendcount - 1) == 8) {
    mqttpumpidchange(pumpid8);
    ppbuffer[2] = 0x63;
    ppbuffer[3] = 0x61;
    ppbuffer[4] = 0x6E;
    ppbuffer[5] = 0x63;
    ppbuffer[6] = 0x65;
    ppbuffer[7] = 0x6C;
    Serial.print("reqcancel ");
    Serial.println(ppbuffer);
    if (sendpermission && !app8) {
      client.publish(reqcancelbuf, ppbuffer);
      txledonoff();
    }
    pp8 = false;
  }
}


void pumpidchange() {  //change incomming pumpid(0 - 8) from device to mqtt pumpid(0 - 32)
  if (Buffer[1] == 0x30 && Buffer[2] == 0x31) mqttpumpidchange(pumpid1);
  else if (Buffer[1] == 0x30 && Buffer[2] == 0x32) mqttpumpidchange(pumpid2);
  else if (Buffer[1] == 0x30 && Buffer[2] == 0x33) mqttpumpidchange(pumpid3);
  else if (Buffer[1] == 0x30 && Buffer[2] == 0x34) mqttpumpidchange(pumpid4);
  else if (Buffer[1] == 0x30 && Buffer[2] == 0x35) mqttpumpidchange(pumpid5);
  else if (Buffer[1] == 0x30 && Buffer[2] == 0x36) mqttpumpidchange(pumpid6);
  else if (Buffer[1] == 0x30 && Buffer[2] == 0x37) mqttpumpidchange(pumpid7);
  else if (Buffer[1] == 0x30 && Buffer[2] == 0x38) mqttpumpidchange(pumpid8);
}


void mqttpumpidchange(int pumpid) {
  if (pumpid == 1) {
    ppbuffer[0] = 0x30;
    ppbuffer[1] = 0x31;
  } else if (pumpid == 2) {
    ppbuffer[0] = 0x30;
    ppbuffer[1] = 0x32;
  } else if (pumpid == 3) {
    ppbuffer[0] = 0x30;
    ppbuffer[1] = 0x33;
  } else if (pumpid == 4) {
    ppbuffer[0] = 0x30;
    ppbuffer[1] = 0x34;
  } else if (pumpid == 5) {
    ppbuffer[0] = 0x30;
    ppbuffer[1] = 0x35;
  } else if (pumpid == 6) {
    ppbuffer[0] = 0x30;
    ppbuffer[1] = 0x36;
  } else if (pumpid == 7) {
    ppbuffer[0] = 0x30;
    ppbuffer[1] = 0x37;
  } else if (pumpid == 8) {
    ppbuffer[0] = 0x30;
    ppbuffer[1] = 0x38;
  } else if (pumpid == 9) {
    ppbuffer[0] = 0x30;
    ppbuffer[1] = 0x39;
  } else if (pumpid == 10) {
    ppbuffer[0] = 0x31;
    ppbuffer[1] = 0x30;
  } else if (pumpid == 11) {
    ppbuffer[0] = 0x31;
    ppbuffer[1] = 0x31;
  } else if (pumpid == 12) {
    ppbuffer[0] = 0x31;
    ppbuffer[1] = 0x32;
  } else if (pumpid == 13) {
    ppbuffer[0] = 0x31;
    ppbuffer[1] = 0x33;
  } else if (pumpid == 14) {
    ppbuffer[0] = 0x31;
    ppbuffer[1] = 0x34;
  } else if (pumpid == 15) {
    ppbuffer[0] = 0x31;
    ppbuffer[1] = 0x35;
  } else if (pumpid == 16) {
    ppbuffer[0] = 0x31;
    ppbuffer[1] = 0x36;
  } else if (pumpid == 17) {
    ppbuffer[0] = 0x31;
    ppbuffer[1] = 0x37;
  } else if (pumpid == 18) {
    ppbuffer[0] = 0x31;
    ppbuffer[1] = 0x38;
  } else if (pumpid == 19) {
    ppbuffer[0] = 0x31;
    ppbuffer[1] = 0x39;
  } else if (pumpid == 20) {
    ppbuffer[0] = 0x32;
    ppbuffer[1] = 0x30;
  } else if (pumpid == 21) {
    ppbuffer[0] = 0x32;
    ppbuffer[1] = 0x31;
  } else if (pumpid == 22) {
    ppbuffer[0] = 0x32;
    ppbuffer[1] = 0x32;
  } else if (pumpid == 23) {
    ppbuffer[0] = 0x32;
    ppbuffer[1] = 0x33;
  } else if (pumpid == 24) {
    ppbuffer[0] = 0x32;
    ppbuffer[1] = 0x34;
  } else if (pumpid == 25) {
    ppbuffer[0] = 0x32;
    ppbuffer[1] = 0x35;
  } else if (pumpid == 26) {
    ppbuffer[0] = 0x32;
    ppbuffer[1] = 0x36;
  } else if (pumpid == 27) {
    ppbuffer[0] = 0x32;
    ppbuffer[1] = 0x37;
  } else if (pumpid == 28) {
    ppbuffer[0] = 0x32;
    ppbuffer[1] = 0x38;
  } else if (pumpid == 29) {
    ppbuffer[0] = 0x32;
    ppbuffer[1] = 0x39;
  } else if (pumpid == 33) {
    ppbuffer[0] = 0x33;
    ppbuffer[1] = 0x30;
  } else if (pumpid == 31) {
    ppbuffer[0] = 0x33;
    ppbuffer[1] = 0x31;
  } else if (pumpid = 32) {
    ppbuffer[0] = 0x33;
    ppbuffer[1] = 0x32;
  }
}


void initWiFi() {
  for (int i = 0; i < 50; i++) {
    ssidtemp[i] = EEPROM.read(i);
    // Serial.print(getDataEEPROM(i));
  }
  for (int i = 50; i < 100; i++) {
    passtemp[i - 50] = EEPROM.read(i);
    // Serial.print(getDataEEPROM(i));
  }
  Serial.print(ssidtemp);
  Serial.print("  ");
  Serial.println(passtemp);
  // WiFi.begin(ssidtemp, passtemp);
  WiFi.begin("POS_Server", "asdffdsa");
  Serial.print("Connecting to WiFi ..");
  wifitrytime = 0;
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

// cloud connection
void reconnect() {
  // Loop until we're reconnected
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
      client.subscribe("1etpos/#");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");  // Wait 5 seconds before retrying
      delay(1000);
    }
  }
}


void callback(char* topic, byte* payload, unsigned int length) {
  incommingMessage = "";

  for (int j = 0; j < length; j++) incommingMessage += (char)payload[j];

  // Serial.println("Message arrived [" + String(topic) + "]" + incommingMessage);


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

  if (String(topic) == String(pumpapprobuf)) {  // pump req appro
    pumappprofun();
    rxledonoff();
  }
  if (String(topic) == String(pricechange)) {  // price change
    pricechangefun();
    rxledonoff();
  }

  if (String(topic) == String(preset)) {  // price change
    presetfun();
    rxledonoff();
  }

  if (String(topic) == String(myreload)) {  // reload change
    // reloadfun();
    reloadcount = true;
    rxledonoff();
  }

  if (String(topic) == String(presetcancel)) {  // reload change
    presetcancelfun();
    rxledonoff();
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

  // incommingMessage = "";
  if (String(topic) == "detpos/local_server/whreq") {  // whole req
    if (incommingMessage == String(devicenum)) {
      rxledonoff();
      sendpermission = true;
      client.publish(whreqbuf, devicebuf);
      txledonoff();
    }
  }
}


void pumappprofun() {
  char pumpapproArray[8];
  incommingMessage.toCharArray(pumpapproArray, incommingMessage.length() + 1);
  Serial.print("Appro  is ");

  Serial.println(pumpapproArray);

  charArray[0] = pumpapproArray[0];
  charArray[1] = pumpapproArray[1];

  pumpmqttnumchange();

  if (data_sequence[0] == 0x30 && data_sequence[1] == 0x31) {
    Serial2.write(APappro1, 8);
    Serial.println("U write 1 ");
    delay(100);
    Serial2.write(APappro1, 8);
    app1 = true;
  } else if (data_sequence[0] == 0x30 && data_sequence[1] == 0x32) {
    Serial2.write(APappro2, 8);
    Serial.println("U write 2 ");
    delay(100);
    Serial2.write(APappro2, 8);
    app2 = true;
  } else if (data_sequence[0] == 0x30 && data_sequence[1] == 0x33) {
    Serial2.write(APappro3, 8);
    delay(100);
    Serial2.write(APappro3, 8);
    Serial.println("U write 3 ");
    app3 = true;
  } else if (data_sequence[0] == 0x30 && data_sequence[1] == 0x34) {
    Serial2.write(APappro4, 8);
    delay(100);
    Serial2.write(APappro4, 8);

    Serial.println("U write 4 ");
    app4 = true;
  } else if (data_sequence[0] == 0x30 && data_sequence[1] == 0x35) {
    Serial2.write(APappro5, 8);
    Serial.println("U write 5 ");
    delay(100);
    Serial2.write(APappro5, 8);
    app5 = true;
  } else if (data_sequence[0] == 0x30 && data_sequence[1] == 0x36) {
    Serial2.write(APappro6, 8);
    Serial.println("U write 6 ");
    delay(100);
    Serial2.write(APappro6, 8);
    app6 = true;
  } else if (data_sequence[0] == 0x30 && data_sequence[1] == 0x37) {
    Serial2.write(APappro7, 8);
    Serial.println("U write 7 ");
    delay(100);
    Serial2.write(APappro7, 8);
    app7 = true;
  } else if (data_sequence[0] == 0x30 && data_sequence[1] == 0x38) {
    Serial2.write(APappro8, 8);
    Serial.println("U write 8 ");
    delay(100);
    Serial2.write(APappro8, 8);
    app8 = true;
  }
}

void pricechangefun() {

  //  char charArray[7];

  incommingMessage.toCharArray(charArray, incommingMessage.length() + 1);

  Serial.print("pc is ");
  Serial.println(charArray);

  pumpmqttnumchange();

  data_sequence[5] = charArray[2];
  data_sequence[6] = charArray[3];
  data_sequence[7] = charArray[4];
  data_sequence[8] = charArray[5];
  lrc = 0x00;

  for (int j = 0; j < sizeof(data_sequence); j++) {
    Serial.print(data_sequence[j], HEX);
    Serial.print(" ");
  }
  Serial.println();

  Serial.println(data_sequence);
  Serial.println(sizeof(data_sequence));


  // Calculate LRC checksum
  for (int j = 0; j < sizeof(data_sequence); j++) {
    lrc ^= data_sequence[j];
  }
  // Print LRC checksum in hexadecimal and decimal format
  Serial.print("LRC Checksum (Hex): 0x");
  if (lrc < 0x10) {
    Serial.print("0");
  }
  Serial.println(lrc, HEX);

  Serial.print("LRC Checksum (Decimal): ");
  Serial.println(lrc);

  send_sequence[1] = data_sequence[0];
  send_sequence[2] = data_sequence[1];

  send_sequence[6] = data_sequence[5];
  send_sequence[7] = data_sequence[6];
  send_sequence[8] = data_sequence[7];
  send_sequence[9] = data_sequence[8];

  send_sequence[11] = lrc;

  for (int j = 0; j < 12; j++) {
    Serial.print(send_sequence[j]);
    Serial.print(" ");
  }
  Serial.println();

  for (int j = 0; j < 12; j++) {
    Serial.print(send_sequence[j], HEX);
    Serial.print(" ");
  }
  Serial.println();

  Serial2.write(send_sequence, 12);
  delay(100);
  Serial2.write(send_sequence, 12);
}


void presetfun() {

  incommingMessage.toCharArray(presetmqttary, incommingMessage.length() + 1);
  Serial.print("preset is ");
  Serial.println(presetmqttary);

  pumpmqttnumchangeinpreset();

  stary[0] = data_sequence[0];
  stary[1] = data_sequence[1];

  if (presetmqttary[2] == 0x50) stary[5] = 0x41;
  else if (presetmqttary[2] == 0x4C) stary[5] = 0x51;

  stary[6] = presetmqttary[3];
  stary[7] = presetmqttary[4];
  stary[8] = presetmqttary[5];
  stary[9] = presetmqttary[6];
  stary[10] = presetmqttary[7];
  stary[11] = presetmqttary[8];
  stary[12] = presetmqttary[9];

  lrc = 0x00;

  for (int j = 0; j < sizeof(stary); j++) {
    Serial.print(stary[j], HEX);
    Serial.print(" ");
  }
  Serial.println();

  Serial.println(stary);
  Serial.println(sizeof(stary));


  // Calculate LRC checksum
  for (int j = 0; j < sizeof(stary); j++) {
    lrc ^= stary[j];
  }
  // Print LRC checksum in hexadecimal and decimal format
  Serial.print("LRC Checksum (Hex): 0x");
  if (lrc < 0x10) {
    Serial.print("0");
  }
  Serial.println(lrc, HEX);

  Serial.print("LRC Checksum (Decimal): ");
  Serial.println(lrc);

  starysend_sequence[1] = data_sequence[0];
  starysend_sequence[2] = data_sequence[1];

  starysend_sequence[6] = stary[5];
  starysend_sequence[7] = stary[6];
  starysend_sequence[8] = stary[7];
  starysend_sequence[9] = stary[8];
  starysend_sequence[10] = stary[9];
  starysend_sequence[11] = stary[10];
  starysend_sequence[12] = stary[11];
  starysend_sequence[13] = stary[12];

  starysend_sequence[14] = stary[13];

  starysend_sequence[15] = lrc;

  for (int j = 0; j <= 15; j++) {
    Serial.print(starysend_sequence[j]);
    Serial.print(" ");
  }
  Serial.println();

  for (int j = 0; j <= 15; j++) {
    Serial.print(starysend_sequence[j], HEX);
    Serial.print(" ");
  }
  Serial.println();

  Serial2.write(starysend_sequence, 16);
  delay(100);
  Serial2.write(starysend_sequence, 16);
}

void pumpmqttnumchange() {  //change incomming pumpid(0 - 32) from Mqtt to Device pumpid(0 - 8)
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

  data_sequence[0] = 0x30;
  data_sequence[1] = 0x30;

  if (pumpid1 == pumpmqttnum) {
    data_sequence[0] = 0x30;
    data_sequence[1] = 0x31;
  } else if (pumpid2 == pumpmqttnum) {
    data_sequence[0] = 0x30;
    data_sequence[1] = 0x32;
  } else if (pumpid3 == pumpmqttnum) {
    data_sequence[0] = 0x30;
    data_sequence[1] = 0x33;
  } else if (pumpid4 == pumpmqttnum) {
    data_sequence[0] = 0x30;
    data_sequence[1] = 0x34;
  } else if (pumpid5 == pumpmqttnum) {
    data_sequence[0] = 0x30;
    data_sequence[1] = 0x35;
  } else if (pumpid6 == pumpmqttnum) {
    data_sequence[0] = 0x30;
    data_sequence[1] = 0x36;
  } else if (pumpid7 == pumpmqttnum) {
    data_sequence[0] = 0x30;
    data_sequence[1] = 0x37;
  } else if (pumpid8 == pumpmqttnum) {
    data_sequence[0] = 0x30;
    data_sequence[1] = 0x38;
  }
}


void pumpmqttnumchangeinpreset() {  //change incomming pumpid(0 - 32) from Mqtt to Device pumpid(0 - 8)
  if (presetmqttary[0] == 0x30 && presetmqttary[1] == 0x31) {
    pumpmqttnum = 1;
  } else if (presetmqttary[0] == 0x30 && presetmqttary[1] == 0x32) {
    pumpmqttnum = 2;
  } else if (presetmqttary[0] == 0x30 && presetmqttary[1] == 0x33) {
    pumpmqttnum = 3;
  } else if (presetmqttary[0] == 0x30 && presetmqttary[1] == 0x34) {
    pumpmqttnum = 4;
  } else if (presetmqttary[0] == 0x30 && presetmqttary[1] == 0x35) {
    pumpmqttnum = 5;
  } else if (presetmqttary[0] == 0x30 && presetmqttary[1] == 0x36) {
    pumpmqttnum = 6;
  } else if (presetmqttary[0] == 0x30 && presetmqttary[1] == 0x37) {
    pumpmqttnum = 7;
  } else if (presetmqttary[0] == 0x30 && presetmqttary[1] == 0x38) {
    pumpmqttnum = 8;
  } else if (presetmqttary[0] == 0x30 && presetmqttary[1] == 0x39) {
    pumpmqttnum = 9;
  } else if (presetmqttary[0] == 0x31 && presetmqttary[1] == 0x30) {
    pumpmqttnum = 10;
  } else if (presetmqttary[0] == 0x31 && presetmqttary[1] == 0x31) {
    pumpmqttnum = 11;
  } else if (presetmqttary[0] == 0x31 && presetmqttary[1] == 0x32) {
    pumpmqttnum = 12;
  } else if (presetmqttary[0] == 0x31 && presetmqttary[1] == 0x33) {
    pumpmqttnum = 13;
  } else if (presetmqttary[0] == 0x31 && presetmqttary[1] == 0x34) {
    pumpmqttnum = 14;
  } else if (presetmqttary[0] == 0x31 && presetmqttary[1] == 0x35) {
    pumpmqttnum = 15;
  } else if (presetmqttary[0] == 0x31 && presetmqttary[1] == 0x36) {
    pumpmqttnum = 16;
  } else if (presetmqttary[0] == 0x31 && presetmqttary[1] == 0x37) {
    pumpmqttnum = 17;
  } else if (presetmqttary[0] == 0x31 && presetmqttary[1] == 0x38) {
    pumpmqttnum = 18;
  } else if (presetmqttary[0] == 0x31 && presetmqttary[1] == 0x39) {
    pumpmqttnum = 19;
  } else if (presetmqttary[0] == 0x32 && presetmqttary[1] == 0x30) {
    pumpmqttnum = 20;
  } else if (presetmqttary[0] == 0x32 && presetmqttary[1] == 0x31) {
    pumpmqttnum = 21;
  } else if (presetmqttary[0] == 0x32 && presetmqttary[1] == 0x32) {
    pumpmqttnum = 22;
  } else if (presetmqttary[0] == 0x32 && presetmqttary[1] == 0x33) {
    pumpmqttnum = 23;
  } else if (presetmqttary[0] == 0x32 && presetmqttary[1] == 0x34) {
    pumpmqttnum = 24;
  } else if (presetmqttary[0] == 0x32 && presetmqttary[1] == 0x35) {
    pumpmqttnum = 25;
  } else if (presetmqttary[0] == 0x32 && presetmqttary[1] == 0x36) {
    pumpmqttnum = 26;
  } else if (presetmqttary[0] == 0x32 && presetmqttary[1] == 0x37) {
    pumpmqttnum = 27;
  } else if (presetmqttary[0] == 0x32 && presetmqttary[1] == 0x38) {
    pumpmqttnum = 28;
  } else if (presetmqttary[0] == 0x32 && presetmqttary[1] == 0x39) {
    pumpmqttnum = 29;
  } else if (presetmqttary[0] == 0x33 && presetmqttary[1] == 0x30) {
    pumpmqttnum = 30;
  } else if (presetmqttary[0] == 0x33 && presetmqttary[1] == 0x31) {
    pumpmqttnum = 31;
  } else if (presetmqttary[0] == 0x33 && presetmqttary[1] == 0x32) {
    pumpmqttnum = 32;
  }

  data_sequence[0] = 0x30;
  data_sequence[1] = 0x30;

  if (pumpid1 == pumpmqttnum) {
    data_sequence[0] = 0x30;
    data_sequence[1] = 0x31;
  } else if (pumpid2 == pumpmqttnum) {
    data_sequence[0] = 0x30;
    data_sequence[1] = 0x32;
  } else if (pumpid3 == pumpmqttnum) {
    data_sequence[0] = 0x30;
    data_sequence[1] = 0x33;
  } else if (pumpid4 == pumpmqttnum) {
    data_sequence[0] = 0x30;
    data_sequence[1] = 0x34;
  } else if (pumpid5 == pumpmqttnum) {
    data_sequence[0] = 0x30;
    data_sequence[1] = 0x35;
  } else if (pumpid6 == pumpmqttnum) {
    data_sequence[0] = 0x30;
    data_sequence[1] = 0x36;
  } else if (pumpid7 == pumpmqttnum) {
    data_sequence[0] = 0x30;
    data_sequence[1] = 0x37;
  } else if (pumpid8 == pumpmqttnum) {
    data_sequence[0] = 0x30;
    data_sequence[1] = 0x38;
  }
}

void hmisetup() {

  if (Serial.available()) {
    for (int j = 0; j <= 50; j++)  //this loop will store whole frame in buffer array.
    {
      Buffer[j] = Serial.read();
      Serial.print(Buffer[j], HEX);
      Serial.print(" ");
    }
    Serial.println();

    if (Buffer[4] == 0x10 && Buffer[8] == 0x01) {
      ESP.restart();
      Serial.println("restart");
    } else if (Buffer[4] == 0x13) {  // wifi ssid
      for (int j = 0; j < 50; j++) ssidBuf[j] = 0;

      for (int j = 0; j < 50; j++) {
        if (Buffer[j + 9] == 0xFF) break;
        ssidBuf[j] = Buffer[j + 9];
      }
      Serial.println(ssidBuf);

      Serial.write(showSSID, 6);
      Serial.write(ssidBuf, 30);
      Serial.write(showSSID, 6);
      Serial.write(ssidBuf, 30);

    } else if (Buffer[4] == 0x14) {  // wifi password
      for (int j = 0; j < 50; j++) passBuf[j] = 0;

      for (int j = 0; j < 50; j++) {
        if (Buffer[j + 9] == 0xFF) break;
        passBuf[j] = Buffer[j + 9];
      }
      Serial.println(passBuf);
      Serial.write(showPASS, 6);
      Serial.write(passBuf, 50);
      Serial.write(showPASS, 6);
      Serial.write(passBuf, 50);
    } else if (Buffer[4] == 0x11) {  // wifi connect
      WiFi.hostname("device1");
      WiFi.begin(ssidBuf, passBuf);
      wifitrytime = 0;
      while (WiFi.status() != WL_CONNECTED && wifitrytime != 15) {
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
        writeString(0, ssidBuf);
        writeString(50, passBuf);


        // digitalWrite(wifiled, HIGH);

        Serial.write(page, 9);
        Serial.write(0x00);
        Serial.println("home page");
        delay(1000);
      }
    } else if (Buffer[4] == 0x31) {  // devnumber
      devicenum = Buffer[8];
      Serial.println(devicenum);
    } else if (Buffer[4] == 0x41) {  // pumpid 1
      pumpid1 = Buffer[8];
      Serial.println(pumpid1);
    } else if (Buffer[4] == 0x42) {  // pumpid 2
      pumpid2 = Buffer[8];
      Serial.println(pumpid2);
    } else if (Buffer[4] == 0x43) {  // pumpid 3
      pumpid3 = Buffer[8];
      Serial.println(pumpid3);
    } else if (Buffer[4] == 0x44) {  // pumpid 4
      pumpid4 = Buffer[8];
      Serial.println(pumpid4);
    } else if (Buffer[4] == 0x45) {  // pumpid 5
      pumpid5 = Buffer[8];
      Serial.println(pumpid5);
    } else if (Buffer[4] == 0x46) {  // pumpid 6
      pumpid6 = Buffer[8];
      Serial.println(pumpid6);
    } else if (Buffer[4] == 0x47) {  // pumpid 7
      pumpid7 = Buffer[8];
      Serial.println(pumpid7);
    } else if (Buffer[4] == 0x48) {  // pumpid 8
      pumpid8 = Buffer[8];
      Serial.println(pumpid8);
    } else if (Buffer[4] == 0x40 && Buffer[8] == 0x00) {  // 2noz
      nozzlenum = 2;
    } else if (Buffer[4] == 0x40 && Buffer[8] == 0x01) {  // 4noz
      nozzlenum = 4;
    } else if (Buffer[4] == 0x40 && Buffer[8] == 0x02) {  // 8noz
      nozzlenum = 8;
    } else if (Buffer[4] == 0x40 && Buffer[8] == 0x03) {  // save
      saveall();
    }
  }
}


void saveall() {
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


void writeString(char add, String data) {
  int _size = data.length();
  int j;
  for (j = 0; j < _size; j++) {
    EEPROM.write(add + j, data[j]);
    EEPROM.commit();
  }
  EEPROM.write(add + _size, '\0');  //Add termination null character for String Data
  EEPROM.commit();
  Serial.println("Wrtiting ssid and pass to eeprom");
}


//======================================= publising as strings
void publishMessage(const char* topic, String payload, boolean retained) {
  client.publish(topic, (byte*)payload.c_str(), 10, true);
  Serial.println("Message publised [" + String(topic) + "]: " + payload);
}

void mqtt_setup() {
  for (int j = 0; j < 2; j++) {
    if (!client.connected()) {
      reconnect();
    } else {
      Serial.println("Cloud is connected");
    }
  }
}