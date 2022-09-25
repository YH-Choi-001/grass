#include <Hc_sr04.h>
#include <Wire.h>

yh::rec::Hc_sr04 uts [4] {
    yh::rec::Hc_sr04(0, 4),
    yh::rec::Hc_sr04(1, 5),
    yh::rec::Hc_sr04(2, 6),
    yh::rec::Hc_sr04(3, 7)
};

volatile uint8_t dists [4];
volatile uint8_t idx;

volatile bool inc_trend;

void rx (int len) {
    const uint8_t rx_byte = Wire.read();
    if (rx_byte >= 'w' && rx_byte <= 'z') {
        idx = rx_byte - 'w';
    } else if (rx_byte) {
        OCR1A = rx_byte;
    }
}

void tx () {
    for (uint8_t i = idx; i < 4; i++) {
        Wire.write(dists[i]);
    }
    idx = 0;
    uint8_t temp = OCR1A;
    if (inc_trend) {
        temp += 15;
        if (temp == 255) {
            inc_trend = false;
        }
    } else {
        temp -= 15;
        if (temp == 0) {
            inc_trend = true;
        }
    }
    OCR1A = temp;
}

unsigned long prev_time;

void setup () {
    DDRB |= (1 << 0);
    DDRB |= (1 << 1);
    OCR1A = 0;
    TCCR1A |= (1 << COM1A1);
    inc_trend = true;
    uts[0].begin();
    uts[1].begin();
    uts[2].begin();
    uts[3].begin();
    uts[0].set_max_range_in_cm(255);
    uts[1].set_max_range_in_cm(255);
    uts[2].set_max_range_in_cm(255);
    uts[3].set_max_range_in_cm(255);
    Wire.begin(0x77);
    Wire.onReceive(rx);
    Wire.onRequest(tx);
    prev_time = millis();
}

void loop () {
    {
        const uint16_t temp = uts[0].read_dist_cm();
        if (temp > 255) {
            dists[0] = 0;
        } else {
            dists[0] = temp;
        }
    }
    {
        const uint16_t temp = uts[1].read_dist_cm();
        if (temp > 255) {
            dists[1] = 0;
        } else {
            dists[1] = temp;
        }
    }
    {
        const uint16_t temp = uts[2].read_dist_cm();
        if (temp > 255) {
            dists[2] = 0;
        } else {
            dists[2] = temp;
        }
    }
    {
        const uint16_t temp = uts[3].read_dist_cm();
        if (temp > 255) {
            dists[3] = 0;
        } else {
            dists[3] = temp;
        }
    }
    const unsigned long curr_time = millis();
    if ((curr_time - prev_time) > 400) {
        PINB |= (1 << 0);
        prev_time = curr_time;
    }
}
