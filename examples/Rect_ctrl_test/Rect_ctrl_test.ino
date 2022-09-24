#include <grass.h>

void setup() {
  robot.begin();
  Serial.begin(9600);
}

void loop() {
  robot.rect_ctrl(0, 90, 0);
  delay(2000);
  robot.rect_ctrl(90, 90, 0);
  delay(2000);
  robot.rect_ctrl(90, 0, 0);
  delay(2000);
  robot.rect_ctrl(90, -90, 0);
  delay(2000);
  robot.rect_ctrl(0, -90, 0);
  delay(2000);
  robot.rect_ctrl(-90, -90, 0);
  delay(2000);
  robot.rect_ctrl(-90, 0, 0);
  delay(2000);
  robot.rect_ctrl(-90, 90, 0);
  delay(2000);
}
