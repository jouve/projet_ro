#ifndef MYGRAPH_HPP
#define MYGRAPH_HPP

#include <lemon/list_graph.h>
#include <lemon/maps.h>
#include <lemon/adaptors.h>

#include <fstream>

using namespace std;
using namespace lemon;

class MyGraph : public ListGraph
{
public:
    MyGraph()
    {
        k = -1;
        number = new NodeMap<int>(*this);
        weight = new EdgeMap<float>(*this);
    }

    MyGraph(string file)
    {
        k = -1;
        number = new NodeMap<int>(*this);
        weight = new EdgeMap<float>(*this);
        loadFromFile(file);
    }

    NodeMap<int>& Number()
    {
        return *number;
    }

    EdgeMap<float>& Weight()
    {
        return *weight;
    }

    Node addNode(int value)
    {
        Node node = ListGraph::addNode();
        Number()[node] = value;
        return node;
    }

    Edge addEdge(Node u, Node v, float value)
    {
        Edge edge = ListGraph::addEdge(u, v);
        Weight()[edge] = value;
        return edge;
    }

    void loadFromFile(string filename)
    {
        ifstream file(filename.c_str(), ifstream::in);

        if (! file.is_open())
            throw exception();

        string line;
        getline(file, line);
        stringstream s(std::stringstream::in | std::stringstream::out);
        s << line;
        int nvertex = -1, nedges = -1;
        s >> nvertex;
        s >> k;
        s >> nedges;

        vector<MyGraph::Node> initList;

        for(int i = 1; i <= nvertex; i++)
        {
            Node node = addNode(i);
            initList.push_back(node);
        }

        for(int i = 0; i < nedges; i++)
        {
            getline(file, line);

            int source, target;
            float edge_weight;
            std::stringstream s_tmp(std::stringstream::in | std::stringstream::out);
            s_tmp << line;
            s_tmp >> source;
            s_tmp >> target;
            s_tmp >> edge_weight;
            edge_weight *= 2;

            addEdge(initList[source - 1], initList[target - 1], edge_weight);
        }
    }

    int K()
    {
        return k;
    }

    float nodeWeight(Node n)
    {
        float weightSum = 0;
        for (IncEdgeIt e(*this, n); e != INVALID; ++e) {
            weightSum +=  Weight()[(Edge)e];
        }
        return weightSum;
    }

    float nodeWeight(SubGraph<MyGraph> sg, Node n)
    {
        float weightSum = 0;
        for (SubGraph<MyGraph>::IncEdgeIt e(sg, n); e != INVALID; ++e) {
            weightSum +=  Weight()[(Edge)e];
        }
        return weightSum;
    }

    float SubGraphWeight(SubGraph<MyGraph>& sg)
    {
        float weight = 0;
        for (SubGraph<MyGraph>::EdgeIt e(sg); e != INVALID; ++e) {
            weight += Weight()[(SubGraph<MyGraph>::Edge)e];
        }
        return weight;
    }

private:
    int k;
    NodeMap<int> *number;
    EdgeMap<float> *weight;
};

#endif // MYGRAPH_HPP
