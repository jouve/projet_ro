#include <lemon/list_graph.h>
#include <lemon/maps.h>
#include <lemon/adaptors.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <cstdlib>
#include <cstdio>
#include <cmath>

#include <cassert>
#include <time.h>

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

class MySubgraph : public SubGraph<MyGraph>
{
    typedef SubGraph<MyGraph> Parent;

    MySubgraph(MyGraph& g, bool def)
    {
        MyGraph::NodeMap<bool> nodeFilter(g, def);
        MyGraph::EdgeMap<bool> edgeFilter(g, true);
        initialize(g, nodeFilter, edgeFilter);
    }

    float nodeWeight(Node n)
    {
        float weightSum = 0;
        for (IncEdgeIt e(*this, n); e != INVALID; ++e) {
            weightSum +=  _graph->Weight()[(Edge)e];
        }
        return weightSum;
    }

    float totalWeight()
    {
        float weight = 0;
        for (EdgeIt e(*this); e != INVALID; ++e) {
            weight += _graph->Weight()[(Edge)e];
        }
        return weight;
    }
};

class heuristique
{
public:
    virtual float operator() (MyGraph& g, SubGraph<MyGraph>& sg) {return countNodes(g) < countNodes(sg);}
    virtual float operator() (MyGraph& g, SubGraph<MyGraph>& sg, heuristique& heuristic) {return heuristic(g, sg);}
};

/* val weight for each node then remove the one with less weight
 */
class heuristique1a : public heuristique
{
public:
    float operator() (MyGraph& g, SubGraph<MyGraph>& sg)
    {
        SubGraph<MyGraph>::NodeMap<float> _nodeWeight(sg);

        // Associate each node with the weight of edges
        for(SubGraph<MyGraph>::NodeIt n(sg); n != INVALID; ++n)
        {
            _nodeWeight[(SubGraph<MyGraph>::Node)n] = g.nodeWeight((SubGraph<MyGraph>::Node)n);
        }

        for (int j = countNodes(g); j > g.K(); j--)
        {
            SubGraph<MyGraph>::Node lowestWeightNode;
            float lowestWeight = 1000000000;
            // Put the node with the minimal weight in lowestWeightNode
            for (SubGraph<MyGraph>::NodeIt n(sg); n != INVALID; ++n)
            {
                float nWeight = _nodeWeight[n];
                if (nWeight < lowestWeight)
                {
                    lowestWeightNode = n;
                    lowestWeight = nWeight;
                }
            }
            // and disable it
            sg.disable(lowestWeightNode);
        }

        return g.SubGraphWeight(sg);
    }
};

/* Eval weight for each node
 * then remove the one with less weight
 * then update the nodes weight by remove the weight of deleted edges
 */
class  heuristique1b : public heuristique
{
public:
    float operator() (MyGraph& g, SubGraph<MyGraph>& sg)
    {
        SubGraph<MyGraph>::NodeMap<float> nodeWeight(sg);

        // init
        for (SubGraph<MyGraph>::NodeIt n(sg); n != INVALID; ++n)
        {
            nodeWeight[(SubGraph<MyGraph>::Node)n] = g.nodeWeight((SubGraph<MyGraph>::Node)n);
        }

        //loop
        for (int j = countNodes(g); j > g.K(); j--)
        {
            SubGraph<MyGraph>::Node lowestWeightNode;
            float lowestWeight = 1000000000;
            // Put the node with the minimal weight in lowestWeightNode
            for (SubGraph<MyGraph>::NodeIt n(sg); n != INVALID; ++n)
            {
                float nWeight = nodeWeight[n];
                if (nWeight < lowestWeight)
                {
                    lowestWeightNode = n;
                    lowestWeight = nWeight;
                }
            }

            // delete the values of the edges
            for (SubGraph<MyGraph>::IncEdgeIt e(sg, lowestWeightNode); e != INVALID; ++e) {
                SubGraph<MyGraph>::Edge edge = (SubGraph<MyGraph>::Edge)e;
                SubGraph<MyGraph>::Node otherEnd;
                if (g.u(edge) == lowestWeightNode)
                    otherEnd = g.v(edge);
                else
                    otherEnd = g.u(edge);
                nodeWeight[otherEnd] -= g.Weight()[edge];
            }

            // and disable it
            sg.disable(lowestWeightNode);
        }

        return g.SubGraphWeight(sg);
    }
};

