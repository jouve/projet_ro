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

int main(int argc, char *argv[])
{
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

    for(vector<string>::iterator instance = instances.begin(); instance != instances.end(); ++instance)
    {
        cout << *instance << ",";
        MyGraph g(*instance);

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
}
