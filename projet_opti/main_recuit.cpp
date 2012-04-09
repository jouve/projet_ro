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

int main(int argc, char *argv[])
{
    int heuristiqueNo = 1;
    float T0 = 100;
    float r = 0.95;
    int iter = 1000;

    if (argc == 5)
    {
        heuristiqueNo = atoi(argv[1]);
        T0 = atof(argv[2]);
        r = atof(argv[3]);
        iter = atoi(argv[4]);
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

    heuristique *heur = NULL;
    if (heuristiqueNo == 1)
        heur = new heuristique1();
    else if (heuristiqueNo == 2)
        heur = new heuristique2();
    else if (heuristiqueNo == 3)
        heur = new heuristique3();
    else
        throw "bad choice";

    cout << "filename, nb nodes, density, k, iter, heuristique1, interm values, tours, recuit" << endl;

    for(vector<string>::iterator instance = instances.begin(); instance != instances.end(); ++instance)
    {
        MyGraph g(*instance);
        int nodesCount = countNodes(g);
        float density = 100 * 2 * (float)countEdges(g)/ (float)(nodesCount * (nodesCount - 1));
        cout << *instance << "," << nodesCount << "," << density << "," << g.K() << "," << iter << ",";

        ListGraph::NodeMap<bool> nodeFilter(g, true);
        ListGraph::EdgeMap<bool> edgeFilter(g, true);

        SubGraph<MyGraph> sg(g, nodeFilter, edgeFilter);

        float weight = 0;
        if (heuristiqueNo == 1 || heuristiqueNo == 3)
            weight = heur->operator ()(g, sg);
        else
        {
            heuristique aux = heuristique1();
            weight = heur->operator ()(g, sg, aux);
        }

        cout << weight << ",";

        float recuit = recuitSimule(g, sg, T0, r, iter);

        cout << recuit << ",";
        cout << endl;
    }
    //*/

    return EXIT_SUCCESS;
}
