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

void loadFromFile(string filename)
{
    ifstream file(filename.c_str(), ifstream::in);

    if (! file.is_open())
        throw exception();

    string line;
    getline(file, line);
    stringstream s(std::stringstream::in | std::stringstream::out);
    s << line;
    int nvertex = -1, nedges = -1, k = -1;
    s >> nvertex;
    s >> k;
    s >> nedges;

    cout << "Maximize" << endl;
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

        if (i != 0)
            cout << " + " ;
        cout << edge_weight << " x_" << source << " x_" << target;
        //addEdge(initList[source - 1], initList[target - 1], edge_weight);
    }
    cout << endl;
    cout << "Subject To" << endl;
    for(int i = 1; i <= nvertex; i++) {
        if (i != 1)
            cout << " + ";
        cout << "x_" << i;
    }
    cout << endl;
    cout << "End" << endl;
}

int main(void)
{
    loadFromFile("kcluster40_025_10_1.txt");
}
