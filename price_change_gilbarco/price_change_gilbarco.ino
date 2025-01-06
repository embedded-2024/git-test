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

unsigned char pump_select[1] = { 0x21 };

unsigned char price[11] = { 0xFF, 0xE5, 0xF4, 0xF7, 0xE0, 0xE0, 0xE0, 0xE3, 0xFB, 0xE1, 0xF0 };


bool active_check = false;

// unsigned char dd[2] = {0x31, 0x31};

void setup() {

  Serial.begin(115200);
  Serial2.begin(5787, SERIAL_8E1, 16, 17);

  pinMode(15, OUTPUT);


  delay(2000);

  digitalWrite(15, HIGH);

  Serial2.write(state_req_1, 1);
  delay(2);
  digitalWrite(15, LOW);
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

    // if (buffer[i - 1] == 0xF0) i = 0;

    if (buffer[i - 1] == 0x61) {

      Serial.println("i am working");

      digitalWrite(15, HIGH);
      Serial2.write(pump_select, 1);
      delay(2);
      digitalWrite(15, LOW);

      while (!Serial2.available()) {
        delay(2);
      }

      while (Serial2.available()) {
        i = 0;
        buffer[i] = Serial2.read();

        Serial.print(i);
        Serial.print("/");
        Serial.print(buffer[i], HEX);
        Serial.print(" ");
      }


      digitalWrite(15, HIGH);
      Serial2.write(price, 11);
      delay(2);
      digitalWrite(15, LOW);

      i = 0;
    }

    // if (buffer[i - 1] == 0x61 || buffer[i - 1] == 0x62) i = 0;

    // if (buffer[i - 1] == 0x91) {  // live one

    //   Serial.println();

    //   for (int i = 0; i < 151; i++) {
    //     buffer[i] = 0;
    //   }

    //   i = 0;

    //   digitalWrite(15, HIGH);
    //   Serial2.write(live_1, 1);
    //   delay(2);
    //   digitalWrite(15, LOW);

    //   while (!Serial2.available() > 0) {
    //     delay(2);
    //   }

    //   while (Serial2.available() > 0) {
    //     buffer[i] = Serial2.read();
    //     delay(2);

    //     Serial.print(i);
    //     Serial.print("/");
    //     Serial.print(buffer[i], HEX);
    //     Serial.print(" ");

    //     i++;

    //     if (buffer[i - 1] == 0xF0) i = 0;
    //   }
    // }

    // if (buffer[i - 1] == 0x92) {  // live two

    //   Serial.println();

    //   for (int i = 0; i < 151; i++) {
    //     buffer[i] = 0;
    //   }

    //   i = 0;

    //   digitalWrite(15, HIGH);
    //   Serial2.write(live_2, 1);
    //   delay(2);
    //   digitalWrite(15, LOW);

    //   while (!Serial2.available() > 0) {
    //     delay(2);
    //   }

    //   while (Serial2.available() > 0) {
    //     buffer[i] = Serial2.read();
    //     delay(2);

    //     Serial.print(i);
    //     Serial.print("/");
    //     Serial.print(buffer[i], HEX);
    //     Serial.print(" ");

    //     i++;

    //     if (buffer[i - 1] == 0xF0) i = 0;
    //   }
    // }

    // manage_msg();

    // if (buffer[i - 1] == 0xB1 || buffer[i - 1] == 0xA1) {

    //   Serial.println();

    //   i = 0;

    //   digitalWrite(15, HIGH);
    //   Serial2.write(tran_data, 1);
    //   delay(2);
    //   digitalWrite(15, LOW);

    //   while (!Serial2.available()) {
    //     delay(2);
    //   }

    //   while (Serial2.available() > 0) {

    //     // Serial.print("i am B1 tran");
    //     buffer[i] = Serial2.read();
    //     delay(5);

    //     Serial.print(i);
    //     Serial.print("/");
    //     Serial.print(buffer[i], HEX);
    //     Serial.print(" ");

    //     i++;

    //     if (buffer[i - 1] == 0xF0) i = 0;
    //   }

    //   Serial.println();

    //   digitalWrite(15, HIGH);
    //   Serial2.write(final_data, 1);
    //   delay(2);
    //   digitalWrite(15, LOW);

    //   while (!Serial2.available()) {
    //     delay(2);
    //   }

    //   while (Serial2.available() > 0) {

    //     buffer[i] = Serial2.read();
    //     delay(5);

    //     Serial.print(i);
    //     Serial.print("/");
    //     Serial.print(buffer[i], HEX);
    //     Serial.print(" ");

    //     i++;

    //     if (buffer[i - 1] == 0xF0) i = 0;
    //   }
    // }

    // if (buffer[i - 1] == 0xB2 || buffer[i - 1] == 0xA2) {

    //   Serial.println();

    //   i = 0;

    //   digitalWrite(15, HIGH);
    //   Serial2.write(tran_data_1, 1);
    //   delay(2);
    //   digitalWrite(15, LOW);

    //   while (!Serial2.available()) {
    //     delay(2);
    //   }

    //   while (Serial2.available() > 0) {

    //     // Serial.print("i am B1 tran");
    //     buffer[i] = Serial2.read();
    //     delay(5);

    //     Serial.print(i);
    //     Serial.print("/");
    //     Serial.print(buffer[i], HEX);
    //     Serial.print(" ");

    //     i++;

    //     if (buffer[i - 1] == 0xF0) i = 0;
    //   }

    //   Serial.println();

    //   digitalWrite(15, HIGH);
    //   Serial2.write(final_data_1, 1);
    //   delay(2);
    //   digitalWrite(15, LOW);

    //   while (!Serial2.available()) {
    //     delay(2);
    //   }

    //   while (Serial2.available() > 0) {

    //     buffer[i] = Serial2.read();
    //     delay(5);

    //     Serial.print(i);
    //     Serial.print("/");
    //     Serial.print(buffer[i], HEX);
    //     Serial.print(" ");

    //     i++;

    //     if (buffer[i - 1] == 0xF0) i = 0;
    //   }
    // }

    count++;
    // main_fun();

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
