//Random1.cpp
#include "Random1.h"
#include <cstdlib>
#include <cmath>

double GetOneGaussianByBoxMuller()
{
	double result;

	double x;
	double y;

	double sizeSquared;
	do //Do-while loop generates random points (x, y) until one lies inside the unit circle
	{
		//Generate uniform random variables x and y in the range [-1, 1]
		x = 2.0*rand()/static_cast<double>(RAND_MAX)-1;
		y = 2.0*rand()/static_cast<double>(RAND_MAX)-1;
		sizeSquared = x*x + y*y;
	}
	while
		( sizeSquared >= 1.0); //Keep generating random points until the distance is less than 1
	//Application of the Box-Muller transformation to generate a Gaussian random number
	result = x*sqrt(-2*log(sizeSquared)/sizeSquared);

	return result;  //Return the generated Gaussian random number

}