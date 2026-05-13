#include <Arduino_FreeRTOS.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  xTaskCreate(One, "one", 128, NULL, 1, NULL);
  xTaskCreate(Two, "two", 128, NULL, 1, NULL);
}

void loop() {
  // put your main code here, to run repeatedly:
}

void One(void *pvParameters) {
  while(1) {
    Serial.println("One");
    delay(2000);
  }
}

void Two(void *pvParameters) {
  while(1) {
    Serial.println("Two");
    delay(1000);
  }
}