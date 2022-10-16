#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "nav.h"
#include "../fonts/font5x7_1.h"
#include "button.h"
#include "pio.h"
#include <stdlib.h>
#include "tinygl.h"
#include "../fonts/font5x7_1.h"

#define MESSAGE_RATE 25
#define LOOP_RATE 250
#define PACER_RATE 500

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

static int arrow_speed = 60;
static bool sabotagePowerUp = false;

static void button_task_init(void)
{
    button_init();
}


static void button_task(void) {
    button_update();

    if (button_push_event_p(BUTTON1) && sabotagePowerUp) {
        arrow_speed = arrow_speed - (arrow_speed / 20);
        sabotagePowerUp = false;
        led_set(LED1, 0);
    }
}


int main(void)
{
    system_init();

    /* TODO: Initialise the pins of the LED matrix.  */
    ledmat_init();
    button_task_init();

    /* Initialize  */
    pacer_init(LOOP_RATE);

    int delay = 1000;
    int current_tick = 0;

    int arrow_x = 0;
    int arrow_y = 0;

    int correctCount = 0;
    int wrongMove = 0;
    bool lost = false;

    while (1)
    {

        if (wrongMove == 3) {
            lost = true;
        }

        if (!lost) {
            /* Pace the loop.  */
            pacer_wait();
            clear_screen();
            navswitch_update();
            button_task();

            tinygl_init(PACER_RATE);
            tinygl_font_set(&font5x7_1);
            tinygl_text_speed_set(MESSAGE_RATE);
            tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);

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
            // nav.h
            move(arrow_x, arrow_y, &correctCount, &wrongMove);

            if (correctCount == 2) {
                led_set(LED1, 1);
                sabotagePowerUp = true;
                correctCount = 0;
            }

            current_tick++;

        } else if (lost) {
            // ADD LOSE SCREEN
            tinygl_text("YOU LOSE!");
            tinygl_update();
        }
    }
}
