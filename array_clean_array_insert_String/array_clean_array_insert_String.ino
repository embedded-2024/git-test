void setup() {
  Serial.begin(115200);


  String data = "permit";
  char mqttdatabuf[20];  // Ensure the array is large enough

  // Copy the String into the array starting from index 2
  data.toCharArray(&mqttdatabuf[2], data.length() + 1);  // +1 to include null terminator

  // Print the array content for debugging
  Serial.println(&mqttdatabuf[2]);  // Output: permit

  for (int i = 0; i < 20; i++) {
    Serial.print(mqttdatabuf[i]);
  }
  Serial.println();

  memset(mqttdatabuf, '\0', 20);

  for (int i = 0; i < 20; i++) {
    Serial.print(mqttdatabuf[i]);
    // Serial.print(".");
  }
}

void loop() {
  // Nothing to do here
}
