#include "dancing.h"
#include <iostream>
#include <cstdio>
#include <cassert>
#include <algorithm>

using std::cout;
using std::endl;

#define WIDTH 4
#define HEIGHT 16

Node *build_structure(Node::Collector &collector) {
    Node *root = Node::create_root(collector);

    // constraints
    Node *any_number[HEIGHT][WIDTH];
    for (int i=0; i<HEIGHT; i++) {
        for (int j=0; j<WIDTH; j++) {
            char id[256];
            snprintf(id,256,"any number in (%d,%d)",i,j);
            any_number[i][j] = Node::add_constraint(root,collector,id);
        }
    }

    Node *must_number[HEIGHT][WIDTH];
    must_number[0][0] = Node::add_constraint(root,collector,"2 in ligne 1");
    must_number[0][1] = Node::add_constraint(root,collector,"4 in ligne 1");
    must_number[0][2] = Node::add_constraint(root,collector,"5 in ligne 1");
    must_number[0][3] = Node::add_constraint(root,collector,"9 in ligne 1");

    must_number[1][0] = Node::add_constraint(root,collector,"1 in ligne 2");
    must_number[1][1] = Node::add_constraint(root,collector,"3 in ligne 2");
    must_number[1][2] = Node::add_constraint(root,collector,"6 in ligne 2");
    must_number[1][3] = Node::add_constraint(root,collector,"10 in ligne 2");

    must_number[2][0]  = Node::add_constraint(root,collector,"2 in ligne 3");
    must_number[2][1]  = Node::add_constraint(root,collector,"4 in ligne 3");
    must_number[2][2]  = Node::add_constraint(root,collector,"7 in ligne 3");
    must_number[2][3]  = Node::add_constraint(root,collector,"11 in ligne 3");

    must_number[3][0]  = Node::add_constraint(root,collector,"1 in ligne 4");
    must_number[3][1]  = Node::add_constraint(root,collector,"3 in ligne 4");
    must_number[3][2]  = Node::add_constraint(root,collector,"8 in ligne 4");
    must_number[3][3]  = Node::add_constraint(root,collector,"12 in ligne 4");

    must_number[4][0]  = Node::add_constraint(root,collector,"1 in ligne 5");
    must_number[4][1]  = Node::add_constraint(root,collector,"6 in ligne 5");
    must_number[4][2]  = Node::add_constraint(root,collector,"8 in ligne 5");
    must_number[4][3]  = Node::add_constraint(root,collector,"13 in ligne 5");

    must_number[5][0]  = Node::add_constraint(root,collector,"2 in ligne 6");
    must_number[5][1]  = Node::add_constraint(root,collector,"5 in ligne 6");
    must_number[5][2]  = Node::add_constraint(root,collector,"7 in ligne 6");
    must_number[5][3]  = Node::add_constraint(root,collector,"14 in ligne 6");

    must_number[6][0]  = Node::add_constraint(root,collector,"3 in ligne 7");
    must_number[6][1]  = Node::add_constraint(root,collector,"6 in ligne 7");
    must_number[6][2]  = Node::add_constraint(root,collector,"8 in ligne 7");
    must_number[6][3]  = Node::add_constraint(root,collector,"15 in ligne 7");

    must_number[7][0]  = Node::add_constraint(root,collector,"4 in ligne 8");
    must_number[7][1]  = Node::add_constraint(root,collector,"5 in ligne 8");
    must_number[7][2]  = Node::add_constraint(root,collector,"7 in ligne 8");
    must_number[7][3]  = Node::add_constraint(root,collector,"16 in ligne 8");

    must_number[8][0]  = Node::add_constraint(root,collector,"1 in ligne 9");
    must_number[8][1]  = Node::add_constraint(root,collector,"10 in ligne 9");
    must_number[8][2]  = Node::add_constraint(root,collector,"12 in ligne 9");
    must_number[8][3]  = Node::add_constraint(root,collector,"13 in ligne 9");

    must_number[9][0]  = Node::add_constraint(root,collector,"2 in ligne 10");
    must_number[9][1]  = Node::add_constraint(root,collector,"9 in ligne 10");
    must_number[9][2]  = Node::add_constraint(root,collector,"11 in ligne 10");
    must_number[9][3]  = Node::add_constraint(root,collector,"14 in ligne 10");

    must_number[10][0] = Node::add_constraint(root,collector,"3 in ligne 11");
    must_number[10][1] = Node::add_constraint(root,collector,"10 in ligne 11");
    must_number[10][2] = Node::add_constraint(root,collector,"12 in ligne 11");
    must_number[10][3] = Node::add_constraint(root,collector,"15 in ligne 11");

    must_number[11][0] = Node::add_constraint(root,collector,"4 in ligne 12");
    must_number[11][1] = Node::add_constraint(root,collector,"9 in ligne 12");
    must_number[11][2] = Node::add_constraint(root,collector,"11 in ligne 12");
    must_number[11][3] = Node::add_constraint(root,collector,"16 in ligne 12");

    must_number[12][0] = Node::add_constraint(root,collector,"5 in ligne 13");
    must_number[12][1] = Node::add_constraint(root,collector,"9 in ligne 13");
    must_number[12][2] = Node::add_constraint(root,collector,"14 in ligne 13");
    must_number[12][3] = Node::add_constraint(root,collector,"16 in ligne 13");

    must_number[13][0] = Node::add_constraint(root,collector,"6 in ligne 14");
    must_number[13][1] = Node::add_constraint(root,collector,"10 in ligne 14");
    must_number[13][2] = Node::add_constraint(root,collector,"13 in ligne 14");
    must_number[13][3] = Node::add_constraint(root,collector,"15 in ligne 14");

    must_number[14][0] = Node::add_constraint(root,collector,"7 in ligne 15");
    must_number[14][1] = Node::add_constraint(root,collector,"11 in ligne 15");
    must_number[14][2] = Node::add_constraint(root,collector,"14 in ligne 15");
    must_number[14][3] = Node::add_constraint(root,collector,"16 in ligne 15");

    must_number[15][0] = Node::add_constraint(root,collector,"8 in ligne 16");
    must_number[15][1] = Node::add_constraint(root,collector,"12 in ligne 16");
    must_number[15][2] = Node::add_constraint(root,collector,"13 in ligne 16");
    must_number[15][3] = Node::add_constraint(root,collector,"15 in ligne 16");

    Node *column_filled[HEIGHT][WIDTH];
    for (int i=0; i<HEIGHT; i++) {
        for (int j=0; j<WIDTH; j++) {
            char id[256];
            snprintf(id,256,"%d in column %d",i+1,j);
            column_filled[i][j] = Node::add_constraint(root,collector,id);
        }
    }

    // moves
    Node *moves[HEIGHT][WIDTH][HEIGHT];
    for (int i=0; i<HEIGHT; i++) {
        for (int j=0; j<WIDTH; j++) {
            for (int k=0; k<HEIGHT; k++) {
                char id[256];
                snprintf(id,256,"%d in (%d,%d)",k+1,i,j);
                Node *move = Node::add_move(root,collector,id);
                moves[i][j][k] = move;

                Node::add_link(move,any_number[i][j],collector,"any link");
                Node::add_link(move,column_filled[k][j],collector,"column link");
            }
        }
    }

    for (int j=0; j<WIDTH; j++) Node::add_link(moves[15][j][7],must_number[15][0],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[15][j][11],must_number[15][1],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[15][j][12],must_number[15][2],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[15][j][14],must_number[15][3],collector,"line link");

    for (int j=0; j<WIDTH; j++) Node::add_link(moves[14][j][6],must_number[14][0],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[14][j][10],must_number[14][1],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[14][j][13],must_number[14][2],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[14][j][15],must_number[14][3],collector,"line link");

    for (int j=0; j<WIDTH; j++) Node::add_link(moves[13][j][5],must_number[13][0],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[13][j][9],must_number[13][1],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[13][j][12],must_number[13][2],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[13][j][14],must_number[13][3],collector,"line link");

    for (int j=0; j<WIDTH; j++) Node::add_link(moves[12][j][4],must_number[12][0],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[12][j][8],must_number[12][1],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[12][j][13],must_number[12][2],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[12][j][15],must_number[12][3],collector,"line link");

    for (int j=0; j<WIDTH; j++) Node::add_link(moves[11][j][3],must_number[11][0],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[11][j][8],must_number[11][1],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[11][j][10],must_number[11][2],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[11][j][15],must_number[11][3],collector,"line link");

    for (int j=0; j<WIDTH; j++) Node::add_link(moves[10][j][2],must_number[10][0],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[10][j][9],must_number[10][1],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[10][j][11],must_number[10][2],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[10][j][14],must_number[10][3],collector,"line link");

    for (int j=0; j<WIDTH; j++) Node::add_link(moves[9][j][1],must_number[9][0],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[9][j][8],must_number[9][1],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[9][j][10],must_number[9][2],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[9][j][13],must_number[9][3],collector,"line link");

    for (int j=0; j<WIDTH; j++) Node::add_link(moves[8][j][0],must_number[8][0],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[8][j][9],must_number[8][1],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[8][j][11],must_number[8][2],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[8][j][12],must_number[8][3],collector,"line link");

    for (int j=0; j<WIDTH; j++) Node::add_link(moves[7][j][3],must_number[7][0],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[7][j][4],must_number[7][1],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[7][j][6],must_number[7][2],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[7][j][15],must_number[7][3],collector,"line link");

    for (int j=0; j<WIDTH; j++) Node::add_link(moves[6][j][2],must_number[6][0],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[6][j][5],must_number[6][1],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[6][j][7],must_number[6][2],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[6][j][14],must_number[6][3],collector,"line link");

    for (int j=0; j<WIDTH; j++) Node::add_link(moves[5][j][1],must_number[5][0],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[5][j][4],must_number[5][1],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[5][j][6],must_number[5][2],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[5][j][13],must_number[5][3],collector,"line link");

    for (int j=0; j<WIDTH; j++) Node::add_link(moves[0][j][1],must_number[0][0],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[0][j][3],must_number[0][1],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[0][j][4],must_number[0][2],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[0][j][8],must_number[0][3],collector,"line link");

    for (int j=0; j<WIDTH; j++) Node::add_link(moves[1][j][0],must_number[1][0],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[1][j][2],must_number[1][1],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[1][j][5],must_number[1][2],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[1][j][9],must_number[1][3],collector,"line link");

    for (int j=0; j<WIDTH; j++) Node::add_link(moves[2][j][1],must_number[2][0],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[2][j][3],must_number[2][1],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[2][j][6],must_number[2][2],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[2][j][10],must_number[2][3],collector,"line link");

    for (int j=0; j<WIDTH; j++) Node::add_link(moves[3][j][0],must_number[3][0],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[3][j][2],must_number[3][1],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[3][j][7],must_number[3][2],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[3][j][11],must_number[3][3],collector,"line link");

    for (int j=0; j<WIDTH; j++) Node::add_link(moves[4][j][0],must_number[4][0],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[4][j][5],must_number[4][1],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[4][j][7],must_number[4][2],collector,"line link");
    for (int j=0; j<WIDTH; j++) Node::add_link(moves[4][j][12],must_number[4][3],collector,"line link");

    return root;
}

