#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#define TYPESTR(x) _Generic((x), \
                        int: "int", \
                        long: "long", \
                        float: "float", \
                        double: "double", \
                        default: "something else")

#define FMTSPEC(x) _Generic((x), \
                        int: "%d", \
                        long: "%ld", \
                        float: "%f", \
                        double: "%f", \
                        char *: "%s")

#define PRINT_VAL(x)   \
    do{                  \
        char fmt[512]; \
        snprintf(fmt, sizeof fmt, #x " = %s\n", FMTSPEC(x)); \
        printf(fmt, (x)); \
} while(0)
    

int sum(int p[], int count ){
    int total = 0;

    for (int i = 0; i < count; i ++) {
        total += p[i];
    }

    return total;
}




int *get3490(void)
{
    // Don't do this
    return &(int){3490};
}


struct coord {
    int x, y;
};

void print_coord(struct coord *c) {
    printf("%d, %d\n", c->x, c->y);
}
int main(void) {
    
    /*
    Here will be some notes on these 2 topics:

        - How to have “anonymous” unnamed objects and how that’s useful.
        - How to generate type-dependent code.

    */

    // compound literals (objects without assignment)

    {
        // put type in the () brackets
        (int[]){1, 2, 3, 4};//compound literal right here

    }

    {
        // we can do this if we wanna call function
        int s = sum((int[]){1, 2, 3, 4}, 4);
        printf("%d\n", s);
    }

    {
        //print_coord((struct coord){.x = 10, .y = 20});

        // or if we don't wanna copy just do

        print_coord(&(struct coord){.x = 10, .y = 20});

    }

    {
        // caution: lifetime of unnamed object ends at the end of it's scope

        int *p;

        {
            p = &(int){10};
        }

        printf("%d\n", *p);  // INVALID: The (int){10} fell out of scope
    }

    printf("%d\n", *get3490());  // INVALID: (int){3490} fell out of scope

    {
        int x = 3490;

        printf("%d\n", x);               // 3490 (variable)
        printf("%d\n", 3490);            // 3490 (constant)
        printf("%d\n", (int){3490});     // 3490 (unnamed object)
    }

    // generic selections

    {
        // it's very simillar to a switch case,
        // we can change the value stored in the variable
        // based on the type of some other variable
        int i;
        float f;
        char c;

        char *s = _Generic(i, int: "that variable is an int", float: "that variable is a float", default: "that variable is something else");

        printf("%s\n", s);
    }

    {
        int i;
        long l;
        float f;
        double d;
        char c;

        printf("i is type %s\n", TYPESTR(i));
        printf("l is type %s\n", TYPESTR(l));
        printf("f is type %s\n", TYPESTR(f));
        printf("d is type %s\n", TYPESTR(d));
        printf("c is type %s\n", TYPESTR(c));

        PRINT_VAL(i);
    }

    // incomplete types

    {
        // this is valid code

        struct foo *x;
        union bar *y;
        enum baz *z;

        // these are incomplete types
    }

    {
        struct antelope {              // struct antelope is incomplete here
            int leg_count;             // Still incomplete
            float stomach_fullness;    // Still incomplete
            float top_speed;           // Still incomplete
            char *nickname;            // Still incomplete
        };    
    }

    // one specialty about incomplete types is that we can declare a pointer to one like when building a linked list for example

    {
        struct node {
            int val;
            struct node *next;  // struct node is incomplete, but that's OK!
        };
    }

    {
        // this wouldn't be possible without this pointer rule
        // about incomplete types
        // you cannot dereference a pointer to an incomplete type though

        struct a {
            struct b *x;  // Refers to a `struct b`
        };

        struct b {
            struct a *x;  // Refers to a `struct a`
        };
    }
    {
        // important to note that this is
        extern int my_array[];  // Incomplete type
        // because array length is not defined
    }

    // you can also complete incomplete types like this
    {

        struct foo;        // incomplete type

        struct foo *p;     // pointer, no problem

        // struct foo f;   // Error: incomplete type!

        struct foo {
            int x, y, z;
        };                 // Now the struct foo is complete!

        struct foo f;      // Success!
    }
    
    // bit sized types

    /*
    We can specify the exact number of bits for an integer type

    There are several options to do so:

    Integers of exactly a certain size (intN_t)
    Integers that are at least a certain size (int_leastN_t)
    Integers that are at least a certain size and are as fast as possible (int_fastN_t)189
    */

    {
        int32_t w;          // w is exactly 32 bits, signed
        uint16_t x;         // x is exactly 16 bits, unsigned

        int_least8_t y;     // y is at least 8 bits, signed

        uint_fast64_t z;    // z is the fastest representation at least 64 bits, unsigned

    }

    {
        // the list of the types that are defined guaranteed
        /*
        int_least8_t      uint_least8_t
        int_least16_t     uint_least16_t
        int_least32_t     uint_least32_t
        int_least64_t     uint_least64_t

        int_fast8_t       uint_fast8_t
        int_fast16_t      uint_fast16_t
        int_fast32_t      uint_fast32_t
        int_fast64_t      uint_fast64_t
        */

        // these are optional depending on the system
        /*
        int8_t      uint8_t
        int16_t     uint16_t
        int32_t     uint32_t
        int64_t     uint64_t

        Namely, the system has 8, 16, 32, or 64 bit 
        integers with no padding that use two’s complement representation, 
        in which case the intN_t variant for that 
        particular number of bits must be defined.
        */

        // use this if you wanna go as big as possible
        // (max int representation possible)
        /*
        intmax_t
        uintmax_t
        */

        // use these macros to append proper suffix to the number
        // if you wanna fix the number of bits like "22L" or "3490ULL"

        /*
        INT8_C(x)     UINT8_C(x)
        INT16_C(x)    UINT16_C(x)
        INT32_C(x)    UINT32_C(x)
        INT64_C(x)    UINT64_C(x)
        INTMAX_C(x)   UINTMAX_C(x)
        */

        uint16_t x = UINT16_C(12);
        intmax_t y = INTMAX_C(3490);

        // limits defined for min and max values of each type
        /*
        INT8_MAX           INT8_MIN           UINT8_MAX
        INT16_MAX          INT16_MIN          UINT16_MAX
        INT32_MAX          INT32_MIN          UINT32_MAX
        INT64_MAX          INT64_MIN          UINT64_MAX

        INT_LEAST8_MAX     INT_LEAST8_MIN     UINT_LEAST8_MAX
        INT_LEAST16_MAX    INT_LEAST16_MIN    UINT_LEAST16_MAX
        INT_LEAST32_MAX    INT_LEAST32_MIN    UINT_LEAST32_MAX
        INT_LEAST64_MAX    INT_LEAST64_MIN    UINT_LEAST64_MAX

        INT_FAST8_MAX      INT_FAST8_MIN      UINT_FAST8_MAX
        INT_FAST16_MAX     INT_FAST16_MIN     UINT_FAST16_MAX
        INT_FAST32_MAX     INT_FAST32_MIN     UINT_FAST32_MAX
        INT_FAST64_MAX     INT_FAST64_MIN     UINT_FAST64_MAX

        INTMAX_MAX         INTMAX_MIN         UINTMAX_MAX
        */

        // limit for all unsigned types is obviously 0
        // so there's no macro for them

        // format specifiers

        /*
        PRIdn    PRIdLEASTn    PRIdFASTn    PRIdMAX
        PRIin    PRIiLEASTn    PRIiFASTn    PRIiMAX
        */

        int_least16_t x1 = 3490;

        // these macros are string literals so we can do this

        printf("The value is %" PRIdLEAST16 "!\n", x);

        // there are also these macros for unsigned types
        /*
        PRIon    PRIoLEASTn    PRIoFASTn    PRIoMAX
        PRIun    PRIuLEASTn    PRIuFASTn    PRIuMAX
        PRIxn    PRIxLEASTn    PRIxFASTn    PRIxMAX
        PRIXn    PRIXLEASTn    PRIXFASTn    PRIXMAX

        the lowercase n should be substituted with 8, 16, 32, or 64.
        */

        // this is the exact same thing but for scanf()
        /*
        SCNdn    SCNdLEASTn    SCNdFASTn    SCNdMAX
        SCNin    SCNiLEASTn    SCNiFASTn    SCNiMAX
        SCNon    SCNoLEASTn    SCNoFASTn    SCNoMAX
        SCNun    SCNuLEASTn    SCNuFASTn    SCNuMAX
        SCNxn    SCNxLEASTn    SCNxFASTn    SCNxMAX
        */


    }

    

    return 0;
}