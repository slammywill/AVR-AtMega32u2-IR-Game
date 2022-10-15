#include "system.h"
#include "led.h"

// Defining loop rate in Hz.
#define LOOP_RATE 100

int main (void)
{
    system_init ();
    led_init();


    while (1)
    {
        led_set(1, 1);


    }
}
