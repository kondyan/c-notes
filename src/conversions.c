#include <stdlib.h>
#include <stdio.h>
int main() {

    char s[10];
    float f = 3.14159;

    // convert "f" to string storing with at most 10 bytes including NUL terminator
    snprintf(s, 10, "%f", f);

    printf("String value: %s\n", s);

    printf("%d %d\n", 5, '5');
    return 0;
}