#ifndef HEURISTIQUE_H
#define HEURISTIQUE_H

#include "MyGraph.hpp"

class heuristique
{
public:
    virtual float operator() (MyGraph& g, SubGraph<MyGraph>& sg) {return countNodes(g) < countNodes(sg);}
    virtual float operator() (MyGraph& g, SubGraph<MyGraph>& sg, heuristique& heuristic) {return heuristic(g, sg);}
};


#endif // HEURISTIQUE_H
