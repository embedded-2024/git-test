



unsigned char read_state_1[4] = { 0x01, 0x03, 0x12, 0xEA };
unsigned char read_state_2[4] = { 0x02, 0x03, 0x12, 0xE9 };

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

char Buffer[50];
int count;

unsigned long start_time;

void setup() {
  Serial.begin(115200);
  Serial2.begin(19200, SERIAL_8N1, 16, 17);



  Serial2.write(read_state_1, 4);
}

void loop() {

  delay(5);

  if (Serial2.available()) {

    start_time = millis() / 1000;

    Buffer[count] = Serial2.read();
    delay(2);

    Serial.print(count);
    Serial.print("/");
    Serial.print(Buffer[count], HEX);
    Serial.print(" ");



    count++;
    // if (Buffer[count - 1] == )

    // Serial.println(count);

    if (Buffer[count - 1] == 0x57) {
      // Serial.println("i am 57");
      for (int i = 0; i < 8; i++) {
        Buffer[count] = Serial2.read();
        Serial.print(count);
        Serial.print("//");
        Serial.print(Buffer[count], HEX);
        Serial.print(" ");
      }
      count = 0;
      pump_state();
    }


    if (Buffer[count - 1] == 0x77) {
      count = 0;
      for (int i = 0; i < 8; i++) {
        Buffer[count] = Serial2.read();
        Serial.print(count);
        Serial.print("//");
        Serial.print(Buffer[count], HEX);
        Serial.print(" ");
      }

      Serial2.write(send_app1, 4);
      delay(2);
      count = 0;


      while (Serial2.available()) {
        Buffer[count] = Serial2.read();
        Serial.print(count);
        Serial.print("///");
        Serial.print(Buffer[count], HEX);
        Serial.print(" ");
      }
      pump_state();
    }



  } else {

    if (millis() / 1000 - start_time > 1) {
      Serial.println("Waiting reply from dispenser");
      Serial2.write(read_state_1, 4);
      start_time = millis() / 1000;
    }
  }
}

void pump_state() {
  delay(20);

  Serial2.write(read_state_1, 4);
  delay(2);
  Serial.println("Send Read State");
}
