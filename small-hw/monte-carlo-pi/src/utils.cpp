#include "../include/utils.h"
#include <iostream>
#include <cmath>

std::uniform_real_distribution<> get_real_distribution()
{
	// produces random floating-point values uniformly distributed on the interval
	return std::uniform_real_distribution<>(0.0, 1.0);
}

std::mt19937_64 get_random_generator()
{
	// non-deterministic random number generator using hardware entropy source
	std::random_device random_device;
	// seeding Mersenne Twister with a non-deterministic value
	return std::mt19937_64(random_device());
}

double generate_random_number()
{
	static auto generator = get_random_generator();
	static auto dist = get_real_distribution();
	return dist(generator);
}

double GetPi(std::function<double()> generator,
		 size_t number_of_iterations)
{
    if (number_of_iterations == 0) {
        return 0;
    }
    double radius = 1.0;
    size_t number_of_points = 0;
    for (size_t i = 0; i < number_of_iterations; ++i) {
            double x = generator();
            double y = generator();
            if (x * x + y * y <= radius) {
                ++number_of_points;
            }
    }
    double P = (double)number_of_points / number_of_iterations;
    return 4 * P;
}
