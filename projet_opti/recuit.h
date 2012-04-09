#ifndef RECUIT_H
#define RECUIT_H

#include "MyGraph.hpp"

float recuitSimule(MyGraph& g,
                   SubGraph<MyGraph>& sg,
                   float T,
                   float coeff,
                   int iter_palier);

#endif // RECUIT_H
