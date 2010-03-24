#include "array.h"
#include "dancing.h"
#include <iostream>
#include <list>

using std::cin;
using std::cerr;
using std::cout;
using std::endl;

typedef std::list<Node*> Collector;

Node *build_structure(const Array &array, Collector &collector) {
    Node *root = new Node("root",-1);
    collector.push_back(root);

    { //create columns row
    Node *column_prec = root;
    for (int j=0; j<array.width; j++) {
        Node::Id column_id("ac");
        column_id[0]+=j;

        Node *column = new Node(column_id,j);
        column_prec->insert_right(column);
        collector.push_back(column);

        column_prec = column;
    }
    }

    { //create rows column
    Node *row_prec = root;
    for (int i=0; i<array.height; i++) {
        Node::Id row_id("0r");
        row_id[0]+=i;
        
        Node *row = new Node(row_id,i);
        row_prec->insert_down(row);
        collector.push_back(row);

        row_prec = row;
    }
    }

    //insert ones in the structure
    for (Node *column=root->right; column!=root; column=column->right) {
        for (Node *row=root->down; row!=root; row=row->down) {
            if (array.get_value(row->data,column->data)) {
                Node::Id row_id("a0");
                row_id[0]+=column->data;
                row_id[1]+=row->data;

                Node *element = new Node(row_id,-1);
                column->insert_top(element);
                row->insert_left(element);
                collector.push_back(element);
            }
        }
    }

    return root;
}

void delete_collector(Collector &collector) {
    for (Collector::iterator i=collector.begin(); i!=collector.end(); i++) { delete *i; }
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
    Collector collector;
    Node *root = build_structure(array,collector);
    cout << "collector has " << collector.size() << " nodes ";
    cout << "expected " << array.width << "+" << array.height << "+1+" << array.get_number_of_ones() <<"=" << (array.width+array.height+1+array.get_number_of_ones()) << endl;
    print_root_as_array(root,cout);

    //solving
    SolveParams params(root,1);
    cout << "looking for  " << params.max_solution << " solution(s)" << endl;
    solve(params,cout);
    cout << "found " << params.solutions.size() << " solution(s)" << endl;
    for (SolveParams::Solutions::iterator isolution=params.solutions.begin(); isolution!=params.solutions.end(); isolution++) {
        cout << "solution=";
        for (SolveParams::Solution::const_iterator i=isolution->begin(); i!=isolution->end(); i++) { cout << (*i)->id << " "; }
        cout << endl;
    }

    print_root_as_array(root,cout);

    //delete collector
    delete_collector(collector);

	return 0;
}


