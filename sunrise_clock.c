#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000L // CPU at 16 MHz

#include "time_keeper.c"

int main(void) {
    setup_datetime_default();

    while(1) {
        // FIXME - stick some debugging code here to blink an LED every second
    };

    return 0;
}
