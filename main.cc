#include <iostream>
#include <string>
#include <cassert>
#include <list>

using std::endl;
using std::cin;
using std::cout;
using std::cerr;

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

typedef std::string Id;

struct Node {
	Node(const Id &id, int data) : headertop(this), headerright(this), left(this), right(this), top(this), down(this), id(id), data(data) {}

    void insert_right(Node *node) {
        Node *list_right = this->right;
        this->right = node;
        this->right->left = this;
        list_right->left = node;
        list_right->left->right = list_right;
        node->headerright = this->headerright;
    }

    void insert_left(Node *node) {
        Node *list_left = this->left;
        this->left = node;
        this->left->right = this;
        list_left->right = node;
        list_left->right->left = list_left;
        node->headerright = this->headerright;
    }

    void insert_down(Node *node) {
        Node *list_down = this->down;
        this->down = node;
        this->down->top = this;
        list_down->top = node;
        list_down->top->down = list_down;
        node->headertop = this->headertop;
    }

    void insert_top(Node *node) {
        Node *list_top = this->top;
        this->top = node;
        this->top->down = this;
        list_top->down = node;
        list_top->down->top = list_top;
        node->headertop = this->headertop;
    }

    void print(std::ostream &os) {
        os << id << " " << data;
        os << " headerright=" << (headerright==this ? "self" : headerright->id);
        os << " left=" << (left==this ? "self" : left->id);
        os << " right=" << (right==this ? "self" : right->id);
        os << " headertop=" << (headertop==this ? "self" : headertop->id);
        os << " top=" << (top==this ? "self" : top->id);
        os << " down=" << (down==this ? "self" : down->id);
    }
	Node *headertop,*headerright;
	Node *left,*right;
	Node *top,*down;
	const Id id;
    const int data;
};


typedef std::list<Node*> Nodes;

Node *build_structure(const Array &array, Nodes &collector) {
    Node *root = new Node("root",-1);
    collector.push_back(root);

    {
    Node *column_prec = root;
    for (int j=0; j<array.width; j++) {
        Id column_id("ac");
        column_id[0]+=j;

        Node *column = new Node(column_id,j);
        column_prec->insert_right(column);
        collector.push_back(column);

        column_prec = column;
    }
    }

    {
    Node *row_prec = root;
    for (int i=0; i<array.height; i++) {
        Id row_id("0r");
        row_id[0]+=i;
        
        Node *row = new Node(row_id,i);
        row_prec->insert_down(row);
        collector.push_back(row);

        row_prec = row;
    }
    }

    for (Node *column=root->right; column!=root; column=column->right) {
        for (Node *row=root->down; row!=root; row=row->down) {
            if (array.get_value(row->data,column->data)) {
                Id row_id("a0");
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

Node *find_minimum_column(Node *root) {
    Node *column_min = NULL;
    int length_min = 0;

    for (Node *column=root->right; column!=root; column=column->right) {
        int length = 0;
        for (Node *row=column->down; row!=column; row=row->down) { length++; }

        column->print(cout);
        cout << " -> " << length << endl;

        if (length > 0 && (column_min == NULL || length < length_min)) {
            length_min = length;
            column_min = column;
        }
    }

    return column_min;
}

void print_root(const Node *root, std::ostream &os) {
    os << " ";
    for (Node *column=root->right; column!=root; column=column->right) { os << column->id[0]; }
    os << endl;


}


typedef std::list<int> Solution;
            
bool solve(Node *root, Solution &partial_solution) {
    if (not root->down) {
        cout << "found solution" << endl;
        return true;
    }

    Node *min_column = find_minimum_column(root);
    if (not min_column) {
        cout << "no min column" << endl;
        return false;
    }

    cout << "min column is ";
    min_column->print(cout);
    cout << endl;

    return true;

    Node *row = min_column->down;
    while (row) {
        cout << "trying row ";
        row->print(cout);
        cout << endl;

        row = row->down;
    }

    return true;
}

int main(int argc, char *argv[]) {
	Array array(7,6);
	if (not array.parse(cin)) { cerr << "error while parsing array\n"; return 1; }
	array.print(cout);

    Nodes collector;
    Node *root = build_structure(array,collector);

    print_root(root,cout);
    for (Nodes::const_iterator i=collector.begin(); i!=collector.end(); i++) { 
        (*i)->print(cout);
        cout << endl;
    }

    Solution solution;
    bool found = solve(root,solution);
    cout << "found=" << found << endl;
    for (Solution::const_iterator i=solution.begin(); i!=solution.end(); i++) {
        cout << (*i) << " ";
    }
    cout << endl;

	return 0;
}


