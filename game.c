/*
* MAIN GAME FILE
* AUTHORS: Zac Avis, Sam Willems
* DESC: Main game file
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
#define WINCOUNT 50

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
static int arrow_speed = 100;
static bool sabotagePowerUp = false;

// IF PLAYER HAS A POWER UP & PUSHES BUTTON THEN IT'LL SPEED OPPONENTS GAME UP
static void button_task(void) {

    button_update();

    if (button_push_event_p(BUTTON1) && sabotagePowerUp) {
        ir_uart_putc('P');
        sabotagePowerUp = false;
        led_set(LED1, 0);
    }

}

// MAIN
int main(void) {

    // INITIALISE
    system_init();
    ir_uart_init();
    ledmat_init();
    button_init();
    pacer_init(PACER_RATE);
    tinygl_init(LOOP_RATE);
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
        // BOTH PLAYERS ARE READY SO START THE GAME
        if (started) {

            // RECEIVE IR FROM OPPONENT
            if (ir_uart_read_ready_p()) {
                char ir_received = ir_uart_getc();
                if (ir_received == 'P') {
                    arrow_speed = arrow_speed - (arrow_speed / 10);
                } else if (ir_received == 'F') {
                    gameOver = true;
                } else if (ir_received == 'W') {
                    lost = true;
                    gameOver = true;
                }
            }

            // ENDS GAME & SENDS IR TO OPPONENT TO END THEIR GAME
            if (wrongMove == 3) {
                ir_uart_putc('F');
                gameOver = true;
                lost = true;
            }

            // RANDOMLY CHOOSES POSITION WHERE A DOT WILL FALL,
            if (!gameOver) {
                // PACES LOOP, CLEARS THE LOOP, UPDATES NAV SWITCH & BUTTON
                pacer_wait();
                clear_screen();
                navswitch_update();
                button_task();

                // WAITS CERTAIN AMOUNT OF TIME THEN INCREASES y VALUE
                if (current_tick % arrow_speed == 0) {
                    arrow_y++;
                }

                // SETS LED TO LOW
                ledmat_pixel_set(arrow_x, arrow_y, 1);
                // nav.h
                move(arrow_x, arrow_y, &correctCount, &wrongMove, &score);

                // IF YOU GET 5 CORRECT MOVES IN A ROW THEN YOU GET A POWER UP TO SPEED UP OPPONENTS GAME
                if (correctCount == 3) {
                    led_set(LED1, 1);
                    sabotagePowerUp = true;
                    correctCount = 0;
                }

                // IF SCORE IS WINCOUNT YOU WIN
                if (score == WINCOUNT) {
                    ir_uart_putc('W');
                    gameOver = true;
                }

                //  IF ARROW IS MORE THAN OR EQUAL TO 7 ADD ANOTHER ARROW AT A RANDOM LOCATION 
                if (arrow_y >= 7) {
                    arrow_y = 0;
                    arrow_x = rand() % 5;
                }

                current_tick++;
                if (current_tick >= 10000) {
                    current_tick = 0;
                }

            // IF GAME IS OVER, DISPLAYS 'W' IF YOU WIN, 'L' IF YOU LOSE
            } if (gameOver) {
                if (lost) {
                    tinygl_text("L");
                }
                else {
                    tinygl_text("W");
                }
                tinygl_update();
            }
        }
    }
}
