unsigned char data[33] = { 0xFF, 0xF3, 0xF8, 0xEA, 0xE0, 0xE0, 0xE0, 0xE0, 0xF6, 0xE0, 0xF4, 0xF7, 0xE0, 0xE8, 0xE2, 0xE3, 0xF9, 0xE6, 0xE5, 0xE4, 0xE3, 0xE2, 0xE1, 0xFA, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xFB, 0xE0, 0xF0 };

unsigned char total[32] = { 0xFF, 0xF6, 0xE0, 0xF9, 0xE3, 0xE2, 0xE4, 0xE5, 0xE4, 0xE1, 0xE7, 0xE4, 0xFA, 0xE9, 0xE2, 0xE0, 0xE5, 0xE1, 0xE8, 0xE2, 0xE0, 0xF4, 0xE0, 0xE4, 0xE3, 0xE3, 0xF5, 0xE0, 0xE0, 0xE1, 0xE0, 0xF6 };

char pump_price_ary[4];
char tran_price_ary[6];
char tran_volume_ary[6];
char total_volume_ary[8];
char total_amount_ary[8];

char mqttdatabuf[50];

String price;
String liter;
String price_state;
String total_state;
String total_amount;


String tt_price;
String total_price;
String total_liter;

String totalizer_amount;
String totalizer_volume;

void setup() {
  Serial.begin(115200);

  main_fun();
}


void loop() {

}

void main_fun() {

  for (int i = 0; i < 33; i++) {

    switch (data[i]) {

      case 0xF7:
        // Serial.println("i am F7");
        i++;
        for (int j = 0; j < 4; j++) {
          pump_price_ary[j] = data[i + j];

          Serial.print(pump_price_ary[j], HEX);
          Serial.print(" ");
        }
        Serial.println();

        break;

      case 0xF9:
        i++;
        for (int j = 0; j < 6; j++) {
          tran_price_ary[j] = data[i + j];

          Serial.print(tran_price_ary[j], HEX);
          Serial.print(" ");
        }
        Serial.println();

        break;

      case 0xFA:
        i++;
        for (int j = 0; j < 6; j++) {
          tran_volume_ary[j] = data[i + j];

          Serial.print(tran_volume_ary[j], HEX);
          Serial.print(" ");
        }
        Serial.println();
        break;
    }
  }


  price_state = bcd_converter(pump_price_ary, 4);
  price = bcd_converter(tran_price_ary, 6);
  liter = bcd_converter(tran_volume_ary, 6);

  Serial.println(price_state);
  Serial.println(price);
  Serial.println(liter);


  for (int i = 0; i < 32; i++) {
    switch (total[i]) {
      case 0xF9:
        i++;
        for (int j = 0; j < 8; j++) {
          total_volume_ary[j] = total[j + i];
          Serial.print(total_volume_ary[j], HEX);
          Serial.print(" ");
        }
        Serial.println();

        break;

      case 0xFA:
        i++;
        for (int j = 0; j < 8; j++) {
          total_amount_ary[j] = total[j + i];
          Serial.print(total_amount_ary[j], HEX);
          Serial.print(" ");
        }

        Serial.println();
        break;
    }

    if (total[i] == 0xF5) {
      Serial.println("bye .................");
    }
  }

  total_amount = bcd_converter(total_amount_ary, 8);
  total_state = bcd_converter(total_volume_ary, 8);

  Serial.println(" /////////////////////////////////// ");
  Serial.println(total_amount);
  Serial.println(total_state);


}

void mqttfinal() {


  for (int y = 0; y < 50; y++) {
    mqttdatabuf[y] = 0;
  }

  int arycount = 2;

  // pumpidchange();

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
  if (liter.length() < 3) {
    //
    mqttdatabuf[arycount++] = '.';
    for (int y = 0; y < liter.length(); y++) {
      mqttdatabuf[arycount++] = liter_ary[y];
    }
  } else {
    for (int y = 0; y < liter.length() - 2; y++) {
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

  Serial.print("before arr count is => "); 
  Serial.println(arycount);

  int y = total_state.length() - 2;

  for (int i = 0; i < total_state.length(); i++) {

    if (i == y) {
      mqttdatabuf[arycount++] = '.';
    }
    mqttdatabuf[arycount++] = total_ary[i];
  }

  Serial.print("after arr count is => ");
  Serial.println(arycount);

  mqttdatabuf[arycount] = 'A';
  arycount++;

  for (int y = total_amount.length() - 1; y >= 0; y--) {
    mqttdatabuf[arycount + y] = total_amount_ary[y];
  }

  for (int y = 0; y < 50; y++) {
    Serial.print(mqttdatabuf[y]);
    Serial.print(" ");
  }

  // client.publish(ppfinal, mqttdatabuf);

}



String bcd_converter(char* data, size_t length) {
  char number[9] = { 0 };
  int digitIndex = 0;


  for (int i = length - 1; i >= 0; --i) {
    uint8_t byte = data[i];


    if ((byte & 0xF0) == 0xE0) {
      number[digitIndex++] = '0' + (byte & 0x0F);
    }
  }

  return String(number);
}
