#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <wctype.h>
/*
following topics will be covered:

Unicode background
Character encoding background
Source and Execution character Sets
Using Unicode and UTF-8
Using other character types like wchar_t, char16_t, and char32_t

*/
int main(void){
    
    /*Encodings for Unicode
    Encoding 	Description
    UTF-8 	    A byte-oriented encoding that uses a variable number of bytes per character. This is the one to use.
    UTF-16 	    A 16-bit per character160 encoding.
    UTF-32 	    A 32-bit per character encoding.
    */

    /*
    Standard character set for both source and execution:

    A B C D E F G H I J K L M
    N O P Q R S T U V W X Y Z
    a b c d e f g h i j k l m
    n o p q r s t u v w x y z
    0 1 2 3 4 5 6 7 8 9
    ! " # % & ' ( ) * + , - . / :
    ; < = > ? [ \ ] ^ _ { | } ~
    space tab vertical-tab
    form-feed end-of-line


    Those are the characters you can use in your source and remain 100% portable.
    */

    // how to use unicode in C

    {
    char *s = "\u20AC1.23";

    printf("%s\n", s);  // €1.23

    char *x = "\U0001D4D1";

    printf("%s\n", x);  // Prints a mathematical letter "B"

    }
    // multibyte characters
{
    char c[128] = "Hello, world!"; // Multibyte string

    /*
        What we’re saying here is that a particular character that’s not in the 
        basic character set could be composed of multiple bytes. 
        Up to MB_LEN_MAX of them (from <limits.h>). 
        Sure, it only looks like one character on the screen, but it could be multiple bytes.
    */

    char *s = "\u20AC1.23";  // €1.23

    // strlen returns the number of bytes not the number of characters so this
    printf("%zu\n", strlen(s));  // 7!      

    printf("%x\n", '€');
    printf("%x\n", '\u20ac');
}
    // wide chars to print out more bits

    wchar_t *s = L"Hello, world!";
    wchar_t c = L'B';

    printf("%ls %lc\n", s, c);

    /*
    conversions:

    mb: multibyte
    wc: wide character
    mbs: multibyte string
    wcs: wide character string

    Conversion Function 	Description
    mbtowc() 	            Convert a multibyte character to a wide character.
    wctomb() 	            Convert a wide character to a multibyte character.
    mbstowcs() 	            Convert a multibyte string to a wide string.
    wcstombs() 	            Convert a wide string to a multibyte string.
    */

    {
    // Get out of the C locale to one that likely has the euro symbol
    setlocale(LC_ALL, "");

    char *mb_string = "The cost is \u20ac1.23";
    size_t mb_len = strlen(mb_string);

    // wide char array
    wchar_t wc_string[128];


    // convert
    size_t wc_len = mbstowcs(wc_string,mb_string,128);

    // Print result--note the %ls for wide char strings
    printf("multibyte: \"%s\" (%zu bytes)\n", mb_string, mb_len);
    printf("wide char: \"%ls\" (%zu characters)\n", wc_string, wc_len);
    }
    

    {

        /*
        I/O Functions
        

        I/O Function 	Description
        wprintf() 	    Formatted console output.
        wscanf() 	    Formatted console input.
        getwchar() 	    Character-based console input.
        putwchar() 	    Character-based console output.
        fwprintf() 	    Formatted file output.
        fwscanf() 	    Formatted file input.
        fgetwc() 	    Character-based file input.
        fputwc() 	    Character-based file output.
        fgetws() 	    String-based file input.
        fputws() 	    String-based file output.
        swprintf()  	Formatted string output.
        swscanf() 	    Formatted string input.
        vfwprintf() 	Variadic formatted file output.
        vfwscanf() 	    Variadic formatted file input.
        vswprintf() 	Variadic formatted string output.
        vswscanf() 	    Variadic formatted string input.
        vwprintf() 	    Variadic formatted console output.
        vwscanf() 	    Variadic formatted console input.
        ungetwc() 	    Push a wide character back on an output stream.
        fwide() 	    Get or set stream multibyte/wide orientation.
        */

        /*
        Conversion functions

        Conversion Function 	Description
        wcstod() 	            Convert string to double.
        wcstof() 	            Convert string to float.
        wcstold() 	            Convert string to long double.
        wcstol() 	            Convert string to long.
        wcstoll() 	            Convert string to long long.
        wcstoul() 	            Convert string to unsigned long.
        wcstoull() 	            Convert string to unsigned long long.
        */

        /*
            String and Memory copying

            Copying Function 	Description
            wcscpy() 	        Copy string.
            wcsncpy() 	        Copy string, length-limited.
            wmemcpy() 	        Copy memory.
            wmemmove() 	        Copy potentially-overlapping memory.
            wcscat() 	        Concatenate strings.
            wcsncat() 	        Concatenate strings, length-limited.

            String and Memory comparing

            Comparing Function 	Description
            wcscmp() 	        Compare strings lexicographically.
            wcsncmp() 	        Compare strings lexicographically, length-limited.
            wcscoll() 	        Compare strings in dictionary order by locale.
            wmemcmp() 	        Compare memory lexicographically.
            wcsxfrm() 	        Transform strings into versions such that wcscmp() behaves like wcscoll()

            String searching

            Searching Function 	Description
            wcschr() 	        Find a character in a string.
            wcsrchr() 	        Find a character in a string from the back.
            wmemchr() 	        Find a character in memory.
            wcsstr() 	        Find a substring in a string.
            wcspbrk() 	        Find any of a set of characters in a string.
            wcsspn() 	        Find length of substring including any of a set of characters.
            wcscspn() 	        Find length of substring before any of a set of characters.
            wcstok() 	        Find tokens in a string.

            Miscellaneous

            Length/Misc Function 	Description
            wcslen() 	            Return the length of the string.
            wmemset() 	            Set characters in memory.
            wcsftime() 	            Formatted date and time output.

            <wctype.h> for these

            Character Classification functions

            Length/Misc Function 	Description
            iswalnum() 	            True if the character is alphanumeric.
            iswalpha() 	            True if the character is alphabetic.
            iswblank() 	            True if the character is blank (space-ish, but not a newline).
            iswcntrl() 	            True if the character is a control character.
            iswdigit() 	            True if the character is a digit.
            iswgraph() 	            True if the character is printable (except space).
            iswlower() 	            True if the character is lowercase.
            iswprint() 	            True if the character is printable (including space).
            iswpunct() 	            True if the character is punctuation.
            iswspace() 	            True if the character is whitespace.
            iswupper() 	            True if the character is uppercase.
            iswxdigit() 	        True if the character is a hex digit.
            towlower() 	            Convert character to lowercase.
            towupper() 	            Convert character to uppercase.
        */

       
    }

    {
         mbstate_t mbs;

        // Set the state to the initial state
        memset(&mbs, 0, sizeof mbs);
    }

    {
        setlocale(LC_ALL, "");

        char *mb_string = "The cost is \u20ac1.23";
        size_t mb_len = strlen(mb_string);

        // wide char array
        wchar_t wc_string[128];

         mbstate_t mbs;
        memset(&mbs, 0, sizeof mbs);

        const char *invalid = mb_string;

        size_t wc_len = mbsrtowcs(wc_string, &invalid, 128, &mbs);


        if (invalid == NULL) {
            printf("No invalid characters found\n");

            printf("multibyte: \"%s\" (%zu bytes)\n", mb_string, mb_len);
            printf("wide char: \"%ls\" (%zu characters)\n", wc_string, wc_len);
        } else {
            ptrdiff_t offset = invalid - mb_string;
            printf("Invalid character at offset %d\n", offset);
        }
    }

    // UTF-8

    {
        setlocale(LC_ALL, "en_US.UTF-8");  // Non-portable name
        char *s = u8"Hello, world!";

        printf("&s\n", s);
    }

    /*
    Multibyte conversion

    Conversion Function 	Description
    mbrtoc16() 	            Convert a multibyte character to a char16_t character.
    mbrtoc32() 	            Convert a multibyte character to a char32_t character.
    c16rtomb() 	            Convert a char16_t character to a multibyte character.
    c32rtomb() 	            Convert a char32_t character to a multibyte character.
    */

    
    return 0;
}