char mqttdatabuf[50];
// char mqttdatabuf[50];
String liter;
String price;
String price_state;
String total_state;
String total_amount;

void setup() {
  Serial.begin(115200);
  // mqttpplive();
  mqttfinal();

}

void loop() {
  

}


void mqttfinal() {

  liter = 21111;
  price = 2000;
  price_state = 3000;
  total_state = 111111111;
  total_amount = 222222222;


  for (int y = 0; y < 50; y++) {
    mqttdatabuf[y] = 0;
  }

  int arycount = 2;

  // mqttdatabuf[arycount++] = '0';
  // pumpidchange();

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
  if (liter.length() < 3) {
    
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


  Serial.print("before arr count is => ");   // ak add
  Serial.println(arycount);

  int y = total_state.length() - 2;

  for ( int i = 0 ; i < total_state.length(); i++) {

    if ( i == y ) {
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

   
}

void mqttpplive() {

  liter = "11111";
  price = "1000";

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


  for (int y = 0; y < 50; y++) {
    Serial.print(mqttdatabuf[y]);
    Serial.print(" ");
  }


  // client.publish(pplive, mqttdatabuf);

  // txledonoff();
}
