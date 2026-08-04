#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

#ifdef __STDC_NO_THREADS__
#error I need threads to build this program!
#endif

// Threads are availabe since C11+

// u need to specify like this when building
//  gcc -std=c11 -o foo foo.c -lpthreads

/*
Threads are a way to have all those shiny CPU cores you paid for do work for you in the same program.

Normally, a C program just runs on a single CPU core. But if you know how to split up the work, you can give pieces of it to a number of threads and have them do the work simultaneously.

Though the spec doesn’t say it, on your system it’s very likely that C (or the OS at its behest) will attempt to balance the threads over all your CPU cores.

And if you have more threads than cores, that’s OK. You just won’t realize all those gains if they’re all trying to compete for CPU time.
*/


// what can you do?

/*
you can create a thread. it will begin running the function you specify.
The parent thread that spwaned it will also continue to run.

you can wait for the thread to complete (joining)

or if you don't care you can detach it

a thread can explicitly exit, or it can implicitly call it
quits by returning from its main function.

a thread can also sleep for a preiod of time,
basically doing nothing while other threads run

The main() program is a thread, as well

additionally there's thread local storage, mutexes
and conditional variables.

If a standard library function 
is maintaining state between calls in a variable you don’t 
own, or if a function is returning a pointer to a 
thing that you didn’t pass in, it’s not threadsafe.
*/

int run (void *arg) {
    int *a = arg; // We'll pass in an int* from thrd_create()

    printf("THREAD: RUnning thread with arg %d\n", *a);

    return 12;
}

int main(void) {

    // let's create some threads and wait for them to complete (join)

    // every thread has a type of thrd_t
    // every thread is given a new ID

    // when you create a thread
    // you HAVE to give it a pointer to a function to run
    // you need to pass the arguments to the function or NULL if there aren't any

    // when you want to join it (wait until it completes) you have to specify it's ID
    // so C knows which one to wait for.

    /*
    
    So the basic idea is:

    Write a function to act as the thread’s “main”. It’s not main()-proper, but analogous to it. The thread will start running there.
    From the main thread, launch a new thread with thrd_create(), and pass it a pointer to the function to run.
    In that function, have the thread do whatever it has to do.
    Meantimes, the main thread can continue doing whatever it has to do.
    When the main thread decides to, it can wait for the child thread to complete by calling thrd_join(). Generally you must thrd_join() the thread to clean up after it or else you’ll leak memory201

    */

    {
        int arg = 3490;
        HANDLE hThread = CreateThread(NULL,0,run,&arg,0,NULL);
    }

    return 0;
}