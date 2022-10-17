/*
* MAIN GAME FILE
* AUTHORS: Zac Avis, Sam Willems
*/
#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "nav.h"
#include "../fonts/font5x7_1.h"
#include "button.h"
#include "pio.h"

#define LOOP_RATE 500
#define PACER_RATE 500

// PUTS THE ROWS OF LEDS INTO A LIST
static pio_t ledmat_rows[] =
{
    LEDMAT_ROW1_PIO, LEDMAT_ROW2_PIO, LEDMAT_ROW3_PIO, LEDMAT_ROW4_PIO,
    LEDMAT_ROW5_PIO, LEDMAT_ROW6_PIO, LEDMAT_ROW7_PIO
};
// PUTS THE COLUMNS OF LEDS INTO A LIST
static pio_t ledmat_cols[] =
{
    LEDMAT_COL1_PIO, LEDMAT_COL2_PIO, LEDMAT_COL3_PIO,
    LEDMAT_COL4_PIO, LEDMAT_COL5_PIO
};

// SET A PIXEL AT SPECIFIED COLUMN & ROW TO BE ON OR OFF
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

// INITIALISES LED MATRIX PIO PINS
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

// VARIABLES
static int arrow_speed = 60;
static bool sabotagePowerUp = false;

static void button_task_init(void)
{
    button_init();
}

// IF PLAYER HAS A POWER UP & PUSHES BUTTON THEN IT'LL SPEED OPPONENTS GAME UP
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

    // INITIALISE
    // ir_uart_init();
    ledmat_init();
    button_task_init();
    pacer_init(LOOP_RATE);
    tinygl_init(PACER_RATE);
    tinygl_font_set(&font5x7_1);

    // VARIABLES
    int current_tick = 0;
    int arrow_x = 0;
    int arrow_y = 0;

    int correctCount = 0;
    int score = 0;
    int wrongMove = 0;
    bool gameOver = false;
    bool lost = false;
    bool started = true;

    while (1)
    {
        // if (!started) {
        //     ir_uart_putc('S');
        //     char start = ir_uart_getc();
        //     if (start == 'S') {
        //         started = true;
        //     }
        // }
        if (started) {
            if (wrongMove == 3) {
                gameOver = true;
                lost = true;
            }

            // RANDOMLY CHOOSES POSITION WHERE A DOT WILL FALL,
            if (!gameOver) {
                // PACE THE LOOP & CLEARS THE LOOP
                clear_screen();
                navswitch_update();
                button_task();
                pacer_wait();

                // WAITS CERTAIN AMOUNT OF TIME THEN INCREASES y VALUE
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

                // SETS LED TO LOW
                ledmat_pixel_set(arrow_x, arrow_y, 1);
                // nav.h
                move(arrow_x, arrow_y, &correctCount, &wrongMove, &score);

                if (correctCount == 5) {
                    led_set(LED1, 1);
                    sabotagePowerUp = true;
                    correctCount = 0;
                }

                current_tick++;
            if (score == 50) {
                gameOver = true;
            }

            } if (gameOver) {
                // ADD LOSE SCREEN
                if (lost) {
                    tinygl_text("L");
                }
                else {
                    tinygl_text("W");
                }
                tinygl_update();
            }
            pacer_wait();
        }
    }
}
