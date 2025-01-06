#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

int count = 0;
int count2 = 0;
bool check = false;
bool check2 = false;


String dd = "aungkyaw";


void Task1(void *pvParameters) {


  while (true) {

    if (check) {
      count--;
      Serial.print("core 0 => ");
      Serial.println(count);
      if (count == 5) ESP.restart();
    } 

    if (count == 0) {
      check = false;
      check2 = true;
    }

    vTaskDelay(500);
  }
}

void setup() {
  Serial.begin(115200);
  xTaskCreate(Task1, "Task1", 1024, NULL, 1, NULL);

  if (dd.equals("aungkyaw")) true;
  else Serial.println("not Equal");

  Serial.println("if is true");

}


void loop() {

  if (check2) {
    count++;
    Serial.print("core 1 => ");
    Serial.println(count);
  }


  if (count == 10) {
    check2 = false;
    check = true;
  }
  
  delay(500);
}
