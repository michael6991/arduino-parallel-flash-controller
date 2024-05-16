#include <cstdint>

// Control pinout
#define WE 2
#define OE 3
// CE can be defined or connected directly to gnd
// if you choose to control CE, then you'll have to dive in the code
// and add some tweaks here and there.

// Address pinout, can be extended or shortend if needed
// just don't forget to update ADDR_MIN/MAX accordingly
#define A0  22
#define A1  23
#define A2  24
#define A3  25
#define A4  26
#define A5  27
#define A6  28
#define A7  29
#define A8  30
#define A9  31
#define A10 32
#define A11 33
#define A12 34
#define A13 35
#define A14 36
#define A15 37
#define A16 38
#define A17 39
#define A18 40
#define ADDR_MIN (A0)
#define ADDR_MAX (A18)

// Data pinout, can be extended or shortend if needed
// just don't forget to update DATA_MIN/MAX accordingly
#define D0  44
#define D1  45
#define D2  46
#define D3  47
#define D4  48
#define D5  49
#define D6  50
#define D7  51
#define DATA_MIN (D0)
#define DATA_MAX (D7)


void set_addr_pins_out() {
    for (uint8_t i = ADDR_MIN; i <= ADDR_MAX; i++) {
        pinMode(i, OUTPUT);
        digitalWrite(i, LOW);
    }
}

void set_data_pins_out() {
    for (uint8_t i = DATA_MIN; i <= DATA_MAX; i++) {
        pinMode(i, OUTPUT);
    }
}

void set_data_pins_in() {
    for (uint8_t i = DATA_MIN; i <= DATA_MAX; i++) {
        pinMode(i, INPUT);
    }
}

void setup_ctrl_pins() {
    pinMode(WE, OUTPUT);
    pinMode(OE, OUTPUT);

    digitalWrite(WE, HIGH);
    digitalWrite(OE, HIGH);
}

void setup() {
    set_data_pins_in();
    setup_ctrl_pins();
    set_addr_pins_out();

    Serial.begin(115200);
    while(!Serial.available());
}

void set_byte(uint8_t out) {
    for (uint8_t i = 0; i < 8; i++) {
        digitalWrite(D0 + i, bitRead(out, i));
    }
}

void set_address(uint32_t addr) {
    for (uint8_t i = 0; i < (ADDR_MAX - ADDR_MIN + 1); i++) {
        digitalWrite(A0 + i, bitRead(addr, i));
    }
}

uint8_t read_byte() {
    uint8_t temp_in = 0;
    for (uint8_t i = 0; i < 8; i++) {
        if (digitalRead(D0 + i)) {
            bitSet(temp_in, i);
        }
    }
    return temp_in;
}

// Reads data from flash memory in a given address
uint8_t read_data(uint32_t addr) {
    uint8_t temp_read = 0;

    set_data_pins_in();
    digitalWrite(WE, HIGH);
    digitalWrite(OE, HIGH);

    set_address(addr);

    digitalWrite(OE, LOW);
    delayMicroseconds(1);

    temp_read = read_byte();
    digitalWrite(OE, HIGH);

    return temp_read;
}

// Writes data that you will want to program into volatile registers of the flash device.
// This function is encapsulated within the program_data function, see below.
void write_byte(uint8_t data, uint32_t addr) {
    digitalWrite(OE, HIGH);
    digitalWrite(WE, HIGH);

    set_address(addr);
    set_byte(data);

    digitalWrite(WE, LOW);
    delayMicroseconds(1);
    digitalWrite(WE, HIGH);
}

void program_data(uint8_t data, uint32_t addr) {
    set_data_pins_out();

    write_byte(0xAA, 0x5555);
    write_byte(0x55, 0x2AAA);
    write_byte(0xA0, 0x5555);
    write_byte(data, addr);

    delayMicroseconds(30);
}

void erase_chip() {
    set_data_pins_out();

    write_byte(0xAA, 0X5555);
    write_byte(0x55, 0x2AAA);
    write_byte(0x80, 0x5555);
    write_byte(0xAA, 0x5555);
    write_byte(0x55, 0x2AAA);
    write_byte(0x10, 0x5555);

    delay(100);
    set_data_pins_in();
}


void loop() {

    // insert your operations here

    while(true);
}