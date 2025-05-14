#include "BarrierOption.h"
#include "Random1.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <algorithm>

//Definition of the constructor
BarrierOption::BarrierOption(
    double S0_, 
    double K_, 
    double sigma_, 
    double r_, 
    double T_, 
    double b_, 
    int nSim_) {
        //Initialize the member variables with the given arguments
        S0 = S0_;
        K = K_;
        sigma = sigma_;
        r = r_;
        T = T_;
        b = b_;
        nSim = nSim_;
}

//Calculation of the price of a barrier option using the Monte Carlo simulation method
double BarrierOption::price(double& stddev_price, int& barrierHits) const {
    //Calculate variance and its square root based on volatility (sigma) and time to maturity (T)
    double variance = sigma * sigma * T;
    double root_variance = std::sqrt(variance);
    double ito_correction = -0.5 * variance; //Ito correction term for Geometric Brownian Motion

    double moved_spot = S0 * std::exp(r * T + ito_correction); //Calculate the drifted initial spot price accounting for the risk-free rate and Ito correction
    double running_sum = 0.0; //Running total of payoffs (for calculating the mean)
    double sum_squared_diff = 0.0; //To accumulate squared differences for variance calculation
    barrierHits = 0; //Counter for paths that exceed the barrier

    std::vector<double> payoffs; //Vector to store all the payoff values

    //To save some trajectories for later printing 
    int numTrajectoriesToPrint = 5; //Number of trajectories to print
    std::vector<std::vector<double>> printedTrajectories; //To save the trajectories


    //Start with the Monte Carlo simulation
    for (int i = 0; i < nSim; ++i) {
        bool hitBarrier = false; //to check if the barrier is hit during the simulation
        double this_spot = S0;
        std::vector<double> trajectory; // Save prices along this trajectory

        //Simulate the price process over 100 time intervals
        for (int t = 0; t < 100; ++t) {
            double dt = T / 100.0;
            double thisGaussian = GetOneGaussianByBoxMuller(); //Generate a random Gaussian variable
            //Simulate the next price step using the GBM formula
            this_spot *= std::exp((r - 0.5 * sigma * sigma) * dt + sigma * std::sqrt(dt) * thisGaussian);

            trajectory.push_back(this_spot); //Save the underlying price

            //Check if the price has hit or exceeded the barrier level
            if (this_spot >= b) { 
                hitBarrier = true; //Check if the barrier has been hit
                if (i < numTrajectoriesToPrint) {
                    std::cout << "Trajectory " << i + 1 << ": barrier reached at time step " << t + 1
                              << " with price " << this_spot << std::endl;
                }
                break; //Exit the loop early, as the barrier is hit

            }
        }
        //If the barrier wasn't hit, calculate the payoff of the option
        if (!hitBarrier) {
            double this_payoff = std::max(this_spot - K, 0.0); // Call option payoff
            running_sum += this_payoff; //Add to running sum of payoffs
            payoffs.push_back(this_payoff); //Store the payoff in the vector
        } else {
            barrierHits++; //Increment barrier hit counter
        }
        //Store some trajectories for later visualization
        if (i < numTrajectoriesToPrint) {
            printedTrajectories.push_back(trajectory); 
        }
    }
    //Calculation of the mean payoff of all non-hit trajectories
    double mean = running_sum / payoffs.size(); 
    //Calculation of the sample variance of the payoffs
    for (double payoff : payoffs) {
        sum_squared_diff += (payoff - mean) * (payoff - mean);
    }

    double variance_price = sum_squared_diff / (payoffs.size() - 1); //Sample variance
    stddev_price = std::sqrt(variance_price);//Standard deviation of the payoffs

    //Printing of some of the trajectories
    for (size_t i = 0; i < printedTrajectories.size(); ++i) {
        std::cout << "Trajectory " << i + 1 << ": ";
        for (double price : printedTrajectories[i]) {
            std::cout << price << " ";
        }
        std::cout << std::endl;
    }

    return mean * std::exp(-r * T); //Return of the discounted mean payoff (present value) of the option
}
