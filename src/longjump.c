#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>

jmp_buf env;

void depth2(void) {
    printf("Entering depth 2\n");
    longjmp(env, 3490);
    printf("Leaving depth 2\n");
}

void depth1(void){
    printf("Entering depth 1\n");
    depth2();
    printf("Leaving depth 1\n");
}

// setjmp and longjmp allow us to bail back in the
// program execution just like goto does skipping some
// code parts   
int main(void) {

    switch (setjmp(env))
    {
    case 0:
        printf("Calling into functions, setjmp() returned 0\n");
        depth1();
        printf("Returned from functions");
        break;
    case 3490:
        printf("Bailed back to main, setjmp() returned 3490\n");
    

    default:
        break;
    }

    {
        // some pitfalls

        // here if we jump back x will be indeterminate

        int x = 20;

        if (setjmp(env) == 0) {
            x = 30;
        }
    }
    {
        // to fix this use volatile

        volatile int x = 20;

        if (setjmp(env) ==0) {
            x = 30;
        }
    }

    // a list of undefined behaviours
    // when calling longjmp()

    /*
    You didn’t call setjmp() earlier
    You called setjmp() from another thread
    You called setjmp() in the scope of a variable length array (VLA), and execution left the scope of that VLA before longjmp() was called.
    The function containing the setjmp() exited before longjmp() was called.
    */

    return 0;
}