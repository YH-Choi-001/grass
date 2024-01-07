#include <grass.h>

int heading;
int back_dist;
int right_dist;

// 0  A->B
// 1  B->C
// 2  C->D
// 3  D->E
// 4  E->A
int phase;
int counter;

// 

int x_cords [6] = {113, 49, 82, 113, 49, 82};
int y_cords [6] = {71, 71, 95, 159, 139, 95};

const int pos_threshold = 4;

uint16_t diff (const int16_t a, const int16_t b) {
  if (a > b) {
    return a - b;
  } else {
    return b - a;
  }
}

void setup() {
  // put your setup code here, to run once:
  robot.begin();
  Serial.begin(9600);
  back_dist = robot.uts_back.read_dist_cm();
  right_dist = robot.uts_right.read_dist_cm();
  counter = 0;
  for (uint8_t i = 0; i < 5; i++) {
    if (diff(right_dist, x_cords[i]) < pos_threshold && diff(back_dist, y_cords[i]) < pos_threshold) {
      phase = i;
    }
  }
  while (robot.uts_back.read_dist_cm() > (back_dist - pos_threshold)) {} // wait for referee to put hand
  while (robot.uts_back.read_dist_cm() < (back_dist - pos_threshold)) {} // wait for referee to remove hand
  robot.compass.reset_heading();
}

void loop_c () {
  Serial.print(robot.uts_back.read_dist_cm());
  Serial.print('\t');
  Serial.print(robot.uts_right.read_dist_cm());
  Serial.print('\n');
}

void loop () {
  // put your main code here, to run repeatedly:
  heading = robot.compass.get_heading();
  back_dist = robot.uts_back.read_dist_cm();
  right_dist = robot.uts_right.read_dist_cm();
  int hori_spd = 0;
  int vert_spd = 0;
  int target_cord_idx = (phase + 1) % (sizeof(x_cords)/sizeof(x_cords[0]));
  if (right_dist - x_cords[target_cord_idx] > pos_threshold) {
    // move right
    hori_spd = 80;
  } else if (right_dist - x_cords[target_cord_idx] < -pos_threshold) {
    // move left
    hori_spd = -80;
  }
  if (back_dist - y_cords[target_cord_idx] > pos_threshold) {
    // move back
    vert_spd = -80;
  } else if (back_dist - y_cords[target_cord_idx] < -pos_threshold) {
    // move front
    vert_spd = 80;
  }
  if (heading >= 180) {
    heading -= 360;
  }
  robot.rect_ctrl(hori_spd, vert_spd, -heading * 1.8);
  if (hori_spd == 0 && vert_spd == 0) {
    delay(1500);
    counter++;
    if (counter >= 6) {
      while (1) {
        delay(100);
      }
    }
    phase++;
    if (phase >= (sizeof(x_cords)/sizeof(x_cords[0]))) {
      phase = 0;
    }
  }
}