void print_root_as_sukodu(const Node *root, std::ostream &os) {
    int possible_count = 0;
    for (Node *row=root->down; row!=root; row=row->down) { possible_count++; }
    os << "root has " << possible_count << " possible moves" << endl;
    for (Node *row=root->down; row!=root; row=row->down) {
        os << *row << " -> ";
        for (Node *element=row->right; element!=row; element=element->right) { os << *element->headertop << " "; }
        os << endl;
    }

    int requirement_count = 0;
    for (Node *column=root->right; column!=root; column=column->right) { requirement_count++; }
    os << "root has " << requirement_count << " requirements" << endl;
    for (Node *column=root->right; column!=root; column=column->right) { os << *column << endl; }
}

int main(int argc, char *argv[]) {
    Node::Collector collector;
    Node *root = build_structure(collector);
    cout << "collector has " << collector.size() << " nodes" << endl;

    //solving
    SolveParams params(root,1);
    print_root_as_sukodu(root,cout);
    cout << "looking for " << params.max_solution << " solution(s)" << endl;
    solve(params,cout);
    cout << "found " << params.solutions.size() << " solution(s)" << endl;
    for (SolveParams::Solutions::iterator isolution=params.solutions.begin(); isolution!=params.solutions.end(); isolution++) {
        assert(isolution->size() == WIDTH*HEIGHT);
        cout << "solution=";
        for (SolveParams::Solution::const_iterator i=isolution->begin(); i!=isolution->end(); i++) { cout << **i << endl; }
        cout << endl;
    }

    delete_collector(collector);
}

