#include <stdint.h>
#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>


// This is a more challenging aspect of multithreading in C.

// So these notes are very important for me :)

// first of all test for atomic support

#if __STDC_VERSION__ < 201112L || __STDC_NO_ATOMICS == 1
#define HAS_ATOMICS 0
#else
#define HAS_ATOMICS 1
#endif

/*
On some systems, you might need to add -latomic to the end of 
your compilation command line to use any functions in the header file.
*/



// ATOMIC VARIABLES

// if you have a shared atomic variable and you write to it from one thread,
// that write will be all-or-nothing in a different thread.

// the other thread will see the entire write of a 32-bit value for example.
// Not half of it. There's no way for one thread to interrupt another that is the
// middle of an atomic multi-byte write

// theres like a lock around getting and setting of that one variable.

// demo


atomic_int x;

int thread1(void *arg){
    (void)arg;

    printf("Thread 1: Sleeping for 1.5 seconds\n");
    thrd_sleep(&(struct timespec){.tv_sec=1, .tv_nsec=500000000}, NULL);

    printf("Thread 1: Setting x to 3490\n");
    x = 3490;

    printf("Thread 1: Exiting\n");
    return 0;
}

int thread2(void *arg) {
    (void)arg;

    printf("Thread 2: Waiting for 3490\n");
    while (x != 3490) {} // spin here

    printf("Thread 2: Got 3490, exiting!\n");
    return 0;
}



int main(void) {



    {
        x = 0;
        thrd_t t1,t2;

        thrd_create(&t1,thread1,NULL);
        thrd_create(&t2,thread2,NULL);

        thrd_join(t1,NULL);
        thrd_join(t2,NULL);

        printf("Main        : Threads are done, so x better be 3490\n");
        printf("Main        : And indeed, x == %d\n", x);
    }
    return 0;
}