#include <stdlib.h>
#include <stdio.h>
#include <complex.h>

#ifdef __STDC_NO_COMPLEX__
#error Comple numbers not supported!
#endif

int main(void) {

    {
        float complex c; // spec prefers this way of defining
        complex float c; // same thing order doesn't matter

        double complex x = 5 + 2 * I;
        double complex y = 10 + 3 * I;
    }

    // you can also build it with macro like this:
    {
        double complex x = CMPLX(5, 2); // this is only for double type
    }
    // it's equivalent to this
    {
        double complex x = 5 + 2*I;
    }

    // macro for floats:        CMPLXF()
    // macro for long double:   CMPLXL()


    {
        // extract different parts from complex numbers

        double complex x = 5 + 2 * I;
        double complex y = 10 + 3 * I;

        printf("x = %f + %fi\n", creal(x), cimag(x));
        printf("y = %f + %fi\n", creal(y), cimag(y));
    }

    // list of complex math functions to use (type double)

    {
        /*
        Trigonometry Functions

        Function 	Description
        ccos() 	    Cosine
        csin() 	    Sine
        ctan() 	    Tangent
        cacos() 	Arc cosine
        casin() 	Arc sine
        catan() 	Play Settlers of Catan
        ccosh() 	Hyperbolic cosine
        csinh() 	Hyperbolic sine
        ctanh() 	Hyperbolic tangent
        cacosh() 	Arc hyperbolic cosine
        casinh() 	Arc hyperbolic sine
        catanh() 	Arc hyperbolic tangent
        */

        /*
        Exponential and Logarithmic Functions

        Function 	Description
        cexp() 	    Base- exponential
        clog() 	    Natural (base-) logarithm
        */

        /*
        Power and Absolute Value Functions

        Function 	Description
        cabs() 	    Absolute value
        cpow() 	    Power
        csqrt() 	Square root
        */

        /*
        Manipulation Functions

        Function 	Description
        creal() 	Return real part
        cimag() 	Return imaginary part
        CMPLX() 	Construct a complex number
        carg() 	    Argument/phase angle
        conj() 	    Conjugate
        cproj() 	Projection on Riemann sphere
        */
    }
    return 0;
}