class Comparator
{
public:
    Comparator(MyGraph& g) : _g(g) {}
    bool operator() (MyGraph::Edge e1, MyGraph::Edge e2) { return (_g.Weight()[e1] < _g.Weight()[e2]); }
private:
    MyGraph& _g;
};

//*
class heuristique2 : public heuristique
{
public:
    float operator() (MyGraph& g, SubGraph<MyGraph>& sg, heuristique& heuristic)
    {
        int k = g.K() * (g.K() - 1) / 2;

        vector<MyGraph::Edge> edges;
        for (MyGraph::EdgeIt e(g); e != INVALID; ++e)
        {
            edges.push_back((MyGraph::Edge)e);
        }
        //*
        //Comparator<MyGraph::Edge> comp = g.DescNodeComparator();
        Comparator comp(g);
        sort(edges.begin(), edges.end(), comp);

        //*/
        vector<MyGraph::Node> keptNodes;
        int cpt = 0;
        for(vector<MyGraph::Edge>::iterator edge = edges.begin(); edge != edges.end() && cpt != k; ++edge, ++cpt)
        {
            //*
            ++cpt;

            bool uInKeptNodes = false;
            bool vInKeptNodes = false;

            MyGraph::Node u = g.u(*edge);
            MyGraph::Node v = g.v(*edge);
            for(vector<MyGraph::Node>::iterator node = keptNodes.begin(); node != keptNodes.end(); ++node)
            {
                if (u == *node)
                {
                    uInKeptNodes = true;
                }
                if (v == *node)
                {
                    vInKeptNodes = true;
                }
            }
            if (!uInKeptNodes)
            {
                keptNodes.push_back(u);
            }
            if (!vInKeptNodes)
            {
                keptNodes.push_back(v);
            }
            //*/
        }

        for(vector<MyGraph::Node>::iterator node = keptNodes.begin(); node != keptNodes.end(); ++node)
        {
            sg.enable(*node);
        }

        float tmpWeight = 0;
        if (countNodes(sg) > g.K())
        {
            tmpWeight = heuristic(g, sg);
        }
        else
        {
            tmpWeight = g.SubGraphWeight(sg);
        }
        return tmpWeight;
    }
};

