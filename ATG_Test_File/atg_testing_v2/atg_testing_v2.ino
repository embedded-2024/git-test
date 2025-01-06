
unsigned char tank_1[8] = { 0x01, 0x69, 0x32, 0x30, 0x31, 0x30, 0x31, 0x03 };
unsigned char tank_2[8] = { 0x01, 0x69, 0x32, 0x30, 0x31, 0x30, 0x32, 0x03 };
unsigned char tank_3[8] = { 0x01, 0x69, 0x32, 0x30, 0x31, 0x30, 0x33, 0x03 };
unsigned char tank_4[8] = { 0x01, 0x69, 0x32, 0x30, 0x31, 0x30, 0x34, 0x03 };

char buffer[100];

char tank_product_ary[8];
char tank_volume_ary[8];
char remain_volume_ary[8];
char oil_height_ary[8];
char temperature_ary[8];
char water_level_ary[8];
char water_volume_ary[8];

int state_count;
int count;

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  Serial2.write(tank_1, 8);
  // Serial2.write(tank_4, 8);
  // Serial.println("Done");
}

void loop() {

  if (Serial2.available()) {

    buffer[count] = Serial2.read();
    delay(5);

    // Serial.print(count);
    // Serial.print("/");
    Serial.print(buffer[count]);
    Serial.print(" ");
    count++;

    if (buffer[count - 7] == 0x26 && buffer[count - 6] == 0x26 && buffer[count - 1] == 0x03) {

      Serial.println();


      for (int i = 0; i < 8; i++) {

        tank_volume_ary[i] = buffer[26 + i];
        tank_product_ary[i] = buffer[34 + i];
        remain_volume_ary[i] = buffer[42 + i];
        oil_height_ary[i] = buffer[50 + i];
        water_level_ary[i] = buffer[58 + i];
        temperature_ary[i] = buffer[66 + i];
        water_volume_ary[i] = buffer[74 + i];
      }

      Serial.println();

      float tank_volume = hexToIEEEFloat(tank_volume_ary);
      Serial.print("ATG tank volume is => ");
      Serial.println(tank_volume, 4);



      // for (int j = 0; j < 8; j++) {

      //   tank_product_ary[j] = buffer[34 + j];
      //   Serial.print(tank_product_ary[j]);
      // }
      // Serial.println();
 
      float tank_product = hexToIEEEFloat(tank_product_ary);
      Serial.print("ATG tank product is => ");
      Serial.println(tank_product, 4);

      // for (int j = 0; j < 8; j++) {

      //   remain_volume_ary[j] = buffer[42 + j];
      //   Serial.print(remain_volume_ary[j]);
      // }
      // Serial.println();
 
      float tank_remain = hexToIEEEFloat(remain_volume_ary);
      Serial.print("ATG tank remain is => ");
      Serial.println(tank_remain, 4);
      
      
      //  for (int j = 0; j < 8; j++) {

      //   oil_height_ary[j] = buffer[50 + j];
      //   Serial.print(oil_height_ary[j]);
      // }
      // Serial.println();
 
      float oil_height = hexToIEEEFloat(oil_height_ary);
      Serial.print("ATG tank oil height is => ");
      Serial.println(oil_height, 4);


      // for (int i = 0; i < 8; i++) {

      //   water_level_ary[i] = buffer[58 + i];
      //   Serial.print(water_level_ary[i]);
      // }
      // Serial.println();
 
      float water_height = hexToIEEEFloat(water_level_ary);
      Serial.print("ATG water height is => ");
      Serial.println(water_height, 4);



      // for (int i = 0; i < 8; i++) {

      //   temperature_ary[i] = buffer[66 + i];
      //   Serial.print(temperature_ary[i]);
      // }
      // Serial.println();
 
      float atg_temp = hexToIEEEFloat(temperature_ary);
      Serial.print("ATG Temperatur is => ");
      Serial.println(atg_temp, 2);

      // for (int i = 0; i < 8; i++) {

      //   water_volume_ary[i] = buffer[74 + i];
      //   Serial.print(water_volume_ary[i]);
      // }
      // Serial.println();
 
      float water_volume = hexToIEEEFloat(water_volume_ary);
      Serial.print("ATG water volume is => ");
      Serial.println(water_volume, 2);

    }

  }
}


void state_fun() {
  if (state_count == 1) Serial2.write(tank_1, 8);
  else if (state_count == 2) Serial2.write(tank_2, 8);
  else if (state_count == 3) Serial2.write(tank_2, 8);
  else if (state_count == 4) Serial2.write(tank_2, 8);

  if (state_count > 4) state_count = 0;
}


float hexToIEEEFloat(char* hexArray) {
  uint32_t hexValue = 0;

  // Convert each hex character to a single 32-bit integer
  for (int i = 0; i < 8; i++) {
    hexValue <<= 4;
    if (hexArray[i] >= '0' && hexArray[i] <= '9')
      hexValue |= (hexArray[i] - '0');
    else if (hexArray[i] >= 'A' && hexArray[i] <= 'F')
      hexValue |= (hexArray[i] - 'A' + 10);
  }

  // Interpret the integer as a float
  float result;
  memcpy(&result, &hexValue, sizeof(result));
  return result;
}
