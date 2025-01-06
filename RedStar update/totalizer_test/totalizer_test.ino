
// String price;
// String price_state;
// String total_state;
// String total_amount;
// String liter;


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

int arycount;

void setup() {
  Serial.begin(115200);
  send_read_total();
  mqttfinal();
}

void loop() {
  

}


void mqttfinal() {

  price = "5000";
  liter = "1111";
  price_state = "3200";
  // total_state = "3531222";
  // total_state = "1153";
  // total_amount = "3788245170"; 


  for (int y = 0; y < 50; y++) {
    mqttdatabuf[y] = 0;
  }

  int arycount = 0;

  // pumpidchange();
  mqttdatabuf[arycount++] = 0x30;
  mqttdatabuf[arycount++] = 0x31;

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

  Serial.print("liter length => ");
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

  Serial.print("your total length is => ");
  Serial.println(total_state.length());

  // Serial.println("////////////////////");

  // Serial.print("your arycount length is => ");
  // Serial.println(arycount);

  // arycount++;
  Serial.print("before arr count is => ");
  Serial.println(arycount);

  int y = total_state.length() - 3;
  bool tt = false;

  for ( int i = 0 ; i < total_state.length(); i++) {

    
    if ( i == y ) {
      mqttdatabuf[arycount++] = '.';
      tt = true;
       
    }
    if (tt) Serial.println(arycount);
    mqttdatabuf[arycount++] = total_ary[i];
    Serial.println(arycount);
    Serial.println(" ");
  }

  Serial.print("after arr count is => ");
  Serial.println(arycount);

  mqttdatabuf[arycount] = 'A';
  arycount++;

  for (int y = total_amount.length() - 1; y >= 0; y--) {
    mqttdatabuf[arycount + y] = total_amount_ary[y];
  }

  Serial.println();

  for (int y = 0; y < 50; y++) {
    if (mqttdatabuf[y] == 0) break;
    Serial.print(y);
    Serial.print("/");
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
 

void send_read_total() {
  
  unsigned char Buffer[30] = { 0x03, 0x8E, 0x6E, 0x0F, 0x50, 0x00, 0x00, 0x00, 0x55, 0x00, 0x00, 0x49, 0x36, 0x00, 0x88, 0x10, 0x00, 0x48 };

  total_state = hexArrayToString(Buffer, sizeof(Buffer));
  total_amount = hexArrayToString_1(Buffer, sizeof(Buffer));

  Serial.println("your result is");
  Serial.println(total_state);
   
}


String hexArrayToString(byte hexArray[], int size) {
  String result = "";
  bool firsttime = true;
  for (int i = 9; i > 3; i--) {

    if (hexArray[i] == 0x00 && firsttime == true) continue;
    else firsttime = false;
    // Convert each hex byte to its decimal representation and append to the result string

    if(String(hexArray[i], HEX).length()<2 && String(hexArray[i], HEX)!= "0") result += "0";

    result += String(hexArray[i], HEX);

    if(String(hexArray[i], HEX)== "0") result += "0";

  }
  return result;
}

String hexArrayToString_1(byte hexArray[], int size) {
  String result = "";
  bool firsttime = true;
  for (int i = 15; i > 9; i--) {

    if (hexArray[i] == 0x00 && firsttime == true) continue;
    else firsttime = false;
    // Convert each hex byte to its decimal representation and append to the result string

    if(String(hexArray[i], HEX).length()<2 && String(hexArray[i], HEX)!= "0") result += "0";

    result += String(hexArray[i], HEX);

    if(String(hexArray[i], HEX)== "0") result += "0";

  }
  return result;
}

 