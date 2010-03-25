#include "dancing.h"
#include <iostream>
#include <cstdio>
#include <cassert>
#include <algorithm>

using std::cout;
using std::endl;

Node *build_structure(Collector &collector) {
    Node *root = new Node("root",-1);
    collector.push_back(root);
    
    Node *any_number[3][3];
    for (int i=0; i<3; i++) {
        for (int j=0; j<3; j++) {
            char id[256];
            snprintf(id,256,"any number in (%d,%d)",i,j);

            Node *column = new Node(id,-2);
            collector.push_back(column);
            any_number[i][j] = column;

            root->insert_left(column);
        }
    }

    Node *must_number[9];
    for (int k=0; k<9; k++) {
        char id[256];
        snprintf(id,256,"must have a %d",k+1);

        Node *column = new Node(id,-3);
        collector.push_back(column);
        must_number[k] = column;

        root->insert_left(column);
    }

    for (int i=0; i<3; i++) {
        for (int j=0; j<3; j++) {
            Node *any_number_column = any_number[i][j];
            for (int k=0; k<9; k++) {
                char id[256];
                snprintf(id,256,"%d in (%d,%d)",k+1,i,j);
                Node *row = new Node(id,-4);
                collector.push_back(row);
                root->insert_top(row);

                Node *element_any = new Node("a",1);
                collector.push_back(element_any);
                row->insert_left(element_any);
                any_number_column->insert_top(element_any);

                Node *element_must = new Node("m",2);
                collector.push_back(element_must);
                row->insert_left(element_must);
                must_number[k]->insert_top(element_must);

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

    cout << row->id << " -> ";
    for (Node *element=row->left; element!=row; element=element->left) { cout << element->headertop->id << " "; }
    cout << endl;
}

void print_root_as_sukodu(const Node *root, std::ostream &os) {
    int possible_count = 0;
    for (Node *row=root->down; row!=root; row=row->down) { possible_count++; }
    os << "root has " << possible_count << " possible moves" << endl;
    for (Node *row=root->down; row!=root; row=row->down) {
        os << row->id << " -> ";
        for (Node *element=row->right; element!=row; element=element->right) { os << element->headertop->id << " "; }
        os << endl;
    }

    int requirement_count = 0;
    for (Node *column=root->right; column!=root; column=column->right) { requirement_count++; }
    os << "root has " << requirement_count << " requirements" << endl;
    for (Node *column=root->right; column!=root; column=column->right) { os << column->id << endl; }
}

int main(int argc, char *argv[]) {
    Collector collector;
    Node *root = build_structure(collector);
    cout << "collector has " << collector.size() << " nodes" << endl;

    //solving
    SolveParams params(root,1);
    print_root_as_sukodu(root,cout);
    cout << "playing move" << endl;
    play_number(root,1,0,4,params);
    play_number(root,1,2,5,params);
    print_root_as_sukodu(root,cout);
    cout << "looking for  " << params.max_solution << " solution(s)" << endl;
    solve(params,cout);
    cout << "found " << params.solutions.size() << " solution(s)" << endl;
    for (SolveParams::Solutions::iterator isolution=params.solutions.begin(); isolution!=params.solutions.end(); isolution++) {
        assert(isolution->size() == 9);
        cout << "solution=";
        for (SolveParams::Solution::const_iterator i=isolution->begin(); i!=isolution->end(); i++) { cout << (*i)->id << " "; }
        cout << endl;
    }

    delete_collector(collector);
}

