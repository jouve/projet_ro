#include "recuit.h"

#include <cmath>

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
                    //cout << Eopti << "|";
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
