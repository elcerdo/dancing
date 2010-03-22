#include <iostream>
#include <string>
#include <cassert>
#include <list>

using std::endl;
using std::cin;
using std::cout;
using std::cerr;

typedef std::string Id;

struct Node {
	Node(const Id &id, int data) : header(NULL), left(NULL), right(NULL), top(NULL), down(NULL), id(id), data(data) {}

    void print(std::ostream &os) {
        os << id << " " << data;
        os << " header=" << (header==NULL ? "null" : header->id);
        os << " left=" << (left==NULL ? "null" : left->id);
        os << " right=" << (right==NULL ? "null" : right->id);
        os << " top=" << (top==NULL ? "null" : top->id);
        os << " down=" << (down==NULL ? "null" : down->id);
    }
	Node *header;
	Node *left,*right;
	Node *top,*down;
	const Id id;
    const int data;
};

typedef std::list<Node*> Nodes;

struct Array {
	Array(int width, int height) : width(width), height(height), size(width*height) {
		data = new bool[size];
		for (int k=0; k<size; k++) { data[k] = false; }
	}
	~Array() { delete [] data; }

	bool parse(std::istream &is) {
		for (int i=0; i<height; i++) {
			for (int j=0; j<width; j++) {
				char foo;
                is >> foo >> std::ws;
				if (is.bad()) return false;
				switch(foo) {
				case '0':
					set_value(i,j,false);
					break;
				case '1':
					set_value(i,j,true);
					break;
				default:
					return false;
					break;
				}
			}
		}
		return true;
	}

	void print(std::ostream &os) {
		for (int i=0; i<height; i++) {
			for (int j=0; j<width; j++) { os << get_value(i,j); }
			os << endl;
		}
	}


	bool get_value(int i, int j) const {
		assert(i >= 0 and i < height);
		assert(j >= 0 and j < width);
		return data[i+height*j];
	}

	void set_value(int i, int j, bool value) {
		assert(i >= 0 and i < height);
		assert(j >= 0 and j < width);
		data[i+height*j] = value;
	}

	const int width, height, size;
	bool *data;
};

Node *build_structure(const Array &array, Nodes &collector) {
    Node *root = new Node("root",-1);
    collector.push_back(root);

    Nodes columns;
    Node *column_prec = NULL;
    for (int j=0; j<array.width; j++) {
        Id column_id("acol");
        column_id[0]+=j;

        Node *column = new Node(column_id,j);
        column->header = root;
        if (column_prec) {
            column->left = column_prec;
            column->left->right = column;
        } else root->down = column;
        collector.push_back(column);

        Node *row_prec = NULL;
        for (int i=0; i<array.height; i++) if (array.get_value(i,j)) {
            Id row_id("a0");
            row_id[0]+=j;
            row_id[1]+=i;

            Node *row = new Node(row_id,i);
            row->header = column;
            if (row_prec) {
                row->top = row_prec;
                row->top->down = row;
            } else column->down = row;

            for (Nodes::iterator icolumn=columns.begin(); icolumn!=columns.end(); icolumn++) {
                Node *current = (*icolumn)->down;
                while (current) {
                    if (current->data == i) break;
                    current = current->down;
                }
                if (current) {
                    row->left = current;
                    row->left->right = row;
                    break;
                }
            }

            collector.push_back(row);

            row_prec = row;
        }

        columns.push_front(column);
        column_prec = column;
    }

    return root;
}


int main(int argc, char *argv[]) {
	Array array(7,6);
	if (not array.parse(cin)) { cerr << "error while parsing array\n"; return 1; }
	array.print(cout);

    Nodes collector;
    Node *root = build_structure(array,collector);

    for (Nodes::const_iterator i=collector.begin(); i!=collector.end(); i++) { 
        (*i)->print(cout);
        cout << endl;
    }

	return 0;
}


