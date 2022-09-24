#include <grass.h>

void setup () {
  robot.begin();
  Serial.begin(9600);
}

void loop () {
  Serial.print("Back: ");
  Serial.print(robot.uts_back.read_dist_cm());
  Serial.print("\tRight: ");
  Serial.print(robot.uts_right.read_dist_cm());
}