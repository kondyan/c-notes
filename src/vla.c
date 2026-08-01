#include <stdlib.h>
#include <stdio.h>

int main(void) {
    
    /*
    vla is an array whose size is determined at runtime.
    this provides dynamic sizing during runtime like malloc()
    but without a need to free() up the memory after
    */
    #if __STDC_NO_VLA__ == 1
        #error Sorry, need VLAs for this program!
    #endif
    {
        int v[10]; // normal array

   
    }   
    {
        int n = 10;
        int v[n]; // vla
    }
    {
    int n;
    char buf[32];
    printf("Enter a number: ");
    fflush(stdout);
    fgets(buf, sizeof buf, stdin);
    n = strtoul(buf, NULL, 10);

    int v[n];

    for (int i = 0; i < n; i++) {
        v[i] = i * 10;
    }

    for (int i = 0; i < n; i++) {
        printf("v[%d] = %d\n", i, v[i]);
    }

    size_t num_elems = sizeof v / sizeof v[0];

    printf("The array has: %zu elements\n", num_elems);
    }

    /*
    VLA Rules:

    You can’t declare a VLA at file scope, and you can’t make a static one in block scope.
    You can’t use an initializer list to initialize the array.
    */
    
    int x = 12;

    printf("%zu\n", sizeof(int [x]));  // Prints 48 on my system

    {
        int w = 10;
        int h = 20;

        int x[h][w];
        int y[5][w];
        int z[10][w][20];
    }

    {
        // vla prototypes
        void do_something(int count, int v[count]);  // With names
        void do_something(int, int v[*]);            // Without names
    }
    return 0;
}