#include "system.h"
#include "navswitch.h"
#include <stdio.h>

#include "nav.h"

// CHECKS TO SEE IF THE PLAYER HAS MADE A RIGHT MOVE OR A WRONG MOVE & ALSO 
// CHECKS TO SEE HOW MANY WRONG MOVES THE PLAYER HAS MADE IN A ROW
void move(int arrow_x, int arrow_y, int* correctCount, int* wrongMove, int* score) {

    // VARIABLES
    int y_check = 1;

    // LEFT
    if (navswitch_push_event_p(NAVSWITCH_WEST)) {
        if (arrow_y >= y_check && arrow_x == 0) {
            (*correctCount)++;
            (*score)++;
            *wrongMove = 0;
        }
        else {
            *correctCount = 0;
            (*wrongMove)++;
        }
    }
    // LEFT-MIDDLE
    else if (navswitch_push_event_p(NAVSWITCH_NORTH)) {
        if (arrow_y >= y_check && arrow_x == 1) {
            (*correctCount)++;
            (*score)++;
            *wrongMove = 0;
        }
        else {
            *correctCount = 0;
            (*wrongMove)++;
        }
    }

    // MIDDLE
    else if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
        if (arrow_y >= y_check && arrow_x == 2) {
            (*correctCount)++;
            (*score)++;
            *wrongMove = 0;
        }
        else {
            *correctCount = 0;
            (*wrongMove)++;
        }
    }

    // RIGHT-MIDDLE
    else if (navswitch_push_event_p(NAVSWITCH_SOUTH)) {
        if (arrow_y >= y_check && arrow_x == 3) {
            (*correctCount)++;
            (*score)++;
            *wrongMove = 0;
        }
        else {
            *correctCount = 0;
            (*wrongMove)++;
        }
    }
    // RIGHT
    else if (navswitch_push_event_p(NAVSWITCH_EAST)) {
        if (arrow_y >= y_check && arrow_x == 4) {
            (*correctCount)++;
            (*score)++;
            *wrongMove = 0;
        }
        else {
            *correctCount = 0;
            (*wrongMove)++;
        }
    }
}
