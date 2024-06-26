/*******************************************
 Author: Adriana Cesario F. Alvim   *
 Daniel Aloise (C++ - object oriented)
 
 ********************************************/

#include "bibrand2.h"

/***************************************************************************
 Portable pseudo-random number generator                                   *
 Machine independent as long as the machine can represent all the integers *
 in the interval [- 2**31 + 1, 2**31 - 1].                         *
 *
 Reference: L. Schrage, "A more Portable Fortran Random Number Generator", *
 ACM Transactions on Mathematical Software, Vol. 2, No. 2,      *
 (June, 1979).                                                  *
 *
 The generator produces a sequence of positive integers, "ix",             *
 by the recursion: ix(i+1) = A * ix(i) mod P, where "P" is Mersenne   *
 prime number (2**31)-1 = 2147483647 and "A" = 7**5 = 16807. Thus all *
 integers "ix" produced will satisfy ( 0 < ix < 2147483647 ).         *
 *
 The generator is full cycle, every integer from 1 to (2**31)-2 =          *
 2147483646 is generated exactly once in the cycle.                   *
 *
 Input: integer "ix", ( 0 < ix < 2147483647 )                              *
 Ouput: real "xrand", ( 0 < xrand < 1 )                                    *
 ***************************************************************************/
float Bibrand::randp() {
	int xhi, xalo, leftlo, fhi, k;

	const int A = 16807; /* = 7**5		       */
	const int P = 2147483647; /* = Mersenne prime (2**31)-1  */
	const int b15 = 32768; /* = 2**15	               */
	const int b16 = 65536; /* = 2**16	               */

	/* get 15 hi order bits of ix */
	xhi = seed / b16;

	/* get 16 lo bits of ix and form lo product */
	xalo = (seed - xhi * b16) * A;

	/* get 15 hi order bits of lo product	*/
	leftlo = xalo / b16;

	/* from the 31 highest bits of full product */
	fhi = xhi * A + leftlo;

	/* get overflo past 31st bit of full product */
	k = fhi / b15;

	/* assemble all the parts and presubtract P */
	/* the parentheses are essential            */
	seed = (((xalo - leftlo * b16) - P) + (fhi - k * b15) * b16) + k;

	/* add P back in if necessary  */
	if (seed < 0)
		seed = seed + P;

	/* multiply by 1/(2**31-1) */
	return (float) (seed * 4.656612875e-10);
}

/***********************************************************
 To generate a random integer "k" in [i,j].                *
 Input: integers "seed", "i" and "j" in [0,2147483646]     *
 Ouput: integer in [i,j]                                   *
 ************************************************************/
int Bibrand::get_rand_ij(int i, int j) {

	randp();

	return ((double) seed / ((double) 2147483647 / ((double) (j - i + 1)))) + i;

}

/**************************************************************
 To generate a random integer "k" in [0,size].                *
 Input: integers "seed" and "size" in [0,2147483646]          *
 Ouput: integer in [0,size]                                   *
 **************************************************************/
int Bibrand::get_rand(int size) {
	randp();
	return ((double) seed / ((double) 2147483647 / ((double) size)));

}

/*************************************************
 To check the correctness of the implementation. *
 Ouput: correct (1) or wrong (0)                 *
 **************************************************/
int Bibrand::trand() {
	int i;

	seed = 1;

	for (i = 0; i < 1000; i++) {
		randp();
	}

	if (seed == 522329230)
		return 1;
	else
		return 0;

}

Bibrand::Bibrand(int s) {

	seed = s;
	/* esta primeira chamada evita o viciamento do primeiro n�mero aleat�rio fornecido */
	randp();
}

