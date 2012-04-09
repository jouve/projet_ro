#ifndef HEURISTIQUE2_HPP
#define HEURISTIQUE2_HPP

#include "heuristique.h"

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

        for (SubGraph<MyGraph>::NodeIt n(sg); n != INVALID; ++n)
        {
            sg.disable((SubGraph<MyGraph>::Node)n);
        }

        vector<MyGraph::Edge> edges;
        for (MyGraph::EdgeIt e(g); e != INVALID; ++e)
        {
            edges.push_back((MyGraph::Edge)e);
        }
        Comparator comp(g);
        sort(edges.begin(), edges.end(), comp);

        int cpt = 0;
        for(vector<MyGraph::Edge>::iterator edge = edges.begin(); edge != edges.end() && cpt != k; ++edge, ++cpt)
        {
            MyGraph::Node u = g.u(*edge);
            MyGraph::Node v = g.v(*edge);
            sg.enable((MyGraph::Node)u);
            sg.enable((MyGraph::Node)v);
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

#endif // HEURISTIQUE2_HPP
