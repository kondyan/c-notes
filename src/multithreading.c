#include <stdlib.h>
#include <stdio.h>
#include <threads.h>

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

// define thread count for later

#define THREAD_COUNT 5

int run (void *arg) {
    int i = *(int*)arg; // We'll pass in an int* from thrd_create()

    free(arg);

    printf("THREAD: Running thread with arg %d\n", i);

    return i;
}



int void_run(void *arg) {
    (void)arg;


    printf("Thread running! %lu\n",thrd_current()); // thrd_current() refers to the ID of the thread (non-portable)
    printf("Thread running!\n");

    return 0;
}

int static_run(void *arg) {
    int n = *(int *)arg;

    free(arg);

    static int foo = 10; // shared between threads

    int x = foo; // local copy

    printf("Thread %d: x = %d, foo = %d\n",n,x,foo);

    if (x != foo) {
        printf("Thread %d: Craziness! x != foo! %d != %d\n", n,x,foo);
    }

    foo++;

    return 0;
}

int threadlocal_run(void *arg) {
    int n = *(int *)arg;

    free(arg);

    thread_local static int foo = 10; // now its not shared :)

    int x = foo; // local copy

    printf("Thread %d: x = %d, foo = %d\n",n,x,foo);

    if (x != foo) {
        printf("Thread %d: Craziness! x != foo! %d != %d\n", n,x,foo);
    }

    foo++;

    return 0;
}

tss_t str;

void some_function(void) {

    // retriever per-thread value of the string

    char *tss_string = tss_get(str);

    printf("TSS sring: %s\n", tss_string);
}

int tss_run(void*arg) {
    int serial = *(int*)arg;
    free(arg);

    char *s = malloc(64);
    sprintf(s, "thread %d! :", serial);

    tss_set(str,s); // save the variable in tss_t

    return 0;
}

mtx_t serial_mtx; // MUTEX VARIABLE

int mutex_run(void *arg) {
    (void)arg;

    static int serial = 0; // shared static variable!

    // Threads will block before the mtx_lock() gets unlocked

    mtx_lock(&serial_mtx);

    printf("Thread running! %d\n", serial);

    serial++;

    // done getting and setting the data, free the lock.
    // this will unblock threads on the mtx_lock() call

    mtx_unlock(&serial_mtx);

    return 0;
}

#define VALUE_COUNT_MAX 5
int value[VALUE_COUNT_MAX]; // shared global
int value_count = 0; // shared global

mtx_t value_mtx; // mutex around value
cnd_t value_cnd; // condition variable on a value