float recuitSimule(MyGraph& g,
                   SubGraph<MyGraph>& sg,
                   float T,
                   float coeff,
                   int iter_palier)
{
    float E = g.SubGraphWeight(sg);
    float Eopti = E;
    int nbSolAmelio = 0;

    ListGraph::NodeMap<bool> nodeFilter(g, false);
    ListGraph::EdgeMap<bool> edgeFilter(g, true);

    SubGraph<MyGraph> sg_opti(g, nodeFilter, edgeFilter);
    for (SubGraph<MyGraph>::NodeIt n(sg); n != INVALID; ++n)
    {
        sg_opti.enable((SubGraph<MyGraph>::Node)n);
    }

    vector<int> S_work;
    for (SubGraph<MyGraph>::NodeIt n(sg); n != INVALID; ++n)
    {
        S_work.push_back(g.id((SubGraph<MyGraph>::Node)n));
    }

    vector<MyGraph::Node> S_graph;
    for (MyGraph::NodeIt n(g); n != INVALID; ++n)
    {
        S_graph.push_back((MyGraph::Node)n);
    }

    int ii = 0;
    cout << "[";
    do
    {
        ++ii;
        nbSolAmelio = 0;
        for(int i = 0; i < iter_palier; i++)
        {

            int old_idx = rand() % S_work.size();
            // Choose one node in S
            SubGraph<MyGraph>::Node Nold = g.nodeFromId(S_work[old_idx]);

            int new_idx = 0;
            // Choose one node not in S
            MyGraph::Node& Nnew = S_graph[new_idx];
            bool ok = false;
            do
            {
                new_idx = rand() % S_graph.size();
                Nnew = S_graph[new_idx];

                ok = true;
                for(SubGraph<MyGraph>::NodeIt node(sg); node != INVALID; ++node)
                {
                    if (g.id((SubGraph<MyGraph>::Node)node) == g.id(Nnew))
                    {
                        ok = false;
                        break;
                    }
                }
            } while( !ok );

            float weightMinus = g.nodeWeight(sg, Nold);
            sg.disable(Nold);
            sg.enable(Nnew);
            S_work[old_idx] = g.id(Nnew);
            float weightPlus = g.nodeWeight(sg, Nnew);
            float delta = weightPlus - weightMinus;

            if (delta >= 0)
            {
                nbSolAmelio++;
                E += delta;
                if (E > Eopti)
                {
                    Eopti = E;
                    for (SubGraph<MyGraph>::NodeIt n(sg_opti); n != INVALID; ++n)
                    {
                        sg_opti.disable((SubGraph<MyGraph>::Node)n);
                    }
                    for (SubGraph<MyGraph>::NodeIt n(sg); n != INVALID; ++n)
                    {
                        sg_opti.enable((SubGraph<MyGraph>::Node)n);
                    }
                    cout << Eopti << "|";
                }
                else
                {
                    cout << Eopti << "|";
                }
            }
            else
            {
                float p = (float)rand() / (float)RAND_MAX;
                if (p <= exp(delta / T))
                {
                    E += delta;
                    nbSolAmelio++;
                }
                else
                {
                    sg.disable(Nnew);
                    sg.enable(Nold);
                    S_work[old_idx] = g.id(Nold);
                }
            }
        }
        T *= coeff;
    } while (((float)nbSolAmelio / (float)iter_palier) > 0.05);

    cout << "]," << ii << ",";

    return Eopti;
}

int main(int argc, char *argv[])
{
    float T0 = 100;
    float r = 0.95;
    int iter = 10000;

    if (argc == 4)
    {
        T0 = atof(argv[1]);
        r = atof(argv[2]);
        iter = atoi(argv[3]);
    }

    vector<string> instances;

    ifstream file("liste.ls", ifstream::in);

    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
            instances.push_back(line);
    }
    else
    {
        throw "error";
    }

    srand ( time(NULL) );

    vector<int> maxid(4, 0);

    cout << "filename, nb nodes, density, k, iter, heuristique1, heuristique2, best" << endl;

    for(vector<string>::iterator instance = instances.begin(); instance != instances.end(); ++instance)
    {
        MyGraph g(*instance);
        int nodesCount = countNodes(g);
        float density = 100 * 2 * (float)countEdges(g)/ (float)(nodesCount * (nodesCount - 1));
        cout << *instance << "," << nodesCount << "," << density << "," << g.K() << "," << iter << ",";

        vector<float> weights;
        //*
        {
            ListGraph::NodeMap<bool> nodeFilter(g, true);
            ListGraph::EdgeMap<bool> edgeFilter(g, true);

            SubGraph<MyGraph> sg(g, nodeFilter, edgeFilter);

            heuristique1b heuristic;
            float weight = heuristic(g, sg);
            weights.push_back(weight);
        }
        //*/

        //*
        {
            ListGraph::NodeMap<bool> nodeFilter(g, false);
            ListGraph::EdgeMap<bool> edgeFilter(g, true);

            SubGraph<MyGraph > sg(g, nodeFilter, edgeFilter);

            heuristique1b auxHeuristic;
            heuristique2 heuristic;
            float weight = heuristic(g, sg, auxHeuristic);
            weights.push_back(weight);
        }
        //*/

        //*
        for(vector<float>::iterator it = weights.begin(); it != weights.end(); ++it)
        {
            cout << *it << ",";
        }

        int max = 0;
        if (weights[0] > weights[1])
            max = 1;
        else if (weights[0] == weights[1])
            max = 3;
        else max = 2;

        cout << max << ",";
        //*/

        cout << endl;
    }

    return EXIT_SUCCESS;
}
