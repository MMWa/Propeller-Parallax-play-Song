//
// Created by wahba on 25/02/2017.
//

#include "simpletools.h"

#define A 220;
#define G 195;
#define F 175;
#define E 165;
#define D 147;
#define C 131;

#define STACK_SIZE 16
static int cog1_stack[STACK_SIZE];
static _thread_state_t thread_data;         //per-thread library variables ("Thread Local Storage")

//Cog Shared Variables
volatile int Frequency = 1;
volatile unsigned int pins;
volatile int exec_flag = 0;

void toggle_pins(void *arg) {
    unsigned int nextcnt;
    unsigned int wait_time = _clkfreq / 1000;

    // direction must be set from within using cog
    _DIRA = pins;       //sets pins direction
    _OUTA ^= pins;      //set pin on first time!
    exec_flag = 1;
    nextcnt = _CNT + wait_time;     //used in another instance
    for (;;) {
        //recalculate the wait time
        wait_time = _clkfreq / (Frequency * 2);
        //output to pins
        _OUTA ^= pins;
        //sleep until _CNT == nextcnt, and return the new _CNT + wait_time
        nextcnt = __builtin_propeller_waitcnt(nextcnt, wait_time);
    }
}

void playOne(int multiplier) {
    //interval multiplier
    //150 works well
    Frequency = D;
    pause(3 * multiplier);
    Frequency = E;
    pause(3 * multiplier);
    Frequency = F;
    pause(3 * multiplier);
    Frequency = G;
    pause(3 * multiplier);
    Frequency = F;
    pause(3 * multiplier);
    Frequency = E;
    pause(3 * multiplier);
    Frequency = D;
    pause(3 * multiplier);
    Frequency = C;
    pause(3 * multiplier);
    Frequency = A;
    pause(4 * multiplier);

    Frequency = G;
    pause(2 * multiplier);
    Frequency = A;
    pause(2 * multiplier);
}

void playWontBackDown(int multiplier) {
    Frequency = D;
    pause(3 * multiplier);
    Frequency = D;
    pause(1 * multiplier);

    Frequency = C;
    pause(1 * multiplier);
    Frequency = D;
    pause(1 * multiplier);
    //------
    Frequency = C;
    pause(1 * multiplier);
    Frequency = D;
    pause(5 * multiplier);
    //------

    Frequency = C;
    pause(1 * multiplier);
    Frequency = D;
    pause(1 * multiplier);
    //------
    Frequency = C;
    pause(1 * multiplier);
    Frequency = D;
    pause(5 * multiplier);
    //------

    Frequency = C;
    pause(1 * multiplier);
    Frequency = D;
    pause(1 * multiplier);
    //------
    Frequency = G;
    pause(1 * multiplier);
    Frequency = A;
    pause(1 * multiplier);
    Frequency = C;
    pause(1 * multiplier);
    Frequency = D;
    pause(2 * multiplier);
}

int main() {

    int pin[] = {5};
    pins = 0;                       //clear all pins
    pins |= 1 << pin[0];
    DIRA |= 1 << 31;                //make sure serial tx pin is output

    _start_cog_thread(cog1_stack + STACK_SIZE, toggle_pins, NULL, &thread_data);

    while (1) {
        if (exec_flag == 1) {
            printf("Playing \n");
            playWontBackDown(150);

        }
    }
}
