#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

int my_printf(int serial, const char *format, ...) {
    va_list va;
    int rv;

    printf("The serial number is: %d\n", serial);

    va_start(va, format);
    rv = vprintf(format, va);
    va_end(va);

    return rv;
}

int add (int x, int y) {
    return x + y;
}

int add_variadic(int count, ...) {
    int total = 0;
    va_list va;

    va_start(va, count); // start with args after count

    for (int i = 0; i < count; i++) {
        int n = va_arg(va, int); // get the next int

        total += n;
    }

    va_end(va); // done

    return total;
}

void func(int a, ...) {
    printf("a is %d\n", a);
}


int main(void) {
    
    // variadic functions
    // describes functions that take arbitrary
    // number of arguments

    {
        // you can do
        add(2, 3);
        add(5, 3);

        // but you cant do

        // add(2, 3, 4);
        // add(5);
    }

    // we've seen an example of a variadic function already
    
    printf("Hello, world!\n");
    printf("The number is %d\n", 2);
    printf("The number is %d and pi is %f\n", 2, 3.14159);

    // we cannot do this though
    // printf();

    // so variadic functions MUST have at least
    // one argument

    {
        //elliples in function signatures

        func(2, 3, 4, 5, 6); // see the function above
        // it take one mandatory argument and variadic
        // number of arguments afterwards
    }
    // but how do we get all the extra arguments

    // #include <stdarg.h>
    // checkout out add_variadic() in the top

    printf("%d\n", add_variadic(4, 6, 2, -4, 17));
    printf("%d\n", add_variadic(2, 22, 44));

    // printf uses the number of %d or other formats
    // to know how many arguments are being passed

    // checkout my_printf

    int x = 10;

    float y = 3.2;
    my_printf(3490, "x is %d, y is %f\n", x, y);
    return 0;
}