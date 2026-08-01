#include <stdlib.h>
#include <stdio.h>
#include <locale.h>

int main(void) {
    setlocale(LC_ALL, "");  // Use this environment's locale for everything
    setlocale(LC_ALL, "C");  // Use the default C locale

    // how to get monetary locale settings

    struct lconv *x = localeconv();

    // pointer to a struct that holds that information

    /*
    _p_:    positive
    _n_:    negative
    _int_:  international


    Field 	                    Description
    
    char    *mon_decimal_point 	Decimal pointer character for money, e.g. ".".
    char    *mon_thousands_sep 	Thousands separator character for money, e.g. ",".
    char    *mon_grouping 	    Grouping description for money (see below).
    char    *positive_sign 	    Positive sign for money, e.g. "+" or "".
    char    *negative_sign 	    Negative sign for money, e.g. "-".
    char    *currency_symbol 	Currency symbol, e.g. "$".
    char    frac_digits 	    When printing monetary amounts, how many digits to print past the decimal point, e.g. 2.
    char    p_cs_precedes 	    1 if the currency_symbol comes before the value for a non-negative monetary amount, 0 if after.
    char    n_cs_precedes 	    1 if the currency_symbol comes before the value for a negative monetary amount, 0 if after.
    char    p_sep_by_space 	    Determines the separation of the currency symbol from the value for non-negative amounts (see below).
    char    n_sep_by_space 	    Determines the separation of the currency symbol from the value for negative amounts (see below).
    char    p_sign_posn 	    Determines the positive_sign position for non-negative values.
    char    n_sign_posn 	    Determines the positive_sign position for negative values.
    char    *int_curr_symbol 	International currency symbol, e.g. "USD ".
    char    int_frac_digits 	International value for frac_digits.
    char    int_p_cs_precedes 	International value for p_cs_precedes.
    char    int_n_cs_precedes 	International value for n_cs_precedes.
    char    int_p_sep_by_space 	International value for p_sep_by_space.
    char    int_n_sep_by_space 	International value for n_sep_by_space.
    char    int_p_sign_posn 	International value for p_sign_posn.
    char    int_n_sign_posn 	International value for n_sign_posn.
    */
    return 0;
}