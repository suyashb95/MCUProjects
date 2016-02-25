#include <stdint.h>

struct {
  int8_t x;
  int8_t y;
  int8_t z;
  uint8_t buttons;
} joyReport;

uint8_t *joyReportPtr = (uint8_t*)(&joyReport);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(200);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  joyReport.buttons = 0;
  joyReport.x = 0;
  joyReport.y = 0;
  joyReport.z = 0;

  joyReport.x = 100;
  Serial.write(joyReportPtr, 4);
  delay(1000);
  
  joyReport.x = 0;
  joyReport.y = 100;
  Serial.write(joyReportPtr, 4);
  delay(1000);

  joyReport.y = 0;
  joyReport.z = 100;
  Serial.write(joyReportPtr, 4);
  delay(1000);
  
  joyReport.x = -100;
  joyReport.z = 0;
  Serial.write(joyReportPtr, 4);
  delay(1000);

  joyReport.x = 0;
  joyReport.y = -100;
  Serial.write(joyReportPtr, 4);
  delay(1000);

  joyReport.y = 0;
  joyReport.z = -100;
  Serial.write(joyReportPtr, 4);
  delay(1000);

  joyReport.buttons = 1;
  Serial.write(joyReportPtr, 4);
  delay(1000);

  joyReport.buttons = 0;
  Serial.write(joyReportPtr, 4);
}
