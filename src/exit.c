#include <stdlib.h>
#include <stdio.h>

void on_exit_1(void) {
    printf("Exit handler 1 called!\n");
}


void on_exit_2(void) {
    printf("Exit handler 2 called!\n");
}


int main(void) {

    // these will be executed in reverse from the return statement order
    // atexit(on_exit_1);
    // atexit(on_exit_2);

    // may be left out, will be placed
    // by compiler for you

    /*
    quick_exit()

        Open files might not be flushed.
        Temporary files might not be removed.
        atexit() handlers won’t be called.

        you need to define
        at_quick_exit(callback)
    */

    // at_quick_exit(on_exit_1);

    // quick_exit(0);



    return 0;
}