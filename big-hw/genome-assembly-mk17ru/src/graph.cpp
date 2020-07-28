#include <stack>
#include <unordered_set>
#include <iostream>
#include <set>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include "graph.h"

namespace genome
{

size_t Edge::id = 0;

Edge::Edge(Node from, Node to)
    : m_from(from), m_to(to), m_id(id++)
{
}

bool Edge::operator==(const Edge & other) const
{
    return m_from == other.m_from && m_to == other.m_to && m_id == other.m_id;
}

void Graph::add_edge(Edge edge)
{
    edges[edge.from()].push_back(edge.to());
}

std::vector<Edge> Graph::find_euler_path()
{
    std::unordered_map<Node, int> us_e, us_n;
    std::stack<Node> st;
    std::vector<Edge> euler_path;
    for (const auto & p : edges) {
        for (const auto & u : p.second) {
            us_n[u]++;
            us_e[p.first]++;
        }
    }
    Node start = "";
    for (const auto & cur : edges) {
        if (((us_e[cur.first] + us_n[cur.first]) & 1) && (us_e[cur.first] > us_n[cur.first])) {
            start = cur.first;
            break;
        }
    }

    if (start != "") {
        st.push(start);
    }
    while(!st.empty()) {
        Node cur = st.top();
        if (edges[cur].empty()) {
            st.pop();
            if (!st.empty()) {
                euler_path.emplace_back(st.top(), cur);
            }
        } else {
            const auto & p = edges[cur].back();
            st.push(p);
            edges[cur].pop_back();
        }
    }
    return euler_path;
}

    void Graph::fast_add_edge(Node node1, Node node2) {
        edges[node1].push_back(node2);
    }


} // namespace genome
