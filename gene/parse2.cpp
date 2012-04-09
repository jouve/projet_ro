#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <cstdlib>
#include <cstdio>
#include <cmath>

#include <cassert>
#include <time.h>
#include <vector>

using namespace std;

void loadFromFile(string filename)
{
    ifstream file(filename.c_str(), ifstream::in);

    ofstream outf((filename+".lp").c_str());

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

    vector<pair<int, pair<int, int> > > my_vect;

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

        my_vect.push_back(pair<int, pair<int, int> >(edge_weight, pair<int, int>(source, target)));
    }

    outf << "Maximize" << endl;
    for(int i = 0; i < my_vect.size(); i++)
    {
        if (i != 0)
            outf << " + ";
        else
            outf << "  obj: ";

        pair<int, pair<int, int> > item = my_vect[i];
        outf << /*item.first <<*/ " y_" << item.second.first << "_" << item.second.second;
    }
    outf << endl;
    outf << "Subject To" << endl;
    int l = 0;

    outf << "  c" << l << ": ";l++;
    for(int i = 1; i <= nvertex; i++) {
        if (i != 1)
            outf << " + ";
        outf << "x_" << i;
    }
    outf << " = " << k << endl;

    for(int i = 0; i < my_vect.size(); i++)
    {
        pair<int, pair<int, int> > item = my_vect[i];
        outf << "  c" << l << ": x_" << item.second.first << " - y_" << item.second.first << "_" << item.second.second << " >= 0" << endl;
        l++;
    }
    for(int i = 0; i < my_vect.size(); i++)
    {
        pair<int, pair<int, int> > item = my_vect[i];
        outf << "  c" << l << ": x_" << item.second.second << " - y_" << item.second.first << "_" << item.second.second << " >= 0 " << endl;
        l++;
    }
    outf << "Binary" << endl;
    for(int i = 0; i < my_vect.size(); i++)
    {
        pair<int, pair<int, int> > item = my_vect[i];
        outf << /*item.first <<*/ " y_" << item.second.first << "_" << item.second.second << " ";
    }
    outf << endl;
    for(int i = 1; i <= nvertex; i++)
        outf << "x_" << i << " ";
    outf << endl;
    outf << "End" << endl;
}

int main(void)
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

    for(vector<string>::iterator instance = instances.begin(); instance != instances.end(); ++instance)
    {
        loadFromFile((string)(*instance));
    }
}
