#include <stdlib.h>
#include <stdio.h>

void do_main_things(void) {
    printf("Executing main program...\n");
}

int factorial(int n, int a) {
    // using tco optimization
tco:
    if (n == 0)
        return a;

    // replace return by setting new parameter values and
    // goto-ing the beginning of the function

    //return factorial(n - 1, a * n);

    int next_n = n - 1;  // See how these match up with
    int next_a = a * n;  // the recursive arguments, above?

    n = next_n; // set params to the new values
    a = next_a;

    goto tco; // recursive call
}
int main(void) {   
    

    // simple example of goto
    {
        printf("One\n");
        printf("Two\n");

        goto skip_3;

        printf("Three\n");

        skip_3:

            printf("Five\n");
    }

    // {
    //     infinite_loop:
    //         printf("Hello, world!\n");
    //         goto infinite_loop;
    // }

    {
        // will be executed just as if labels are not there
        printf("Zero\n");
        label_1:
        label_2:
            printf("One\n");
        label_3:
            printf("Two\n");
        label_4:
            printf("Three\n");

        // labels have a function scope
    }

    {
        // labeled continue

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                printf("%d, %d\n", i, j);
                continue;   // Always goes to next j
            }
        }

        // break out of the inner loop
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                printf("%d, %d\n", i, j);
                break;     // Gets us to the next iteration of i
            }
        }   

        // but what if we wanna get out of both loops in the same time
        // familiar problem isn't it ??


        // goto solves it:

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                for (int k = 0; k < 3; k++) {
                    printf("%d, %d, %d\n", i, j, k);

                    goto continue_i;   // Now continuing the i loop!!
                }
            }
        continue_i:; // use ; because goto can't point to a plain end
        }

        // this solution of nested loops breaking is way cleaner
        // than flags or if checks
    }

    {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                printf("%d, %d\n", i, j);
                goto break_i;   // Now breaking out of the i loop!
            }
        }

        break_i:

            printf("Done!\n");  
    }

    {
    if (1 == -1)
        goto shutdown;

    if (1 == -1)
        goto shutdown_1;

    if (1 == -1)
        goto shutdown_2;

    if (1 == -1)
        goto shutdown_3;

    do_main_things();   // Run our program

    //shutdown_system4();

    shutdown_3:
        //shutdown_system3();

    shutdown_2:
        // shutdown_system2();

    shutdown_1:
        // shutdown_system1();

    shutdown:
        printf("All subsystems shut down.\n");
    }

    {
        for (int i = 0; i <= 8; i++) {
            printf("%d! == %ld\n", i, factorial(i, 1));
        }
    }


    // this will compile with a warning

    {
        goto label1;

    {
        int x = 12345;

        label1:
            printf("%d\n", x); // variable uninitialized if jump here
    }
    }


    // what happens here though?
    // will print some random garbage
    // but on my system prints 10, but that's just luck
    {
        {
        int x = 10;

    label2:
            printf("%d\n", x);
        }

    goto label2;
    }

    // we have some special rule with VLAs

    // we cannot jump from outside of the vla scope inside of it
    // and skip the declaration for example
    {
        int x = 10;

    //goto label;     // this gives an error
    //goto.c:188:5: error: jump into scope of identifier with variably modified type


    {
        int v[x];

label4:

        printf("Hi!\n");
    }
    }

    {
        // u can jump into vla scope if it declares properly inside the label

        // like this:

         int x = 10;

        goto label;

        {
    label:  ;
            int v[x];

            printf("Hi!\n");
        }
    }

    return 0;
}