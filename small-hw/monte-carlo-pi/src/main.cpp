#include <iostream>

#include "../include/utils.h"

int main()
{
    std::cout << GetPi(generate_random_number, 1000000) << std::endl;
}
