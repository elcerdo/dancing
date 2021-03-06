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

void play_sudoku_move(SolveParams &params,int i, int j, int k) {
    char id[256];
    snprintf(id,256,"%d in (%d,%d)",k,i,j);
    Node *row = Node::find_move(params.root,id);
    params.play_move(row);
    cout << *row << endl;
}

int main(int argc, char *argv[]) {
    Node::Collector collector;
    Node *root = build_structure(collector);
    cout << "collector has " << collector.size() << " nodes" << endl;

    //solving
    SolveParams params(root,350);
    Node::print_root(root,cout,true);
    cout << "playing move" << endl;
    play_sudoku_move(params,1,0,4);
    play_sudoku_move(params,1,2,5);
    play_sudoku_move(params,0,0,1);
    play_sudoku_move(params,0,1,2);
    Node::print_root(root,cout,true);
    cout << "looking for " << params.max_solution << " solution(s)" << endl;
    params.solve(cout,true);
    cout << "found " << params.solutions.size() << " solution(s)" << endl;
    for (SolveParams::Solutions::iterator isolution=params.solutions.begin(); isolution!=params.solutions.end(); isolution++) {
        assert(isolution->size() == 9);
        cout << "solution=";
        for (SolveParams::Solution::const_iterator i=isolution->begin(); i!=isolution->end(); i++) { cout << **i << " "; }
        cout << endl;
    }

    delete_collector(collector);
}

