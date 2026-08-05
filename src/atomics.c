#include <stdint.h>
#include <stdio.h>
#include <threads.h>
#include <stdbool.h>
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


    // synchronization

    // this is all about when a certain variable write becomes visible
    // in another thread. And no it's not right away


    // There are tons of things that can delay CPU writes

    // unsynchronized memory accesses can appear out of order depending on which
    // thread is doing the observing, as if the lines of code themselves had been
    // rearranged

    {
        int x, y; // global

        // what happens first? write to x or to y?
        x = 2;
        y = 3;

        printf("%d %d \n", x, y);
    }

    // we don't know CPU might silently reverse lines 102 and 103.
    // here's some pseudocode for the same thing but with multiple threads

    /*
    int x = 0, y = 0;

    thread1() {
        x = 2;
        y = 3;
    }

    thread2() {
        while (y != 3) {}  // spin
        printf("x is now %d\n", x);  // 2? ...or 0?
    }
    */

    // if x would be set first the output is 2 but otherwise if CPU
    // decides to flip those 2 assignment lines what do we get?

    // so when we don't exactly know that we can say that 2 threads are not
    // synchronized

    // but we can ensure an agreement with the code using atomic variables
    // writes to an atomic variable says "anyone who reads this atomic variable in
    // the future will also see the changes i made to memory up to and including
    // atomic variable"

    // now we can fix our example easily

    /*
    int x = 0;
    atomic int y = 0;  // Make y atomic

    thread1() {
        x = 2;
        y = 3;             // Synchronize on write
    }

    thread2() {
        while (y != 3) {}  // Synchronize on read
        printf("x is now %d\n", x);  // 2, period.
    }
    */

    // important to note:

    // nothing sleeps. Both threads are running until they exit. Even the one stuck
    // in the spin loop isn't blocking anyone else from running

    // synchronization happens when one thread reads an atomic variable another
    // thread wrote. So when thread 2 reads y, all previous memory writes in thread 1
    // will be visible in thread 2.


    // x isn't atomic because we are not synchronizing over x

    // forcing synchronization on variables that don't require it is inefficient
    // and can get a lot slower than just regular variable. So we don't use atomics
    // in the particular application unless we have to.

    // BASICS END

    // RABIT HOLE START ;)

    // Acquire and Release

    // Terminology

    // atomic variable read  = acquire operation
    // atomic variable write = release operation

    // Read = Load = Acquire (comparing atomic variable or read it to copy it to another value)
    // Write = Store = Release (assign a value into atomic variable)


    // Acquire/release form the basis for synchronization

    // more details

    /*
    With read/load/acquire of a particular atomic variable:

    All writes (atomic or non-atomic) in another thread 
    that happened before that other thread wrote/stored/released 
    this atomic variable are now visible in this thread.

    The new value of the atomic variable set by the other 
    thread is also visible in this thread

    No reads or writes of any variables/memory in the current 
    thread can be reordered to happen before the acquire

    The acquire acts as a one way barrier when it comes to code reordering
    reads and writes in the current thread can be moved down from before the acquire
    to after it, but more  importantly for synchronization, nothing can move up from
    after the acquire to before it

    

    With write/store/release of a particular atomic variable

    All writes (atomic or non-atomic) in another thread that happened before that order
    thread wrote/stored/released this atomic variable are now visible in this thread

    the value written to this atomic variable by this thread is also visible to other threads

    No reads or writes of any variables/emmeory in the current thread can be reordered to
    happen after this release.

    the release acts as a one way barrier when it comes to code reordering: reads and writes
    in the current thread can be moved up from after the release to before it but not the other
    way around.

    return 0;
    */

    /*
    int x, y, z = 0;
    atomic_int a = 0;

    thread1() {
        x = 10;
        y = 20;
        a = 999;  // Release
        z = 30;
    }

    thread2()
    {
        while (a != 999) { } // Acquire

        assert(x == 10);  // never asserts, x is always 10
        assert(y == 20);  // never asserts, y is always 20

        assert(z == 0);  // might assert!!
    }

    so here thread2 has a simple guarantee that x and y value will be consistent
    because they were set before the atomic release in thread 1 (they will be executed first)

    but thread 2 can't be sure that z is still 0, they assignment might have moved to before
    the a variable assignment

    An important note: releasing one atomic variable has
    no effect on acquires of different atomic variables.
    Each variable is isolated from the others.
    */

    // Sequential Consistency

    /*
    Sequential consistency is what's called a memory ordering. There are many
    memory orderings, but sequential consistency is the sanest C has to offer.
    It's also the default, you have to go out of your way to use other memory
    orderings

    You know by now that compiler or CPU can rearrange memory reads and writes
    in a single thread as long as it follows the as-if rule.

    Above we talked about how can put breaks on this using atomics and it's 
    synchronization ability


    If operations are sequentially consistent, it means at the end of the day, 
    when all is said and done, all the threads can kick up their feet, 
    open their beverage of choice, and all agree on the order in which 
    memory changes occurred during the run. 
    And that order is the one specified by the code.

    One won’t say, “But didn’t B happen before A?” if the rest of them say, 
    “A definitely happened before B”. They’re all friends, here.

    other memory orderings do relaxation of these rules in some kinds of ways
    so if you really know what you're doing you can use them speed boost
    but let's stick to the default and the safe one first
    */

    // Atomic Assignments and Operators

    {
        // atomic_int x = 0;

        // void thread1(void) {
        //     x = x + 3; // NOT atomic!
        // }

        // since we have read of x on the right side and the assignment on the left side
        // another thread could sneak in the middle and make you unhappy

        // you can use shorthand += to get an atomic operation

        // atomic_int x = 0;

        // void thread1(void) {
        //     x+=3; // ATOMIC!
        // }

        // in this case nobody can jump in between

    }

    // these are read-modify-write operations with sequential consistency

    // a is atomic

    /*
    a++       a--       --a       ++a
    a += b    a -= b    a *= b    a /= b    a %= b
    a &= b    a |= b    a ^= b    a >>= b   a <<= b
    */

    // these functions do behind the scene synchronization

    /*
    call_once()      thrd_create()       thrd_join()
    mtx_lock()       mtx_timedlock()     mtx_trylock()
    malloc()         calloc()            realloc()
    aligned_alloc()
    */

    // call_once() synchronizes with all subsequent calls to call_once() so they
    // see the flag set by another thread

    // thrd_create() synchronizes with the shared memory writes from the parent
    // thread from before the thrd_create() call.

    // thrd_join() when thread dies, it synchronizes with the function. THe thread that
    // has called thrd_join() can be assured that it can see all the late thread's shared writes.

    // mtx_lock() earlier calls to mtx_unlock() on the same mutex synchronize on this call
    // mtx_unlock() performs a release of the mutex variable, assuring any subsequent thread
    // that makes an acquire with mtx_lock() can see all the shared memory changes in the critical
    // section

    // mtx_timdelock() and mtx_trylock() similar to the situation with mtx_lock,
    // if this call succeeds, earlier calls to mtx_unlock() synchronize with this one.

    // Dynamic Memory Functions: if you allocate memory, it synchronizes with the previous
    // deallocation of that same memory. And allocations and deallocation of that partiuclar
    // memory region happen in a single total order that all threads can agree upon. I think the
    // iadea here is that the deallocation can wipe the region if it chooses, and want to be sure
    // that a subsequent allocaiton doesn't see the non-wiped data.

    // atomic type specifier, qualifier

    /*
    built in atomic types


    Atomic type 	        Longhand equivalent
    atomic_bool 	        _Atomic _Bool
    atomic_char 	        _Atomic char
    atomic_schar 	        _Atomic signed char
    atomic_uchar 	        _Atomic unsigned char
    atomic_short 	        _Atomic short
    atomic_ushort 	        _Atomic unsigned short
    atomic_int 	            _Atomic int
    atomic_uint 	        _Atomic unsigned int
    atomic_long 	        _Atomic long
    atomic_ulong 	        _Atomic unsigned long
    atomic_llong 	        _Atomic long long
    atomic_ullong 	        _Atomic unsigned long long
    atomic_char16_t     	_Atomic char16_t
    atomic_char32_t     	_Atomic char32_t
    atomic_wchar_t 	        _Atomic wchar_t
    atomic_int_least8_t 	_Atomic int_least8_t
    atomic_uint_least8_t 	_Atomic uint_least8_t
    atomic_int_least16_t 	_Atomic int_least16_t
    atomic_uint_least16_t 	_Atomic uint_least16_t
    atomic_int_least32_t 	_Atomic int_least32_t
    atomic_uint_least32_t 	_Atomic uint_least32_t
    atomic_int_least64_t 	_Atomic int_least64_t
    atomic_uint_least64_t 	_Atomic uint_least64_t
    atomic_int_fast8_t 	    _Atomic int_fast8_t
    atomic_uint_fast8_t 	_Atomic uint_fast8_t
    atomic_int_fast16_t 	_Atomic int_fast16_t
    atomic_uint_fast16_t 	_Atomic uint_fast16_t
    atomic_int_fast32_t 	_Atomic int_fast32_t
    atomic_uint_fast32_t 	_Atomic uint_fast32_t
    atomic_int_fast64_t 	_Atomic int_fast64_t
    atomic_uint_fast64_t 	_Atomic uint_fast64_t
    atomic_intptr_t 	    _Atomic intptr_t
    atomic_uintptr_t 	    _Atomic uintptr_t
    atomic_size_t 	        _Atomic size_t
    atomic_ptrdiff_t    	_Atomic ptrdiff_t
    atomic_intmax_t 	    _Atomic intmax_t
    atomic_uintmax_t 	    _Atomic uintmax_t
    */

    // u can do typedef too

    {
        typedef _Atomic(double) atomic_double;

        atomic_double f;

    }

    // type qualifier
    {
        _Atomic(int) i; // type specifier
        _Atomic int j; // type qulifier

    }

    {
        _Atomic volatile int k; // qualified atomic variable

    }


    // lock free atomic variables

    /*
    If you use an atomic type, you can be assured that accesses 
    to that type will be atomic… but there’s a catch:
    if the hardware can’t do it, it’s done with a lock, instead.
    */


    /*
    First of all, some macros—you can use these at compile time with #if. They apply to both signed and unsigned types.

        Atomic Type 	        Lock Free Macro
        atomic_bool 	        ATOMIC_BOOL_LOCK_FREE
        atomic_char 	        ATOMIC_CHAR_LOCK_FREE
        atomic_char16_t     	ATOMIC_CHAR16_T_LOCK_FREE
        atomic_char32_t     	ATOMIC_CHAR32_T_LOCK_FREE
        atomic_wchar_t      	ATOMIC_WCHAR_T_LOCK_FREE
        atomic_short 	        ATOMIC_SHORT_LOCK_FREE
        atomic_int 	            ATOMIC_INT_LOCK_FREE
        atomic_long 	        ATOMIC_LONG_LOCK_FREE
        atomic_llong 	        ATOMIC_LLONG_LOCK_FREE
        atomic_intptr_t 	    ATOMIC_POINTER_LOCK_FREE
    */

    // all of these macros can have 3 different values, here's the mapping

    /*

    Value 	Meaning
    0 	    Never lock-free.
    1 	    Sometimes lock-free.
    2 	    Always lock-free.

    */

    // you can always test atomicity during the runtime using
    // atomic_is_lock_free(), which return true or false if the type
    // is atomic right now

    // Atomic flags

    // there's only one type which guarantee lock free atomic:

    {
        atomic_flag f = ATOMIC_FLAG_INIT;

        // set and return previous status as a _Boot with:
        // atomic_flag_test_and_set()
        
        // clear the flag atomically using:
        // atomic_flag_clear()
    }

    {
        atomic_flag f= ATOMIC_FLAG_INIT;

        bool r = atomic_flag_test_and_set(&f);
        printf("Value was: %d\n", r);

        r = atomic_flag_test_and_set(&f);
        printf("Value was: %d\n", r);

        atomic_flag_clear(&f);
        r = atomic_flag_test_and_set(&f);
        printf("Value was: %d\n",r);

    }

    // Atomic structs and unions

    // if there is not much data in it it might actually be lock free

    {
        struct point {
            float x,y;
        };

        _Atomic(struct point) p;

        printf("Is lock free: %d\n", atomic_is_lock_free(&p));

        // the catch is that you can't access fields of an atomic struct or union

        // whats the point? you can copy entire struct into a non-atomic variable
        // and use it, you can atomically copy the other way too.

        struct point t;

        p = (struct point) {1,2};

        t = p; // atomic copy

        printf("%f\n", t.x);

    }

    
}
