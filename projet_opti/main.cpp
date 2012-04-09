#include "MyGraph.hpp"
#include "heuristique1.hpp"
#include "heuristique2.hpp"
#include "heuristique3.hpp"
#include "recuit.h"

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

/*
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
*/

/* val weight for each node then remove the one with less weight
 */
/*
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
*/

int main(int argc, char *argv[])
{
    float T0 = 100;
    float r = 0.95;
    int iter = 1000;

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

    cout << "choisir une action : " << endl;
    cout << "  1: lancer les heuristiques (1, 2 et choix aléatoire)" << endl;
    cout << "  2: lancer le recuit" << endl;
    string input;
    cin >> input;

    int choice = atoi(input.c_str());
    if (choice == 1)
    {
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

                SubGraph<MyGraph > sg(g, nodeFilter, edgeFilter);

                heuristique1 heuristic;
                float weight = heuristic(g, sg);
                weights.push_back(weight);
            }
            //*/

            //*
            {
                ListGraph::NodeMap<bool> nodeFilter(g, false);
                ListGraph::EdgeMap<bool> edgeFilter(g, true);

                SubGraph<MyGraph > sg(g, nodeFilter, edgeFilter);

                heuristique1 aux;
                heuristique2 heuristic;
                float weight = heuristic(g, sg, aux);
                weights.push_back(weight);
            }
            //*/

            //*
            {
                ListGraph::NodeMap<bool> nodeFilter(g, true);
                ListGraph::EdgeMap<bool> edgeFilter(g, true);

                SubGraph<MyGraph > sg(g, nodeFilter, edgeFilter);

                heuristique3 heuristic;
                float weight = heuristic(g, sg);
                weights.push_back(weight);
            }
            //*/

            //*
            for(vector<float>::iterator it = weights.begin(); it != weights.end(); ++it)
            {
                cout << *it << ",";
            }
            //*/
            cout << endl;
        }
        else if(choice == 2)
        {
            cout << "choisir une heuristique : "<< endl;
            cout << "  1. heuristique 1" << endl;
            cout << "  2. heuristique 2 + 1" << endl;
            cout << "  3. choix aléatoire" << endl;
            string input2;
            cin >> input2;
            int choice2 = atoi(input2.c_str());

            heuristique *heur = NULL;
            if (choice2 == 1)
                heur = new heuristique1();
            else if (choice2 == 2)
                heur = new heuristique2();
            else if (choice2 == 3)
                heur = new heuristique3();
            else
                throw "bad choice";

            cout << "choisir T0 (defaut = 100)" << endl;
            string T0str;
            cin >> T0str;
            if (T0str.length() == 0)
                T0 = 100;

            cout << "filename, nb nodes, density, k, iter, heuristique1, interm values, tours, recuit" << endl;


        }
        else
            throw "bad choice";



        /*
        {
            ListGraph::NodeMap<bool> nodeFilter(g, true);
            ListGraph::EdgeMap<bool> edgeFilter(g, true);

            SubGraph<MyGraph> sg(g, nodeFilter, edgeFilter);

            heuristique3 heuristic;
            float weight = heuristic(g, sg);

            cout << weight << ",";

            float recuit = recuitSimule(g, sg, T0, r, iter);

            cout << recuit << ",";

        }
        //*/
        cout << endl;
    }

    return EXIT_SUCCESS;
}
