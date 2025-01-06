
#include <Arduino.h>


#define DATA_LENGTH 8
uint8_t bcdData[] = {0xE6, 0xE3, 0xE8, 0xE8, 0xE3, 0xE1, 0xE7, 0xE4}; // Example data

char mqttdatabuf[50];
String liter;
String price;

String bcd_converter(uint8_t* data, size_t length) {
  char number[9] = {0};
  int digitIndex = 0;


  for (int i = length - 1; i >= 0; --i) {
    char byte = data[i];

    
    if ((byte & 0xF0) == 0xE0) { 
      number[digitIndex++] = '0' + (byte & 0x0F);
    }
  }

  return String(number);
}

void setup() {
  Serial.begin(115200);

  price = bcd_converter(bcdData, DATA_LENGTH);

  mqttpplive();

  // int data = result.toInt();
  // Serial.print("Integer Value: ");
  // Serial.println(data);

}

void loop() {
  
}


void mqttpplive() {

  liter = "0";
  // price = "1000";

  for (int y = 0; y < 50; y++) {
    mqttdatabuf[y] = 0;
  }

  int arycount = 2;

  // pumpidchange();


  // mqttdatabuf[1] = '2';

  // pumpidchange
  // pumpidchange();
  // if (Buffer[0] == 0x01) mqttdatabuf[1] = '1';
  // else if (Buffer[0] == 0x02) mqttdatabuf[1] = '2';

  mqttdatabuf[0] = '0';
  mqttdatabuf[1] = '1';


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
    if (mqttdatabuf[y] == 0x00) break;
    Serial.print(mqttdatabuf[y]);
    Serial.print(" ");
  }


  // client.publish(pplive, mqttdatabuf);

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


  Serial.print("before arr count is => ");  // ak add
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

// #include <Arduino.h>

// int DATA_LENGTH = 8;

// char bcdData[8] = { 0xE6, 0xE3, 0xE8, 0xE8, 0xE3, 0xE1, 0xE7, 0xE4 };

// void setup() {
//   Serial.begin(115200);

//   char* dd = cal(bcdData);
//   Serial.printf(dd);
// }


// void loop() {
// }


// char* cal(char* buffer) {
//   // char number[10];
//   char* number = (char*)malloc(10);
//   int digitIndex = 0;


//   for (int i = DATA_LENGTH - 1; i >= 0; --i) {
//     uint8_t byte = buffer[i];

//     if ((byte & 0xF0) == 0xE0) {  // Check for E prefix
//       number[digitIndex++] = '0' + (byte & 0x0F);
//     }
//   }

//   // Insert decimal point
//   for (int i = 7; i >= 6; --i) {  // Shift digits for the decimal point
//     number[i + 1] = number[i];
//   }
//   number[6] = '.';

//   Serial.print("Converted Value: ");
//   Serial.println(number);

//   return number;
// }
