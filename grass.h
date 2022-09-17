#ifndef GRASS_H
#define GRASS_H

#include <Btn7971b.h>
#include <Encoder_2ch.h>
#include <Us_016.h>
// #include <Hc_sr04.h>
#include <Qmc5883l.h>


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
        static void polar_control (const int16_t dir, const int16_t spd, const int16_t rotation);
        // convert rectangular components of x(horizontal) and y(vertical) into speeds of 4 motors
        static void rect_control (const int16_t x, const int16_t y, const int16_t rotation);

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