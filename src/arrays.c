#include <stdlib.h>
#include <stdio.h>


// these are equivalent
int func(int *p) {}
int func2(int p[]){};

int main(void) {

    // we have some qualifiers
    int *const p;
    int *volatile p2;
    int *const volatile p3;

    // how do we specify those in function params?
    {
        int func(int *const volatile p);
    }
    {
        int func(int p[const volatile]);
    }
    {
        int func(int p[const volatile 10]);
    }

    // static in array param means
    // that the array will be at least of length 4 in this case
    {
        int func(int p[static 4]);
    }
    // if we pass an array with length less than 4
    // it will be an undefined behaviour

    {
        // set array with zeros
        int a[5] = {0};
    }

    {
        // multidimensional
         int a[3][2] = {
        {1, 2},
        {3},    // Left off the 4! (0 instead)
        {5, 6}
    };
    }
    {
          int a[3][2] = {
        {1, 2},
        // {3, 4},   // Just cut this whole thing out
        {5, 6}
    };
    // now we have 2 filled in rows so the third one will be
    // {0, 0}
    }

    // you can also do 1d initializer for 2d arrays like this

    {
            int a[3][2] = { 1, 2, 3, 4, 5, 6 };
    }

    // or if it has less elements than array it will fill the rest with zeros

    {
        int a[3][2] = { 1, 2, 3 };
    }

    // or if you wanna fill everything with zeros just do
    {
        int a[3][2] = {0};
    }

    return 0;
}