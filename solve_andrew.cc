#include "andrew.h"
#include <iostream>
#include <cstdio>
#include <cassert>
#include <algorithm>
#include <sstream>

using std::cout;
using std::endl;

int main(int argc, char *argv[]) {
    // params
    int ngames = 10;
    if (argc>1) {
        std::stringstream ss(argv[1]);
        int ngames_cache;
        ss >> ngames_cache;
        if (not ss.fail()) { ngames = ngames_cache; }
    }

    // init stuff
    Node::Collector collector;
    Node *root = build_structure(collector);
    cout << "collector has " << collector.size() << " nodes" << endl;
    Node::print_root(root,cout,false);

    // playing move
    SolveParams params(root,ngames);
    cout << "filling cells" << endl;
    //fill_cell(params,0,0,5);
    //fill_cell(params,0,1,9);
    //fill_cell(params,0,2,4);
    //fill_cell(params,0,3,2);
    Node::print_root(root,cout,false);

    // solving
    cout << "looking for " << params.max_solution << " solution(s)" << endl;
    params.solve(cout,false);
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

