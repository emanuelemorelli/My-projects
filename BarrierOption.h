// BarrierOption.h
#ifndef BARRIER_OPTION_H   //Preprocessor directive to avoid multiple inclusions of this header file
#define BARRIER_OPTION_H

#include <vector> 

//Declaration of the BarrierOption class
class BarrierOption {
private:
    double S0;    //Initial price of the underlying asset (spot price)
    double K;     //Strike price of the option
    double sigma; //Volatility of the underlying asset
    double r;     //Risk-free interest rate 
    double T;     //Time to maturity 
    double b;     //Barrier level
    int nSim;     //Number of Monte Carlo simulations to run

public:
    //Constructor to initialize the option parameters
    BarrierOption(double S0_, double K_, double sigma_, double r_, double T_, double b_, int nSim_);

    //Method to calculate the option price using the Monte Carlo simulation method
    //stddev_price will store the standard deviation of the option price
    //barrierHits will store the number of simulations where the barrier was hit
    double price(double& stddev_price, int& barrierHits) const;  
    //'const' at the end of the method means this method does not modify any member variables of the object
};

#endif  