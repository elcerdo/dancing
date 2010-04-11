#include "dancing.h"
#include <iostream>
#include <cstdio>
#include <cassert>
#include <algorithm>
#include <fstream>

using std::cout;
using std::endl;

#define WIDTH 4
#define HEIGHT 16

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

int main(int argc, char *argv[]) {
    // init stuff
    Node::Collector collector;
    Node *root = build_structure(collector);
    cout << "collector has " << collector.size() << " nodes" << endl;
    print_root(root,cout,false);

    // solving
    SolveParams params(root,3);
    cout << "looking for " << params.max_solution << " solution(s)" << endl;
    solve(params,cout,false);
    cout << "found " << params.solutions.size() << " solution(s)" << endl;
    int count=0;
    for (SolveParams::Solutions::iterator isolution=params.solutions.begin(); isolution!=params.solutions.end(); isolution++) {
        assert(isolution->size() == WIDTH*HEIGHT);

        printf("%d\n",count++);
        printf("  |  K  S  H  L\n");
        printf("--+------------\n");
        for (int i=0; i<HEIGHT; i++) {
            printf("%2d|",i+1);
            for (int j=0; j<WIDTH; j++) {
                int coup;
                for (SolveParams::Solution::const_iterator imove=isolution->begin(); imove!=isolution->end(); imove++) {
                    Node *move = *imove;
                    int im,jm;
                    int ret = sscanf(move->get_id().c_str(),"%d in (%d,%d)",&coup,&im,&jm);
                    assert(ret != EOF);
                    if (im == i and jm == j) break;
                }
                printf(" %2d",coup);
            }
            printf("\n");
        }
        printf("\n");

    }

    delete_collector(collector);
}

