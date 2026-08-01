#include <stdlib.h>
#include <stdio.h>

struct foo1 {
    unsigned char a:2;
    unsigned char :5;   // <-- unnamed bit-field!
    unsigned char b:1;

    /*
    And that works—in our code we use a and b, but never dummy. 
    It’s just there to eat up 5 bits to make sure a and b are
    in the “required” (by this contrived example) positions within the byte.
    */
};


/* 
These are basically just like structs, except the fields overlap in memory. The union will be only large enough for the largest field, and you can only use one field at a time.

It’s a way to reuse the same memory space for different types of data.

You declare them just like structs, except it’s union. Take a look at this:
*/
union foo5 {
    int a, b, c, d, e, f;
    float g, h;
    char i, j, k, l;
};

/*
You can non-portably write to one union field and read from another!

Doing so is called type punning139, and you’d use it if you really knew what you were doing, typically with some kind of low-level programming.

Since the members of a union share the same memory, writing to one member necessarily 
affects the others. And if you read from one what was written to another, you get some weird effects.
*/
union foo4 {
    float b;
    short a;
};

union foo3 {
    int a, b, c, d, e, f;
    float g, h;
    char i, j, k, l;
};

struct foo2 {
    unsigned int a:1;
    unsigned int b:2;
    unsigned int :0;   // <--Zero-width unnamed bit-field to separate memory packaging
    unsigned int c:3;  // compiler would pack c and d into a separate unsigned int with this bit field
    unsigned int d:4;
};

struct foo {
    // specify the number of bits each value can hold from 1 up to bit size of the format
    // to optimize memory, in this case it reduces the sizeof(struct foo) from 16 to 4 bits
    unsigned int a:5, b:5, c:3, d:3;
};

struct parent {
    int a, b;
};

struct child {
    struct parent super;
    int c, d;
};

void print_parent(void *p) {

    // destructuring
    struct parent *self = p;

    printf("Parent: %d, %d\n", self->a, self->b);
}

void print_child(struct child *self) {
    printf("Child: %d, %d\n", self->c, self->d);
}

int main(void) {
    struct child c = {.super.a = 1, .super.b = 2, .c = 3, .d = 4};

    print_child(&c);

    // pointer to a struct has the same value as the pointer to the first field in the struct
    print_parent(&c); // works with child too because the first variable point to parent

    printf("%zu\n", sizeof(struct foo));

    union foo4 x;

    x.b = 3.14159;

    printf("%f\n", x.b);

    printf("%f\n", x.a);

    /*
    In this example, we see that the union has ints and floats in it. 
    And we get pointers to the union, but we cast them to int* and float* types (the cast silences compiler warnings). 
    And then if we dereference those, we see that they have the values we stored directly in the union.
    */
    {   
        union foo5 x;
        int *foo_int_p = (int *)&x;
        float *foo_float_p = (float *)&x;

        x.a = 12;
        printf("%d\n", x.a);
        printf("%d\n", *foo_int_p);

        x.g = 3.141592;
        printf("%f\n", x.g);
        printf("%f\n", *foo_float_p);

    }

    // it works the other way around too
    {
        union foo5 x;
        int *foo_int_p = (int *)&x;             // Pointer to int field
        union foo5 *p = (union foo5 *)foo_int_p;  // Back to pointer to union

        p->a = 12;  // This line the same as...
        x.a = 12;   // this one.
    }

    /*
    If you have a union of structs, and all those structs begin with a common initial sequence, 
    it’s valid to access members of that sequence from any of the union members.

    What?
    */

    {
    struct a {
        int x;     //
        float y;   // Common initial sequence

        char *p;
    };

    struct b {
        int x;     //
        float y;   // Common initial sequence

        double *p;
        short z;
    };

    union foo {
        struct a sa;
        struct b sb;
    };

    /*
    What this rule tells us is that we’re guaranteed that the members 
    of the common initial sequences are interchangeable in code. That is:

    f.sa.x is the same as f.sb.x.

        and

    f.sa.y is the same as f.sb.y.

    */
    }
    
    return 0;
}