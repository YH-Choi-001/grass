#include <Hc_sr04.h>
#include <Wire.h>

yh::rec::Hc_sr04 uts [4] {
    yh::rec::Hc_sr04(1, 5), // R
    yh::rec::Hc_sr04(3, 7), // L
    yh::rec::Hc_sr04(2, 6), // B
    yh::rec::Hc_sr04(0, 4)  // F
};

volatile uint8_t dists [4];
volatile uint8_t idx;
volatile uint8_t hori_delta; // distance to center line

volatile uint8_t mode;

volatile bool inc_trend;

void rx (int len) {
    while (Wire.available()) {
        const uint8_t rx_byte = Wire.read();
        if (rx_byte >= 'w' && rx_byte <= 'z') {
            idx = rx_byte - 'w';
        } else {
            mode = rx_byte;
        }
    }
}

// For mode 0, the chip outputs Right, Left, Back, Front distance (in cm) one by one.
// For mode 1, the chip outputs distance to center line, Back, Front distance (in cm) one by one. (for defense-use or atan-goaling-use)
void tx () {
    if (mode & (1 << 0)) {
        Wire.write(hori_delta);
        Wire.write(dists[2]); // B
        Wire.write(dists[3]); // F
    } else {
        for (uint8_t i = idx; i < 4; i++) {
            Wire.write(dists[i]);
        }
        idx = 0;
    }
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
    dists[0] = dists[1] = dists[2] = dists[3] = 0;
    idx = 0;
    hori_delta = 0;
    mode = 0;
    Wire.begin(0x77);
    Wire.onReceive(rx);
    Wire.onRequest(tx);
    prev_time = millis();
}

void loop () {
    { // R
        const uint16_t temp = uts[0].read_dist_cm();
        if (temp > 255) {
            dists[0] = 0;
        } else {
            dists[0] = temp;
        }
    }
    { // L
        const uint16_t temp = uts[1].read_dist_cm();
        if (temp > 255) {
            dists[1] = 0;
        } else {
            dists[1] = temp;
        }
    }
    { // B
        const uint16_t temp = uts[2].read_dist_cm();
        if (temp > 255) {
            dists[2] = 0;
        } else {
            dists[2] = temp;
        }
    }
    { // F
        const uint16_t temp = uts[3].read_dist_cm();
        if (temp > 255) {
            dists[3] = 0;
        } else {
            dists[3] = temp;
        }
    }
    {
        uint8_t l = dists[1], r = dists[0];
        if (l == 0) { l = 255; }
        if (r == 0) { r = 255; }
        hori_delta = (l >> 1) - (r >> 1); // distance to center line
    }
    const unsigned long curr_time = millis();
    if ((curr_time - prev_time) > 400) {
        PINB |= (1 << 0);
        prev_time = curr_time;
    }
}
