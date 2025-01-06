unsigned char final_data_1[1] = { 0x51 };

char Buffer[150];

int count;

unsigned long start;

void setup() {
  Serial.begin(115200);
  Serial2.begin(5787, SERIAL_8E1, 16, 17);

  pinMode(15, OUTPUT);

  digitalWrite(15, HIGH);
  Serial2.write(final_data_1, 1);
  delay(2);
  digitalWrite(15, LOW);
}

void loop() {

  if (Serial2.available()) {
    start = millis() / 1000;
    Buffer[count] = Serial2.read();
    delay(3);

    Serial.print(count);
    Serial.print("/");
    Serial.print(Buffer[count], HEX);
    Serial.print(" ");

    count++;

    if (Buffer[count-1] == 0xF5 && Buffer[count- 6] == 0xF4) {
      Serial.println("I am done ");
      count = 0;
    }

    if (Buffer[count - 1] == 0xF0) {
      Serial.println("i am working");
      count = 0;
    }


  } else {
    if (millis() / 1000 - start > 2) {
      Serial.println("Waiting ");

      digitalWrite(15, HIGH);
      Serial2.write(final_data_1, 1);
      delay(2);
      digitalWrite(15, LOW);

      start = millis() / 1000;
    }
  }
}
