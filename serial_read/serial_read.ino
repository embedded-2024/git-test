
unsigned char data[4] = { 0x31, 0x32 , 0x33, 0x34 };
char Buffer[10];
int count;

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 16, 17);

  pinMode(15, OUTPUT);

  digitalWrite(15, HIGH);
  Serial2.write(data, 4);
  delay(2);
  digitalWrite(15, LOW);
}

void loop() {

  delay(10);


  digitalWrite(15, LOW);

  if (Serial2.available()) {

    Buffer[count] = Serial2.read();

    delay(2);


    Serial.print(count);
    Serial.print("/");
    Serial.print(Buffer[count], HEX);
    Serial.print(" ");

    count++;

    if (Buffer[count-1] == 0xff) count = 0;

    if (Buffer[count - 2] == 0x30 && Buffer[count - 1] == 0x31) {
      count = 0;
      digitalWrite(15, HIGH);
      Serial2.write(data, 4);
      delay(3);
      digitalWrite(15, LOW);

      Serial.println();
    }
  }

}
