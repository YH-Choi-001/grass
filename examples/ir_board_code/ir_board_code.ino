#include <Mcp_3008.h>
// #include <Wire.h>
#include <Twi_custom.h>

yh::rec::Mcp_3008_default_SPI mcps [3] {
    yh::rec::Mcp_3008_default_SPI(2),
    yh::rec::Mcp_3008_default_SPI(4),
    yh::rec::Mcp_3008_default_SPI(7)
};

/*
uint16_t max [24];
uint16_t min [24];
volatile uint16_t delta [24];

void tx () {
    Wire.write(delta[0] >> 8);
    Wire.write(delta[0] & 0xff);
    Wire.write(delta[1] >> 8);
    Wire.write(delta[1] & 0xff);
    Wire.write(delta[2] >> 8);
    Wire.write(delta[2] & 0xff);
    Wire.write(delta[3] >> 8);
    Wire.write(delta[3] & 0xff);
}

void setup () {
    mcps[0].begin();
    mcps[1].begin();
    mcps[2].begin();
    Wire.begin(0x41);
    Wire.onRequest(tx);
}

void loop () {
    const unsigned long prev = micros();
    max[0] = min[0] = mcps[0].analog_read(0);
    max[1] = min[1] = mcps[0].analog_read(1);
    max[2] = min[2] = mcps[0].analog_read(2);
    max[3] = min[3] = mcps[0].analog_read(3);
    do {
        {
            const uint16_t raw = mcps[0].analog_read(0);
            if (raw < min[0]) {
                min[0] = raw;
            }
            if (raw > max[0]) {
                max[0] = raw;
            }
        }
        {
            const uint16_t raw = mcps[0].analog_read(1);
            if (raw < min[1]) {
                min[1] = raw;
            }
            if (raw > max[1]) {
                max[1] = raw;
            }
        }
        {
            const uint16_t raw = mcps[0].analog_read(2);
            if (raw < min[2]) {
                min[2] = raw;
            }
            if (raw > max[2]) {
                max[2] = raw;
            }
        }
        {
            const uint16_t raw = mcps[0].analog_read(3);
            if (raw < min[3]) {
                min[3] = raw;
            }
            if (raw > max[3]) {
                max[3] = raw;
            }
        }
    } while ((micros() - prev) < 833);
    for (uint8_t i = 0; i < (sizeof(delta) / sizeof(delta[0])); i++) {
        delta[i] = max[i] - min[i];
    }
}

*/

uint16_t data[24];
volatile uint8_t counter;

uint16_t tx () {
    uint8_t temp = 0;
    if (counter & 0x01) {
        // odd
        temp = (data[counter >> 1] & 0xff);
    } else {
        // even
        temp = (data[counter >> 1] >> 8);
    }
    counter++;
    if (counter >= 48) {
        counter = 0;
    }
    return (1 << 8) | temp;
}

void setup () {
    mcps[0].begin();
    mcps[1].begin();
    mcps[2].begin();
    counter = 0;
    twi.begin(0x41);
    //
    twi.slave_transmitting_byte = tx;
}

void loop () {
    for (uint8_t i = 0; i < 24; i++) {
        data[i] = mcps[i >> 3].analog_read(i & 0x07);
    }
}
