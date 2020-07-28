#pragma once

#include <vector>
#include <string_view>
#include <unordered_map>
#include <map>
#include<set>

namespace genome
{

// one possible starting point
//

using Node = std::string_view;

class Edge
{
public:
    Edge(Node from, Node to);

    const auto &from() const { return m_from; }
    const auto &to() const { return m_to; }

    bool operator==(const Edge &other) const;

private:
    Node m_from;
    Node m_to;
    size_t m_id;
    static size_t id;
};

class Graph
{
public:
    void add_edge(Edge edge);

    void fast_add_edge(Node node1, Node node2);


    std::vector<Edge> find_euler_path();

private:
        std::unordered_map<Node, std::vector<Node> > edges;
};

} // namespace genome
