#ifndef HEURISTIQUE3_HPP
#define HEURISTIQUE3_HPP

#include "heuristique.h"

/* val weight for each node then remove the one with less weight
 */
class heuristique3 : public heuristique
{
public:
    float operator() (MyGraph& g, SubGraph<MyGraph>& sg)
    {
        vector<int> idx;

        do {
            bool inIdx = false;
            int newId = rand() % countNodes(g);
            for(vector<int>::iterator it = idx.begin(); it != idx.end(); ++it)
            {
                if (*it == newId){
                    inIdx = true;
                    break;
                }
            }
            if (!inIdx)
                idx.push_back(newId);
        } while (idx.size() != g.K());

        sort(idx.begin(), idx.end());

        int i = 0;
        int cpt = 0;
        for(SubGraph<MyGraph>::NodeIt n(sg); n != INVALID; ++n)
        {
            if (i >= (int)idx.size())
                break;

            if (cpt == idx[i])
            {
                sg.enable((SubGraph<MyGraph>::Node)n);
                i++;
            }
            else
            {
                sg.disable((SubGraph<MyGraph>::Node)n);
            }
            cpt++;
        }

        return g.SubGraphWeight(sg);
    }
};


#endif // HEURISTIQUE3_HPP
