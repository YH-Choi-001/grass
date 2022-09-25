// This source code failed to return non-zero values to master I2C chip
// Do not use

#include <Hc_sr04.h>
#include <Wire.h>

yh::rec::Hc_sr04_ext_int uts [4] {
    yh::rec::Hc_sr04_ext_int(0, 4),
    yh::rec::Hc_sr04_ext_int(1, 5),
    yh::rec::Hc_sr04_ext_int(2, 6),
    yh::rec::Hc_sr04_ext_int(3, 7)
};

volatile uint8_t prev_pind;

ISR(PCINT2_vect) {
    const uint8_t curr_pind = PIND;
    const uint8_t delta = curr_pind ^ prev_pind;
    if (delta & (1 << 4)) {
        uts[0].isr_individual_sensor_routine();
    }
    if (delta & (1 << 5)) {
        uts[1].isr_individual_sensor_routine();
    }
    if (delta & (1 << 6)) {
        uts[2].isr_individual_sensor_routine();
    }
    if (delta & (1 << 7)) {
        uts[3].isr_individual_sensor_routine();
    }
    prev_pind = curr_pind;
}

volatile uint8_t dists [4];
volatile uint8_t idx;

unsigned long prev_time;

void rx (int len) {
    const uint8_t rx_byte = Wire.read();
    if (rx_byte >= 'w' && rx_byte <= 'z') {
        idx = rx_byte - 'w';
    } else if (rx_byte) {
        PORTB |= (1 << 1);
    } else {
        PORTB &= ~(1 << 1);
    }
}

void tx () {
    for (uint8_t i = idx; i < 4; i++) {
        Wire.write(dists[i]);
    }
    idx = 0;
}

// void setup_pcint (const uint8_t pin) {
//     *digitalPinToPCMSK(pin) |= (1 << digitalPinToPCMSKbit(pin));  // enable pin
//     // the respective bit of PCIFR register is set to 1 when a pin in the port changes
//     PCICR |= (1 << digitalPinToPCICRbit(pin)); // enable interrupt for the group
// }

bool upward_trend;

void setup () {
    DDRB |= (1 << 0);
    DDRB |= (1 << 1);
    TCCR1A |= (1 << COM1A1);
    upward_trend = true;
    uts[0].begin();
    uts[1].begin();
    uts[2].begin();
    uts[3].begin();
    // setup_pcint(4);
    // setup_pcint(5);
    // setup_pcint(6);
    // setup_pcint(7);
    PCMSK2 = (1 << 7) | (1 << 6) | (1 << 5) | (1 << 4);
    PCICR |= (1 << 2);
    prev_pind = PIND;
    prev_time = millis();
    idx = 0;
    Wire.begin(0x77);
    Wire.onReceive(rx);
    Wire.onRequest(tx);
    // Serial.begin(9600);
}

void loop () {
    //*
    {
        const uint16_t temp = uts[0].polling_update();
        if (temp > 255) {
            dists[0] = 0;
        } else {
            dists[0] = temp;
        }
    }
    {
        const uint16_t temp = uts[1].polling_update();
        if (temp > 255) {
            dists[1] = 0;
        } else {
            dists[1] = temp;
        }
    }
    //*/
    {
        const uint16_t temp = uts[2].polling_update();
        if (temp > 255) {
            dists[2] = 0;
        } else {
            dists[2] = temp;
        }
    }
    {
        const uint16_t temp = uts[3].polling_update();
        if (temp > 255) {
            dists[3] = 0;
        } else {
            dists[3] = temp;
        }
    }
    const unsigned long curr_time = millis();
    if (curr_time - prev_time > 350) {
        PINB |= (1 << 0);
        prev_time = curr_time;
    }
    if (upward_trend) {
        OCR1A++;
        if (OCR1A == 255) {
            upward_trend = false;
        }
    } else {
        OCR1A--;
        if (OCR1A == 0) {
            upward_trend = true;
        }
    }
    // Serial.print(dists[2]);
    // Serial.print('\t');
    // Serial.print(dists[3]);
    // Serial.print('\n');
}
