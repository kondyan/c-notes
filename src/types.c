#include <stdlib.h>
#include <stdio.h>

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
    

    return 0;
}