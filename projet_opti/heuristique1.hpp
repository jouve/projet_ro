#ifndef HEURISTIQUE1_HPP
#define HEURISTIQUE1_HPP

#include "heuristique.h"


/* Eval weight for each node
 * then remove the one with less weight
 * then update the nodes weight by remove the weight of deleted edges
 */
class  heuristique1 : public heuristique
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
        for (int j = countNodes(sg); j > g.K(); j--)
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

#endif // HEURISTIQUE1_HPP
