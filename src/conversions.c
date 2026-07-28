#include <stdlib.h>
#include <stdio.h>
int main(void) {

    {
    char s[10];
    float f = 3.14159;

    // convert "f" to string storing with at most 10 bytes including NUL terminator
    snprintf(s, 10, "%f", f);

    printf("String value: %s\n", s);

    }
    
    /*
    convert string to numeric value
    ASCII to ....

    Function 	Description
        atoi 	String to int
        atof 	String to float
        atol 	String to long int
        atoll 	String to long long int
    */
{
    char *pi = "3.14159";
    float f = atof(pi);

    printf("%f\n", f);
}
    

    /*
    for better error handling use
    Function 	Description
    strtol 	    String to long int
    strtoll 	String to long long int
    strtoul 	String to unsigned long int
    strtoull 	String to unsigned long long int
    strtof 	    String to float
    strtod 	    String to double
    strtold 	String to long double
    */

    {
        char *s = "3490";

        // null means we don't care about error information
        // we use base 10
        unsigned long int x = strtoul(s, NULL, 10);

        printf("%lu\n", x);
    }
    {
        char *s = "101010";

        unsigned long int x = strtoul(s, NULL, 2);
        printf("%lu\n", x);

    }

    {
        char *s = "34x90"; // not base 10 digit
        char *badchar;

        // badchar is the first char that couldn't be converted to int
        unsigned long int x = strtoul(s, &badchar, 10);


        printf("%lu\n", x); // 34

        

        if (*badchar == '\0') {
            printf("Success! %lu\n", x);
        } else {
            printf("Partial conversion: %lu\n", x);
            printf("Invalid character: %c\n", *badchar);
        }
    }

    {
       printf("%d %d\n", 5, '5');

       char c = '6';

       int i = c - '0';

       printf("%d\n", i);
    }

    return 0;
}