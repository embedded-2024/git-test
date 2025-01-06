
// uint8_t pump_state_1[2] = { 0x01, 0x01 };
uint8_t pump_state_2[2] = { 0x01, 0x02 };

unsigned char pump_state_1[2] = { 0x00, 0x01 };


// byte pump_state_1 = 0x01;

unsigned char auth_1[2] = { 0x31, 0x31 };
unsigned char auth_2[2] = { 0x31, 0x32 };

unsigned long start_time;

#define RXD2 16
#define TXD2 17

#define wifiled 33
#define powerled 32
#define TXled 27
#define RXled 26

char Buffer[120];

int buff_count = 0;
int msg_count = 0;

void setup() {
  Serial.begin(115200);
  Serial2.begin(5787, SERIAL_8E1, RXD2, TXD2);

  pinMode(15, OUTPUT);

  pinMode(wifiled, OUTPUT);
  pinMode(powerled, OUTPUT);
  pinMode(TXled, OUTPUT);
  pinMode(RXled, OUTPUT);

  // Serial2.write(pump_state_1, 2);
  // delay(100);
  // Serial.println("Done");
}

void loop() {

  delay(3);

  if (Serial2.available() > 0) {
    Serial.println("Data is comming");

    digitalWrite(powerled, HIGH);
    digitalWrite(wifiled, HIGH);
    digitalWrite(TXled, HIGH);

    Buffer[buff_count] = Serial2.read();
    delay(10);

    Serial.print(buff_count);
    Serial.print("/");
    Serial.print(Buffer[buff_count], HEX);
    Serial.print(" ");

    buff_count++;

    // if (Buffer[buff_count - 1] == 0x61 || Buffer[buff_count - 1] == 0x62) buff_count = 0;

    // if (Buffer[buff_count - 1] == 0x71) Serial.println("nozzle one is working");

    // if (Buffer[buff_count - 1] == 0x72) Serial.println("nozzle two is working");

    // pump_fun();


  } else {

    if (millis() / 1000 - start_time > 0) {
      Serial.println("Waiting reply from dispenser");

      digitalWrite(15, HIGH);
      Serial2.write(pump_state_1, sizeof(pump_state_1));

      delay(20);
      digitalWrite(15, LOW);

      start_time = millis() / 1000;
    }

    // Serial.println(millis() / 1000 - start_time);

  }

  // delay(100);
}


void pump_fun() {

  delay(50);

  if (millis() / 1000 - start_time > 1) {

    msg_count++;

    Serial.printf("message count is %d\n", msg_count);

    start_time = millis() / 1000;
  }


  // if(msg_count == 1) Serial2.write(pump_state_1, 2);
  // else if (msg_count == 2) Serial2.write(pump_state_2, 2);


  // if (msg_count == 2) msg_count = 0;
  
}



void pump_preset() {

  // Serial2.write(pump_state_1);

  Serial2.write(auth_1, 2);
  delay(2);
}