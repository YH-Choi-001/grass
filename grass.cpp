#ifndef GRASS_CPP
#define GRASS_CPP

#include "grass.h"

Grass_robot robot;

Grass_robot::Grass_robot ()
{
    //
}

yh::rec::Btn7971b Grass_robot::mtrs [4] {
    yh::rec::Btn7971b(5, 13),
    yh::rec::Btn7971b(11, 10),
    yh::rec::Btn7971b(6, 12),
    yh::rec::Btn7971b(3, 2)
};

yh::rec::Encoder_2ch_pulse Grass_robot::encoders [4] {
    yh::rec::Encoder_2ch_pulse(A0, A1),
    yh::rec::Encoder_2ch_pulse(A2, A3),
    yh::rec::Encoder_2ch_pulse(4, 7),
    yh::rec::Encoder_2ch_pulse(8, 9)
};

yh::rec::Us_016 Grass_robot::uts_right (yh::rec::Us_016(NC_PINNO, A11));
yh::rec::Us_016 Grass_robot::uts_back (yh::rec::Us_016(NC_PINNO, A10));

yh::rec::Qmc5883l Grass_robot::compass;

void Grass_robot::begin () {
    mtrs[0].begin();
    mtrs[1].begin();
    mtrs[2].begin();
    mtrs[3].begin();

    encoders[0].begin();
    encoders[1].begin();
    encoders[2].begin();
    encoders[3].begin();

    uts_right.begin();
    uts_back.begin();

    compass.begin();
}

void Grass_robot::set_motor (const uint8_t motor_num, const int16_t spd) {
    mtrs[motor_num].set_spd(target_spds[motor_num] = spd);
}

void Grass_robot::polar_ctrl (const int16_t dir, const int16_t spd, const int16_t rotation) {
    int16_t temp_spds [4];
    // resolve vectors
    temp_spds[0] = spd * sin((dir + 45) * DEG_TO_RAD);
    temp_spds[1] = spd * sin((dir - 45) * DEG_TO_RAD);
    temp_spds[2] = -temp_spds[0];
    temp_spds[3] = -temp_spds[1];
    // add rotation
    temp_spds[0] += rotation;
    temp_spds[1] += rotation;
    temp_spds[2] += rotation;
    temp_spds[3] += rotation;
    const uint8_t oldSREG = SREG;
    noInterrupts();
    // set speeds
    target_spds[0] = temp_spds[0];
    target_spds[1] = temp_spds[1];
    target_spds[2] = temp_spds[2];
    target_spds[3] = temp_spds[3];
    SREG = oldSREG;
    mtrs[0].set_spd(temp_spds[0]);
    mtrs[1].set_spd(temp_spds[1]);
    mtrs[2].set_spd(temp_spds[2]);
    mtrs[3].set_spd(temp_spds[3]);
}

void Grass_robot::rect_ctrl (const int16_t x, const int16_t y, const int16_t rotation) {
    int16_t temp_spds [4];
    // resolve vectors
    // temp_spds[0] = x * sin(45) + y * sin(45);
    // temp_spds[1] = x * sin(45) - y * sin(45);
    // sin(45) == cos(45) == sqrt(2) / 2 == 1 / sqrt(2)
    temp_spds[0] = x * 0.707106781186547 + y * 0.707106781186547;
    temp_spds[1] = x * 0.707106781186547 - y * 0.707106781186547;
    temp_spds[2] = -temp_spds[0];
    temp_spds[3] = -temp_spds[1];
    // add rotation
    temp_spds[0] += rotation;
    temp_spds[1] += rotation;
    temp_spds[2] += rotation;
    temp_spds[3] += rotation;
    const uint8_t oldSREG = SREG;
    noInterrupts();
    // set speeds
    target_spds[0] = temp_spds[0];
    target_spds[1] = temp_spds[1];
    target_spds[2] = temp_spds[2];
    target_spds[3] = temp_spds[3];
    SREG = oldSREG;
    mtrs[0].set_spd(temp_spds[0]);
    mtrs[1].set_spd(temp_spds[1]);
    mtrs[2].set_spd(temp_spds[2]);
    mtrs[3].set_spd(temp_spds[3]);
}

Polar_cord Grass_robot::uts_dist_to_polar_dist (const uint16_t right_cord, const uint16_t back_cord) {
    int delta_x = uts_right.read_dist_cm() - right_cord;
    int delta_y = back_cord - uts_back.read_dist_cm();
    int16_t angle = static_cast<int16_t>(atan2(delta_x, delta_y) * RAD_TO_DEG);
    if (angle < 0) {
        angle += 360;
    }
    return Polar_cord {angle, static_cast<int16_t>(sqrt_int(delta_x * delta_x + delta_y * delta_y))};
}

uint16_t Grass_robot::sqrt_int (const uint32_t x) {
    for (uint16_t i = 0; ((i + 1) != 0); i++) {
        const uint32_t sqed_val = i * i;
        if (x == sqed_val) {
            return i;
        } else if (x < sqed_val) {
            return i - 1;
        }
    }
}

#endif // #ifndef GRASS_CPP