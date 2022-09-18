#ifndef GRASS_H
#define GRASS_H

#include <Btn7971b.h>
#include <Encoder_2ch.h>
#include <Us_016.h>
// #include <Hc_sr04.h>
#include <Qmc5883l.h>

struct Polar_cord {
    int16_t angle;
    int16_t dist;
};

struct Rect_cord {
    int16_t x;
    int16_t y;
};

class Grass_robot {
    private:
        //
    protected:
        //
    public:
        //
        Grass_robot ();
        //
        static void begin ();
        //
        static void set_motor (const uint8_t mtr_num, const int16_t spd);
        // convert polar coordinates of direction and speed (magnitude) into speeds of 4 motors
        static void polar_ctrl (const int16_t dir, const int16_t spd, const int16_t rotation);
        // convert rectangular components of x(horizontal) and y(vertical) into speeds of 4 motors
        static void rect_ctrl (const int16_t x, const int16_t y, const int16_t rotation);
        // input the coordinates you want the robot to move to, then returns the angle and distance for robot to move to the target coordinates
        static Polar_cord uts_dist_to_polar_dist (const uint16_t right_cord, const uint16_t back_cord);
        // square root of an integer
        static uint16_t sqrt_int (const uint32_t x);

        //
        static int16_t target_spds [4];
        // objects
        static yh::rec::Btn7971b mtrs [4];
        static yh::rec::Encoder_2ch_pulse encoders [4];
        static yh::rec::Us_016 uts_right, uts_back;
        // static yh::rec::Hc_sr04_timer_int uts_sr04 [4]; // currently we use US-016
        static yh::rec::Qmc5883l compass;
};

extern Grass_robot robot;

#endif // #ifndef GRASS_H