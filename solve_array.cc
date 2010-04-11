#include "array.h"
#include "dancing.h"
#include <iostream>
#include <cassert>

using std::cin;
using std::cerr;
using std::cout;
using std::endl;

Node *build_structure(const Array &array, Node::Collector &collector) {
    Node *root = Node::create_root(collector);

    //create constaints
    for (int j=0; j<array.width; j++) {
        Node::Id id("a constraint");
        id[0]+=j;
        Node::add_constraint(root,collector,id);
    }

    //create moves
    for (int i=0; i<array.height; i++) {
        Node::Id id("0 move");
        id[0]+=i;
        Node::add_move(root,collector,id);
    }

    //insert ones in the structure
    int j=0;
    for (Node *column=root->right; column!=root; column=column->right) {
        int i=0;
        for (Node *row=root->down; row!=root; row=row->down) {
            if (array.get_value(i,j)) {
                Node::Id id("  ");
                id[0] = column->get_id()[0];
                id[1] = row->get_id()[0];
                Node::add_link(row,column,collector,id);
            }
            i++;
        }
        j++;
    }

    return root;
}

void print_root_as_array(const Node *root, std::ostream &os) {
    if (root->right == root and root->down == root) {
        os << "null matrix" << endl;
        return;
    }

    os << " ";
    for (Node *column=root->right; column!=root; column=column->right) { os << column->get_id()[0]; }
    os << endl;

    for (Node *row=root->down; row!=root; row=row->down) {
        os << row->get_id()[0];
        Node *element = row->right;
        for (Node *column=root->right; column!=root; column=column->right) {
            if (column==element->headertop) {
                os << "1";
                element = element->right;
            } else {
                os << "0";
            }
        }
        assert(element==row);
        os << endl;
    }
}


int main(int argc, char *argv[]) {
    //loading array
    std::istream &input = cin;
    int width,height;
    input >> width >> std::ws >> height >> std::ws;
    if (input.bad()) { cerr << "error while finding array dimensions" << endl; return 1; }
	Array array(width,height);
	if (not array.parse(input)) { cerr << "error while parsing array\n"; return 1; }
    cout << array;

    //building structure
    Node::Collector collector;
    Node *root = build_structure(array,collector);
    cout << "collector has " << collector.size() << " nodes ";
    cout << "expected " << array.width << "+" << array.height << "+1+" << array.get_number_of_ones() <<"=" << (array.width+array.height+1+array.get_number_of_ones()) << endl;
    print_root_as_array(root,cout);

    //solving
    SolveParams params(root,1);
    cout << "looking for " << params.max_solution << " solution(s)" << endl;
    solve(params,cout,true);
    cout << "found " << params.solutions.size() << " solution(s)" << endl;
    for (SolveParams::Solutions::iterator isolution=params.solutions.begin(); isolution!=params.solutions.end(); isolution++) {
        cout << "solution=";
        for (SolveParams::Solution::const_iterator i=isolution->begin(); i!=isolution->end(); i++) { cout << **i << " "; }
        cout << endl;
    }

    print_root_as_array(root,cout);

    //delete collector
    delete_collector(collector);

	return 0;
}


