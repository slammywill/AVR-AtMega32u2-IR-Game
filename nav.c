#include "system.h"
#include "navswitch.h"
#include <stdio.h>

#include "nav.h"

void move(int arrow_x, int arrow_y, int* correctCount) {

    int y_check = 1;

    // LEFT
    if (navswitch_push_event_p(NAVSWITCH_NORTH)) {
        if (arrow_y >= y_check && arrow_x == 1) {
            (*correctCount)++;
        }
        else {
            *correctCount = 0;
        }
    }

    // RIGHT
    else if (navswitch_push_event_p(NAVSWITCH_SOUTH)) {
        if (arrow_y >= y_check && arrow_x == 3) {
            (*correctCount)++;
        }
        else {
            *correctCount = 0;
        }
    }
    // LEFT-MIDDLE
    else if (navswitch_push_event_p(NAVSWITCH_EAST)) {
        if (arrow_y >= y_check && arrow_x == 0) {
            (*correctCount)++;
        }
        else {
            *correctCount = 0;
        }
    }
    // RIGHT-MIDDLE
    else if (navswitch_push_event_p(NAVSWITCH_WEST)) {
        if (arrow_y >= y_check && arrow_x == 4) {
            (*correctCount)++;
        }
        else {
            *correctCount = 0;
        }
    }
    // MIDDLE
    else if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
        if (arrow_y >= y_check && arrow_x == 2) {
            (*correctCount)++;
        }
        else {
            *correctCount = 0;
        }
    }
}