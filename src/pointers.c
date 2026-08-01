#include <stdlib.h>
#include <stdio.h>


void *my_memcpy(void *dest, const void *src, size_t n) {
    // Make local variables for src and dest, but of type unsigned char

    const unsigned char *s = src;
    unsigned char *d = dest;

    while (n-- > 0) { // for n bytes
        *d++ = *s++;  // copy source byte to dest byte;
    }

    // now just return pointer to dest;

    return dest;
}

struct foo {
    char a;
    int b;
};

void print_int(int n) {
    printf("%d\n", n);
}

int add (int a, int b) {
    return a + b;
}

int mult (int a, int b) {
    return a * b;
}

void print_math(int (*op)(int,int), int x, int y) {
    int result = op(x, y);

    printf("%d\n", result);
}
int main(void) {


    // normal pointer
    {
        int x = 3490;  // Type: int
        int *p = &x;   // Type: pointer to an int

        printf("%d\n", *p);  // 3490
    }

    // pointer to pointer example

    {
        int x = 3490;  // Type: int
        int *p = &x;   // Type: pointer to an int
        int **q = &p;  // Type: pointer to pointer to int

        // double deref:
        printf("%d %d\n", *p, **q);  // 3490 3490   
    }

    // nested pointers
    {
        int x = 3490;     // Type: int
        int *p = &x;      // Type: int *, pointer to an int
        int **q = &p;     // Type: int **, pointer to pointer to int
        int ***r = &q;    // Type: int ***, pointer to pointer to pointer to int
        int ****s = &r;   // Type: int ****, you get the idea
        int *****t = &s;  // Type: int *****
    }

    // const pointer

    {
        int *const p;
    }

    {
        int ***const p;

        // p++;  // Not allowed
    }

    {
        int x = 3490;
        int *const p = &x; 
        int **q = &p; // WARNING: initialization discards ‘const’ qualifier from pointer target type

        // the pointer type of p is int *const so the type of q has to be int *const * if you want
        // to keep the const qualifier
    }

    // FIX

    {
        int x = 3490;
        int *const p = &x;
        int *const *q = &p;
    }

    // what if we wanna make q a const as well?

    {
        int x = 3490;
        int *const p = &x;
        int *const *const q = &p;  // More const!
    }

    // multibyte values
    // below will be shown how to iterate through types that take up more than one byte of space
    // check out my_memcpy in the top of the file

    struct foo x = {0x12, 0x12345678};

    unsigned char *p = (unsigned char *)&x;
    // we cast pointer to x with unsigned char

    // we print out all the bytes of the struct to see the padding bytes
    for (size_t i = 0; i < sizeof x; i++) {
        printf("%02X\n", p[i]);
    }

    // we see the exact values that were stored in struct
    // with additional padding bytes but the values stored
    // from the variables are in reverse order why?

    /*
    This will vary depending on the architecture, 
    but my system is little endian, which means the least-significant byte of the number is stored first. 
    Big endian systems will have the 12 first and the 78 last. 
    But the spec doesn’t dictate anything about this representation.↩
    */

    // NULL pointer

    // values that can be used interchangeably:

    /*
    NULL
    0       (null pointer constant)
    '\0'    (byte with all values set to zero)
    (void *)0
    */


    // important rule

    // not good, c has strict aliasing:
    {
        int a = 1;
        float *p = (float *)&a;
    }

    // another way to store pointer differences to count the length of some object

    {
        int cats[100];

        int *f = cats + 20;
        int *g = cats + 60;

        ptrdiff_t d = g - f; // differnces is 40

        // for printing use prefix 't'

        printf("%td\n", d);
        printf("%tX\n", d); // for hex
    }

    // function pointers

    {
        // pointer p to a function
        // the function returns float and
        // takes to ints as arguments
        float (*p)(int, int); 

        // you don't have to give parameters names but
        // you can if you want

        float (*x)(int a, int b);
    }

    // example
    {
        void (*p)(int) = print_int;

        // we create function alias using a pointer
        p(3490);
    }

    // another example where
    // we have a function that takes pointer to a function
    // as a parameter so a callback

    print_math(add, 5, 7);
    print_math(mult, 5, 7);

    return 0;
}