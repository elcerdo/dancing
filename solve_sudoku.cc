#include "dancing.h"
#include <iostream>
#include <cstdio>
#include <cassert>
#include <algorithm>

using std::cout;
using std::endl;

Node *build_structure(Node::Collector &collector) {
    Node *root = Node::create_root(collector);
    
    Node *any_number[3][3];
    for (int i=0; i<3; i++) {
        for (int j=0; j<3; j++) {
            char id[256];
            snprintf(id,256,"any number in (%d,%d)",i,j);
            any_number[i][j] = Node::add_constraint(root,collector,id);
        }
    }

    Node *must_number[9];
    for (int k=0; k<9; k++) {
        char id[256];
        snprintf(id,256,"must have a %d",k+1);
        must_number[k] = Node::add_constraint(root,collector,id);
    }

    for (int i=0; i<3; i++) {
        for (int j=0; j<3; j++) {
            for (int k=0; k<9; k++) {
                char id[256];
                snprintf(id,256,"%d in (%d,%d)",k+1,i,j);
                Node *move = Node::add_move(root,collector,id);

                Node::add_link(move,any_number[i][j],collector,"any link");
                Node::add_link(move,must_number[k],collector,"must link");
            }
        }
    }

    return root;
}

void play_number(Node *root,int i, int j, int k, SolveParams &params) {
    int row_index = (i*3 + j)*9 + k;
    Node *row = root->top;
    while (row!=root and row_index<(3*3*9)) {
        row_index++;
        row = row->top;
    }

    SolveParams::Nodes folded_columns;
    SolveParams::Nodes folded_rows;
    for (Node *element_row=row->right; element_row!=row; element_row=element_row->right) {
        Node *column = element_row->headertop;
        folded_columns.push_back(column);
        for (Node *element_column=column->down; element_column!=column; element_column=element_column->down) {
            Node *aarow = element_column->headerleft;
            bool row_foldable = (std::find(folded_rows.begin(),folded_rows.end(),aarow) == folded_rows.end());
            if (row_foldable) folded_rows.push_back(aarow);
        }

    }

    params.partial_solution.push_back(row);
    for (SolveParams::Nodes::iterator i=folded_columns.begin(); i!=folded_columns.end(); i++) { (*i)->fold_column(); }
    for (SolveParams::Nodes::iterator i=folded_rows.begin(); i!=folded_rows.end(); i++) { (*i)->fold_row(); }

    cout << *row << " -> ";
    for (Node *element=row->left; element!=row; element=element->left) { cout << *element->headertop << " "; }
    cout << endl;
}

int main(int argc, char *argv[]) {
    Node::Collector collector;
    Node *root = build_structure(collector);
    cout << "collector has " << collector.size() << " nodes" << endl;

    //solving
    SolveParams params(root,1);
    print_root(root,cout,true);
    cout << "playing move" << endl;
    play_number(root,1,0,4,params);
    play_number(root,1,2,5,params);
    print_root(root,cout,true);
    cout << "looking for " << params.max_solution << " solution(s)" << endl;
    solve(params,cout,true);
    cout << "found " << params.solutions.size() << " solution(s)" << endl;
    for (SolveParams::Solutions::iterator isolution=params.solutions.begin(); isolution!=params.solutions.end(); isolution++) {
        assert(isolution->size() == 9);
        cout << "solution=";
        for (SolveParams::Solution::const_iterator i=isolution->begin(); i!=isolution->end(); i++) { cout << **i << " "; }
        cout << endl;
    }

    delete_collector(collector);
}

