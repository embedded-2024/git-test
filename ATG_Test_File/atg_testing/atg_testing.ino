
unsigned char tank_1[8] = { 0x01, 0x69, 0x32, 0x30, 0x31, 0x30, 0x31, 0x03 };
unsigned char tank_2[8] = { 0x01, 0x69, 0x32, 0x30, 0x31, 0x30, 0x32, 0x03 };
unsigned char tank_3[8] = { 0x01, 0x69, 0x32, 0x30, 0x31, 0x30, 0x33, 0x03 };
unsigned char tank_4[8] = { 0x01, 0x69, 0x32, 0x30, 0x31, 0x30, 0x34, 0x03 };

char buffer[100];

char tank_product_ary[8];
char tank_volume_ary[8];
char temperature_ary[8];
char water_level_ary[8];

int state_count;
int count;

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  Serial2.write(tank_2, 8);
  // Serial2.write(tank_4, 8);
  // Serial.println("Done");
}

void loop() {

  if (Serial2.available()) {

    buffer[count] = Serial2.read();
    delay(5);

    Serial.print(count);
    Serial.print("/");
    Serial.print(buffer[count], HEX);
    Serial.print(" ");
    count++;

    if (buffer[count - 7] == 0x26 && buffer[count - 6] == 0x26 && buffer[count - 1] == 0x03) {

      Serial.println();

      for (int i = 0; i < 8; i++) {

        tank_volume_ary[i] = buffer[26 + i];
        Serial.print(tank_volume_ary[i]);
        // water_level[i] = buffer[58 + i];
        // temperature[i] = buffer[66 + i];
      }

      Serial.println();

      

      // uint32_t tank_volume_data = IEEE_converter(tank_volume_ary);
      float tank_volume = hexToIEEEFloat(tank_volume_ary);

      Serial.print("ATG tank volume is => ");
      Serial.println(tank_volume, 4);


      for (int j = 0; j < 8; j++) {

        tank_product_ary[j] = buffer[34 + j];

        Serial.print(tank_product_ary[j]);
      }

      // uint32_t tank_product_data = IEEE_converter(tank_product_ary);
      float tank_product = hexToIEEEFloat(tank_product_ary);
      Serial.print("ATG tank product is => ");
      Serial.println(tank_product, 4);

      for (int i = 0; i < 8; i++) {

        water_level_ary[i] = buffer[58 + i];
        Serial.print(water_level_ary[i]);
      }

      // uint32_t water_height_data = IEEE_converter(water_level_ary);
      float water_height = hexToIEEEFloat(water_level_ary);

      Serial.print("ATG water height is => ");
      Serial.println(water_height, 4);

      for (int i = 0; i < 8; i++) {

        temperature_ary[i] = buffer[66 + i];
        Serial.print(temperature_ary[i]);
      }


      // uint32_t atg_temp_data = IEEE_converter(temperature_ary);
      float atg_temp = hexToIEEEFloat(temperature_ary);
      
      Serial.print("ATG Temperatur is => ");
      Serial.println(atg_temp,2);


      // if (buffer[17] == 0x30 && buffer[18] == 0x34) {
      //   Serial.println("this is tank 4");
      //   count = 0;
      //   // state_count++;
      //   // state_fun();
      // }

    }

    // if (buffer[17] == 0x30 && buffer[18] == 0x34) {
    //   Serial.println("this is tank 4");
    // }

    // for (int i ; i < 8 ; i++{
    //   temperature[i] =
    // });
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

// float IEEE_converter(char *data_arr) {

//   uint32_t hexData_ascii = (uint32_t)strtol(data_arr, NULL, 16);

//   return hexData_ascii;
// }


// float f_function(uint32_t hexData) {

//   float *floatPtr = (float *)&hexData;
//   float floatValue = *floatPtr;

//   // Serial.print("Converted float value: ");
//   // Serial.println(floatValue, 4);
//   return floatValue;
// }


/* 
// for(int i = 0 ; i < 50 ; i++) {
  //   buffer[i] = Serial2.read();
  //   delay(5);
  //   Serial.print(i);
  //   Serial.print("/");
  //   Serial.print(buffer[i], HEX);
  //   Serial.print(" ");
  // }
  // Serial.println();


/////////////////

  // uint32_t hexData = 0x42B7B333;   // temp data
  // uint32_t hexData = (uint32_t)strtol(dd, NULL, 16);
  // float dd = f_function(hexData);
  // Serial.print("Converted float dd value: ");
  // Serial.println(dd, 6);

  // delay(5);
*/