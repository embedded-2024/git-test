#include <stdio.h>
#include <stdint.h>

// Your hex character array
char data[8] = {'4', '6', '9', 'E', 'C', '5', '0', '7'}; 
 
// Function to convert hex chars to a float (IEEE 754)
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

void setup() {
    Serial.begin(115200);
    float ieeeFloat = hexToIEEEFloat(data);
    Serial.println(ieeeFloat); // Print the float value
}

void loop() {
    // Your code
}
