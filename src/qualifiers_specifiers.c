#include <stdlib.h>
#include <stdio.h>

void counter(void)
{
    static int count = 1;  // This is initialized one time

    printf("This has been called %d time(s)\n", count);

    count++;
}

int main(void) {
    {
        const int x = 2;
        // x = 4; // COMPILER ERROR cant assign to a constant

        printf("%d\n", x + 30);  // OK, doesn't modify "x"

    }

    {
        int x[] = {10, 20};
        const int *p = x;

        p++; // can modify the pointer itself no problem
        //*p = 30; cant modify the thing it points to
    }

    {
        // const int *p; // can't modify what p points to
        // int const *p; // equivalent to the line above
    }
    {
        int *const p; // can't modify the pointer, but can modify what it points to
        // p++; // compiler error

    }
    {
        int x = 10;
        int *const p = &x;

        *p = 20; // set x to 20, all legal

    }
    {
        const int *const p;  // Can't modify p or *p!
        
    }
    {
        char **p;
        p++;    //ok
        (*p)++; //ok
        printf("%d\n", *p);

    }
    {
        //char **const p;
        //p++;    //error
        //(*p)++;   // ok
              //  printf("%d\n", *p);

    }
    {
        //char *const *p;
        //p++;      //ok
        //(*p)++; // error
         //       printf("%d\n", *p);

    }
    {
        //char *const *const p;
        //p++;      //error
        //(*p)++;   //error
        //        printf("%d\n", *p);

    }

    {
        //const int x = 20;
        // int *p = &x;

        // *p = 40;
        // this is undefined behaviour
        // we assigned a const type variable address
        // to a pointer without const qualifier

        // printf("%d\n", x);  // 40, if you're lucky

    }

    {
        counter();
        counter();
        counter();
        counter();
    }
    {
        extern int a;
        printf("%d\n", a);
    }
    {
        // already under the hood so redundant
        register int a;   // Make "a" as fast to use as possible.

        for (a = 0; a < 10; a++)
            printf("%d\n", a);
        
        // though this wouldnt work now
        //int *p = &a;    // COMPILER ERROR! Can't take address of a register

        //dereferencing doesn't work too
        register int a[] = {11, 22, 33, 44, 55};

        // int a = *(a + 2);  // COMPILER ERROR! Address of a[0] taken

        // Interestingly, for the equivalent with array notation, gcc only warns:


        // int a = a[2];  // COMPILER WARNING!
    }
    
    return 0;
}