#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"
#include "system.h"
#include "pio.h"
#include <stdlib.h>

#define LOOP_RATE 250

static pio_t ledmat_rows[] =
{
    LEDMAT_ROW1_PIO, LEDMAT_ROW2_PIO, LEDMAT_ROW3_PIO, LEDMAT_ROW4_PIO,
    LEDMAT_ROW5_PIO, LEDMAT_ROW6_PIO, LEDMAT_ROW7_PIO
};
static pio_t ledmat_cols[] =
{
    LEDMAT_COL1_PIO, LEDMAT_COL2_PIO, LEDMAT_COL3_PIO,
    LEDMAT_COL4_PIO, LEDMAT_COL5_PIO
};

static void ledmat_pixel_set(int col, int row, bool state)
{
    if (state)
    {
        pio_output_low(ledmat_rows[row]);
        pio_output_low(ledmat_cols[col]);
    }
    else
    {
        pio_output_high(ledmat_rows[row]);
        pio_output_high(ledmat_cols[col]);
    }
}

/** Initialise LED matrix PIO pins.  */
static void ledmat_init(void)
{
    uint8_t row;
    uint8_t col;

    for (row = 0; row < 7; row++)
    {
        pio_config_set(ledmat_rows[row], PIO_OUTPUT_HIGH);
        pio_output_high(ledmat_rows[row]);
    }

    for (col = 0; col < 5; col++)
    {
        pio_config_set(ledmat_cols[col], PIO_OUTPUT_HIGH);
        pio_output_high(ledmat_cols[col]);
    }
}

// CLEAR SCREEN
static void clear_screen(void) {
    for (int i = 0; i < 7; i++) {
        for (int x = 0; x < 5; x++) {
            ledmat_pixel_set(x, i, 0);
        }
    }
}

// ARROWS
// static void drawDownArrow(int draw, int yPos1, int yPos2) {
//     if (draw == 0) {
//         ledmat_pixel_set(2, yPos2, 0);
//         ledmat_pixel_set(1, yPos1, 1);
//         ledmat_pixel_set(3, yPos1, 1);
//     } else if (draw == 1) {
//         ledmat_pixel_set(2, yPos2, 1);
//         ledmat_pixel_set(1, yPos1, 0);
//         ledmat_pixel_set(3, yPos1, 0);
//     }
// }

// static void drawUpArrow(int draw, int yPos1, int yPos2) {
//     if (draw == 0) {
//         ledmat_pixel_set(1, yPos2, 1);
//         ledmat_pixel_set(3, yPos2, 1);
//         ledmat_pixel_set(2, yPos1, 0);
//     }
//     else if (draw == 1) {
//         ledmat_pixel_set(2, yPos1, 1);
//         ledmat_pixel_set(1, yPos2, 0);
//         ledmat_pixel_set(3, yPos2, 0);
//     }
// }

// // NOT DONE
// static void drawLeftArrow(int draw, int yPos1, int yPos2) {
//     if (draw == 0) {
//         ledmat_pixel_set(1, yPos2, 1);
//         ledmat_pixel_set(3, yPos2, 1);
//         ledmat_pixel_set(2, yPos1, 0);
//     }
//     else if (draw == 1) {
//         ledmat_pixel_set(2, yPos1, 1);
//         ledmat_pixel_set(1, yPos2, 0);
//         ledmat_pixel_set(3, yPos2, 0);
//     }
// }

// static void drawRightArrow(int draw, int yPos1, int yPos2) {
//     if (draw == 0) {
//         ledmat_pixel_set(1, yPos2, 1);
//         ledmat_pixel_set(3, yPos2, 1);
//         ledmat_pixel_set(2, yPos1, 0);
//     }
//     else if (draw == 1) {
//         ledmat_pixel_set(2, yPos1, 1);
//         ledmat_pixel_set(1, yPos2, 0);
//         ledmat_pixel_set(3, yPos2, 0);
//     }
// }

// void newArrow(void) {
//     int x = rand() % 5
// }


// DRAW LINE
// static void drawLine(void) {
    
// }

int main(void)
{
    system_init();

    /* TODO: Initialise the pins of the LED matrix.  */
    ledmat_init();

    /* Initialize  */
    pacer_init(LOOP_RATE);

    // int draw = 0;
    // int yPos1 = 0, yPos2 = 1;
    // bool arrowOnScreen = true;

    int delay = 1000;
    int current_tick = 0;
    int arrow_speed = 30;
    // int speed = 100;

    // int waitRand = 0;
    // int randTime = 800;

    int arrow_x = 0;
    int arrow_y = 0;

    while (1)
    {
        /* Pace the loop.  */
        pacer_wait();
        clear_screen();
        current_tick++;

        if (current_tick % arrow_speed == 0) {
            arrow_y++;
        }
        if (arrow_y >= 7) {
            arrow_y = 0;
            arrow_x = rand() % 5;
        }

        if (current_tick >= 10000) {
            current_tick = 0;
        }

        ledmat_pixel_set(arrow_x, arrow_y, 1);


        // // DRAW LINE
        // // drawLine();

        // // DRAW ARROW

        // if (waitRand != randTime) {
        //     waitRand++;
        // } else {
        //     r = rand() % 3;
        //     waitRand = 0;
        //     arrowOnScreen = true;
        // }

        // if (arrowOnScreen) {
        //     if (r == 0) 
        //         drawDownArrow(draw, yPos1, yPos2);
        //     else
        //         drawUpArrow(draw, yPos1, yPos2);

        //     if (draw == 2) {
        //         draw = 0;
        //     }
        //     else {
        //         draw++;
        //     }


        //     if (wait != speed) {
        //         wait++;
        //     }
        //     else {
        //         wait = 0;
        //         if (yPos1 != 5) {
        //             yPos1++;
        //             yPos2++;
        //         } else {
        //             yPos1 = 0, yPos2 = 1;
        //             arrowOnScreen = false;
        //         }
        //     }
        // }

        //tinygl_update();
    }
}
