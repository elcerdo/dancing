#ifndef __ANDREW_H__
#define __ANDREW_H__

#include "dancing.h"

#define WIDTH 4
#define HEIGHT 16

Node *build_structure(Node::Collector &collector);
void fill_cell(SolveParams &params,int i, int j, int k);

#endif
