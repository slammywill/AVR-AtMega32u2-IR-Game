#include "system.h"
#include "navswitch.h"
#include <stdio.h>

#include "nav.h"

// VARIABLES
static int moveDone = 0;

// CHECKS TO SEE IF THE PLAYER HAS MADE A RIGHT MOVE OR A WRONG MOVE & ALSO 
// CHECKS TO SEE HOW MANY WRONG MOVES THE PLAYER HAS MADE IN A ROW
void move(int arrow_x, int arrow_y, int* correctCount, int* wrongMove, int* score) {

    // VARIABLES
    int y_check = 6;

    // LEFT
    if (navswitch_push_event_p(NAVSWITCH_WEST) && moveDone == 0) {
        moveDone++;
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
    else if (navswitch_push_event_p(NAVSWITCH_NORTH) && moveDone == 0) {
        moveDone++;
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
    else if (navswitch_push_event_p(NAVSWITCH_PUSH) && moveDone == 0) {
        moveDone++;
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
    else if (navswitch_push_event_p(NAVSWITCH_SOUTH) && moveDone == 0) {
        moveDone++;
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
    else if (navswitch_push_event_p(NAVSWITCH_EAST) && moveDone == 0) {
        moveDone++;
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

    // IF ARROW TOUCHES BOTTOM ROW & PLAYER HASN'T MADE A MOVE
    else if (moveDone == 0 && arrow_y >= 7) {
        *correctCount = 0;
        (*wrongMove)++;
    }

    // RESET
    if (moveDone == 1 && arrow_y >= 7) {
        moveDone = 0;
    }
}
