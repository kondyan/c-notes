#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
int main(void) {

    char *s = "Hello!";
    char t = 'c';

    // what about special characters like €

    // or this won't work too, how do we go about it ?
    // char t = ''';

    // we can do this trick with backslash for the single quote

    // You can say either “backslash” or “escape” in this context (“escape that quote”)
    
    char t = '\'';

    {

        for (int i = 10; i >= 0; i--) {
        printf("\rT minus %d second%s... \b", i, i != 1? "s": "");

        fflush(stdout);  // Force output to update

        // Sleep for 1 second
        //thrd_sleep(&(struct timespec){.tv_sec=1}, NULL);
    }

    }

    return 0;
}