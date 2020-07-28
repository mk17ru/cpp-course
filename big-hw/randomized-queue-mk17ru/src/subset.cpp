#include <iostream>
#include <cmath>
#include "randomized_queue.h"
#include "subset.h"

void subset(unsigned long k, std::istream & in, std::ostream & out)
{

    std::string line;
    randomized_queue<std::string> queue;
    while (std::getline(in, line)) {
        queue.enqueue(line);
    }
    auto iterator = queue.begin();
    for (size_t i = 0; i < queue.size() && i < k; ++i) {
        out << *iterator << std::endl;
    }
}