int cndrun(void *arg) {
    (void)arg;

    // run infinite loop
    for(;;) {
        mtx_lock(&value_mtx);

        while (value_count < VALUE_COUNT_MAX) {
            printf("Thread is waiting\n");
            cnd_wait(&value_cnd, &value_mtx); // condition wait
            // this releases the mutex under the hood until cnd_signal() is called
        }

        printf("Thread: is awake!\n");

        int t = 0;

        for (int i =0; i < VALUE_COUNT_MAX; i++) {
            t += value[i];
        }

        printf ("Thread: total is %d\n", t);

        // reset input index for the main thread
        value_count = 0;

        mtx_unlock(&value_mtx); // mutex unlock

    }
    return 0;
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
    When the main thread decides to, it can wait for the child thread to complete by calling thrd_join().
    Generally you must thrd_join() the thread to clean up after it or else you’ll leak memory

    */

    // {
    //     thrd_t t;
    //     int arg = 3490;

    //     printf("Launching a thread\n");

    //     /*first the pointer to thread obj, function and then the arguments of the function as pointers*/
    //     thrd_create(&t, run, &arg);

    //     printf("Doing other things while the thread runs\n");

    //     printf("Waiting for the thread to complete...\n");

    //     int res; // holds return value

    //     thrd_join(t, &res); // pointer to the value that should hold the return value of the function running by the thread

    //     printf("Thread exited with return value %d\n", res);
    // }


    // let's launch 5 threads

    {
        thrd_t t[THREAD_COUNT]; // array of threads

        int i;

        printf("Launching threads...\n");

        for (int i =0; i < THREAD_COUNT; i++) {
            // we pass a pointer to i
            // each thread see the SAME POINTER
            // the threads will print weird things
            // because the value of i changes here in the main thread


            int *arg = malloc(sizeof *arg);// allocate a pointer of size int
            *arg = i; // copy i to the address

            thrd_create(t + i, run, arg);
        }

        printf("Doing other things while the thread runs...\n");
        printf("Waiting for the thread to complete...\n");

        for (int i = 0; i < THREAD_COUNT; i++) {
            int res;
            thrd_join(t[i], &res);

            printf("Thread %d complete!\n", res);
        }

        printf("All Threads complete!\n");
    }

    // the weird thing with the output happens because of race conditions
    // main thread is modifying i before the thread has a chance to copy it.
    
    // we need to have per-thread variables that we can pass with the arg

    // we can use malloc() to allocate some space for it and the free it inside
    // of the thread, let's try (the code above is already optimized, pass pointer to i into the thread param to see weird behaviour)


    // Detaching Threads
    {
        // this is a way to fire and forget a thread, you don't have to thrd_join() later

        thrd_t t;
        thrd_create(&t, void_run, NULL);
        thrd_detach(t);

    }

    {
        thrd_t t;

        for (int i = 0; i < THREAD_COUNT; i++) {
            thrd_create(&t, void_run, NULL);
            thrd_detach(t); // detach here
        }

        // sleep for a second so all threads finish
        thrd_sleep(&(struct timespec){.tv_sec=1},NULL);
    }


    // Thread local data

    // Threads don't have memory beyond local variables
    // if you need static or file scope variable, all threads see the same thing

    {
        thrd_t t[THREAD_COUNT];

        for (int i = 0; i < THREAD_COUNT; i++) {
            int *n = malloc(sizeof *n); // holds serial number of the thread
            *n = i;
            thrd_create(t + 1, static_run, n);
        }

        for (int i = 0; i < THREAD_COUNT; i++) {
            thrd_join(t[i], NULL);
        }
    }

    // we can solve this problem using mutex
    // lets see other options first though

    // _Thread_local

    {
         thrd_t t[THREAD_COUNT];

        for (int i = 0; i < THREAD_COUNT; i++) {
            int *n = malloc(sizeof *n); // holds serial number of the thread
            *n = i;

            // note that thread_local variable in a block scope must be static/extern
            thrd_create(t + 1, threadlocal_run, n);
        }

        for (int i = 0; i < THREAD_COUNT; i++) {
            thrd_join(t[i], NULL);
        }
    }

    // Another option: Thread-specific storage (TSS)

    // we use tss_dtor_t type which is a pointer to a function that returns void
    // and take void*, where void* points to data stored in the variable
    // tss_dtor_t will free() everything for you but it's not as slick as
    // thread_local


     {
        thrd_t t[THREAD_COUNT];

        // make new tss variable, the free() function is the destructor
        tss_create(&str,free);

        for (int i = 0; i < THREAD_COUNT; i++) {
            int *n = malloc(sizeof *n); // holds serial number of the thread
            *n = i;

            // note that thread_local variable in a block scope must be static/extern
            thrd_create(t + 1, threadlocal_run, n);
        }

        for (int i = 0; i < THREAD_COUNT; i++) {
            thrd_join(t[i], NULL);
        }

        // All threads are done, so free everything up
        tss_delete(str);
    }

    // finally mutexes

    // why mutexes?

    // you can allow a single thread into a critical section of code at a time
    // you can protect that section with a mutex (Short for “mutual exclusion”, AKA a “lock”
    // on a section of code that only one thread is permitted to execute.)

    // let's see the example
    {
        thrd_t t[THREAD_COUNT];

        mtx_init(&serial_mtx, mtx_plain); // initialize mtx variable located in the file scope

        for (int i = 0; i < THREAD_COUNT; i++) {
            thrd_create(t+i, mutex_run, NULL);
        }

        for (int i = 0; i < THREAD_COUNT; i++) {
            thrd_join(t[i],NULL);
        }

        // done with mutext, destroy it

        mtx_destroy(&serial_mtx);
    }

    // so where the mutext block starts and ends, we have zone that
    // no multiple threads can access in the same time, we isolate it
    // in a way

    // there are different mutext types you can create using mtx_inti()
    // some of these are result of bitwise OR, see below:

    /*
        Type 	                    Description
        mtx_plain 	                Regular ol’ mutex
        mtx_timed 	                Mutex that supports timeouts
        mtx_plain|mtx_recursive 	Recursive mutex
        mtx_timed|mtx_recursive 	Recursive mutex that supports timeouts
    */

    // example with timespec_get() for mutexes with timeouts

    {
        struct timespec timeout;

        timespec_get(&timeout, TIME_UTC); // get curr time
        timeout.tv_sec+=1;

        int result = mtx_timedlock(&serial_mtx, &timeout);

        if (result == thrd_timedout) {
            printf("Mutex lock timed out!\n");
        }
    }


    // condition variables

    // these are the last piece of the puzzle to create performant multithreaded
    // applications.

    // condition variables provide a way for threads to go sleep until some event
    // on another thread occurs.

    // this works with mutexes too since what we're going to wait on generally
    // depends on the value of some data and that data generally needs to be protected
    // by a mutex

    // so condition variables doesn't really hold any data, it just keeps track of
    // the status for a thread or group of threads so C knows when to proceed with
    // the current thread

    // for condition variables we use:
    // cnd_t:           type
    // cnd_init():      initialization
    // cnd_wait():      wait
    // cnd_signal():    wake up
    // cnd_destroy():   clean up 

    {
        thrd_t t;

        // spawn a new thread
        thrd_create(&t, cndrun, NULL);
        thrd_detach(t);

        // setup mutex and conditional variable
        mtx_init(&value_mtx, mtx_plain);
        cnd_init(&value_cnd);

        for (;;) {
            int n;

            scanf("%d", &n);

            mtx_lock(&value_mtx); // lock mutex

            value[value_count++] = n;

            if (value_count == VALUE_COUNT_MAX) {
                printf("Main: signaling thread\n");
                cnd_signal(&value_cnd);
            }

            mtx_unlock(&value_mtx);
        }

        // clean up, theres infinite loop but it's the proper way to do it always

        mtx_destroy(&value_mtx);
        cnd_destroy(&value_cnd);
    }

    // timed condition wait
    // there's a variant of cnd_wait() that allows you to specify a timeout
    // so you can stop waiting
    // the end of the timeout doesn't mean the thread will be instantly brought
    // back to life because it still needs to wait until the other thread
    // releases mutex, but you won't be waiting until cnd_signal() happens

    // we use cnd_timedwait() for that

    {
        struct timespec timeout;

        timespec_get(&timeout, TIME_UTC); // current time
        timeout.tv_sec+=1; // one second after now

        int result = cnd_timedwait(&value_cnd, &value_mtx, &timeout);

        if (result == thrd_timedout) {
            printf("Condition variable timed out!\n");
        }
    }

}