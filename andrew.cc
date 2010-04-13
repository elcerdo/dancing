#include "andrew.h"

#include <iostream>

using std::cout;
using std::endl;

const int possible_lines[HEIGHT][WIDTH] = {
{2,4,5,9},
{1,3,6,10},
{2,4,7,11},
{1,3,8,12},
{1,6,8,13},
{2,5,7,14},
{3,6,8,15},
{4,5,7,16},
{1,10,12,13},
{2,9,11,14},
{3,10,12,15},
{4,9,11,16},
{5,9,14,16},
{6,10,13,15},
{7,11,14,16},
{8,12,13,15}
};

Node *build_structure(Node::Collector &collector) {
    Node *root = Node::create_root(collector);

    // constraints
    // there must be a a number in each cell of the array
    // 16*4 = 64 constraints
    Node *any_number[HEIGHT][WIDTH];
    for (int i=0; i<HEIGHT; i++) {
        for (int j=0; j<WIDTH; j++) {
            char id[256];
            snprintf(id,256,"any number in (%d,%d)",i,j);
            any_number[i][j] = Node::add_constraint(root,collector,id);
        }
    }

    // there must be each 4 number in each line
    // 4*16 = 64 constraints
    Node *must_number[HEIGHT][WIDTH];
    for (int i=0; i<HEIGHT; i++) {
        for (int j=0; j<WIDTH; j++) {
            char id[256];
            snprintf(id,256,"%d in line %d",possible_lines[i][j],i);
            must_number[i][j] = Node::add_constraint(root,collector,id);
        }
    }

    // there must be each number once in each column
    // 16*4 = 64 constraints
    Node *column_filled[HEIGHT][WIDTH];
    for (int i=0; i<HEIGHT; i++) {
        for (int j=0; j<WIDTH; j++) {
            char id[256];
            snprintf(id,256,"%d in column %d",i+1,j);
            column_filled[i][j] = Node::add_constraint(root,collector,id);
        }
    }

    // moves
    // one can play each 16 numbers in each 16*4 cells of the array
    // 16*16*4 = 1024 moves
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

    // link line constraint to corresponding move
    for (int i=0; i<HEIGHT; i++) {
        for (int k=0; k<WIDTH; k++) {
            int number = possible_lines[i][k]-1;
            Node *constraint = must_number[i][k];
            for (int j=0; j<WIDTH; j++) {
                Node *move = moves[i][j][number];
                Node::add_link(move,constraint,collector,"must line link");
            }
        }
    }

    return root;
}

void fill_cell(SolveParams &params,int i, int j, int k) {
    char id[256];
    snprintf(id,256,"%d in (%d,%d)",k,i,j);
    Node *move = Node::find_move(params.root,id);

    params.play_move(move);

    cout << *move << endl;
}

