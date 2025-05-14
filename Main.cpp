#include <iostream>
#include <cmath>
#include "BarrierOption.h" 
#include "Random1.h"

int main() {

    srand(42); //Initialization of the random number generator
    //Parameters of the option
    double S0 = 100.0;   // Initial price of the underlying asset
    double K = 110.0;    // Strike price
    double sigma = 0.25; // Volatility
    double r = 0.05;     // Risk-free rate
    double T = 0.75;     // Time to maturity (in years)
    unsigned long nSim = 10000; //Number of Monte Carlo simulations

    //Specific parameter for the barrier option
    double b = 130.0; // Barrier level

    //Creation of the BarrierOption object
    BarrierOption barrierOption(S0, K, sigma, r, T, b, nSim);

    //Calculation of the barrier option price and standard deviation
    double stddev_price;  //Standard deviation of the price
    int barrierHits;      //Number of paths exceeding the barrier
    double barrierOptionPrice = barrierOption.price(stddev_price, barrierHits);

    // Output the results
    std::cout << "Price of the up-and-out barrier call: " << barrierOptionPrice << std::endl;
    std::cout << "Paths exceeding the barrier: " << barrierHits << " out of " << nSim << std::endl;
    std::cout << "Standard deviation of the price: " << stddev_price << std::endl;

    return 0;
} 