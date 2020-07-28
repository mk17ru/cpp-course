#include <graph.h>
#include <iostream>
#include "genome.h"

namespace genome
{

std::string assembly(size_t k, const std::vector<std::string> & fragments)
{
    Graph graph;
    for (const auto & p : fragments) {
        std::string_view temp(p);
        size_t ind = 0;
        while (ind < temp.size() - k) {
            graph.fast_add_edge(temp.substr(ind, k), temp.substr(ind + 1, k));
            ind++;
        }
    }

    std::vector<Edge> answer = graph.find_euler_path();
    std::string result;
    if (!answer.empty()) {
        result += answer.rbegin()->from();
    }
    for (int i = answer.size() - 1; i >= 0; --i) {
        if ((size_t)i + 1 != answer.size()) {
            result += answer[i].from()[k - 1];
        }
    }
    if (!answer.empty()) {
          result  += answer.begin()->to()[k - 1];
    }
    return result;
}

}
