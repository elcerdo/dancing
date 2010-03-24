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

    int get_number_of_ones() const {
        int count = 0;
        for (int k=0; k<size; k++) if (data[k]) count++;
        return count;
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
protected:
	bool *data;
};


std::ostream &operator<<(std::ostream &os,const Array &array) {
    os << " ";
    for (int j=0; j<array.width; j++) { os << static_cast<char>('a'+j); }
    os << endl;

    for (int i=0; i<array.height; i++) {
        os << static_cast<char>('0'+i);
        for (int j=0; j<array.width; j++) { os << array.get_value(i,j); }
        os << endl;
    }

    return os;
}


typedef std::string Id;

struct Node {
	Node(const Id &id, int data) : headertop(this), headerleft(this), left(this), right(this), top(this), down(this), id(id), data(data) {}

    void insert_right(Node *node) {
        Node *list_right = this->right;
        this->right = node;
        this->right->left = this;
        list_right->left = node;
        list_right->left->right = list_right;
        node->headerleft = this->headerleft;
    }

    void insert_left(Node *node) {
        Node *list_left = this->left;
        this->left = node;
        this->left->right = this;
        list_left->right = node;
        list_left->right->left = list_left;
        node->headerleft = this->headerleft;
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

    void fold_column() {
        assert(this->headertop->data != -1); //this is not the rows column
        Node *element = this->headertop;
        do {
            assert(element->right != NULL); //already folded;

            //find first left unfolded element
            Node *targeting = element->left;
            while (targeting->right == NULL) { targeting = targeting->left; }
            assert(targeting!=element);

            targeting->right = element->right;
            element->right = NULL;

            element = element->top;
        } while (element != this->headertop);
    }

    void unfold_column() {
        assert(this->headertop->data != -1); //this is not the rows column
        Node *element = this->headertop;
        do {
            assert(element->right == NULL); //not folded

            //find first left unfolded element
            Node *targeting = element->left;
            while (targeting->right == NULL) { targeting = targeting->left; }
            assert(targeting!=element);

            element->right = targeting->right;
            targeting->right = element;

            element = element->top;
        } while (element != this->headertop);
    }

    void fold_row() {
        assert(this->headerleft->data != -1); //this is not the columns row
        Node *element = this->headerleft;
        do {
            assert(element->down != NULL); //already folded

            //find first top unfolded element
            Node *targeting = element->top;
            while (targeting->down == NULL) { targeting = targeting->top; }
            assert(targeting!=element);

            targeting->down = element->down;
            element->down = NULL;

            element = element->left;
        } while (element != this->headerleft);
    }

    void unfold_row() {
        assert(this->headerleft->data != -1); //this is not the rows column
        Node *element = this->headerleft;
        do {
            assert(element->down == NULL); //not folded

            //find first top unfolded element
            Node *targeting = element->top;
            while (targeting->down == NULL) { targeting = targeting->top; }
            assert(targeting!=element);

            element->down = targeting->down;
            targeting->down = element;

            element = element->left;
        } while (element != this->headerleft);
    }

	Node *headertop,*headerleft;
	Node *left,*right;
	Node *top,*down;
	const Id id;
    const int data;
};

std::ostream &operator<<(std::ostream &os,const Node &node) {
    os << node.id << " " << node.data;
    os << " headerleft=" << (node.headerleft==&node ? "self" : node.headerleft->id);
    os << " left=" << (node.left==&node ? "self" : node.left->id);
    os << " right=" << (node.right==&node ? "self" : node.right->id);
    os << " headertop=" << (node.headertop==&node ? "self" : node.headertop->id);
    os << " top=" << (node.top==&node ? "self" : node.top->id);
    os << " down=" << (node.down==&node ? "self" : node.down->id);
    return os;
}


typedef std::list<Node*> Nodes;

Node *build_structure(const Array &array, Nodes &collector) {
    Node *root = new Node("root",-1);
    collector.push_back(root);

    { //create columns row
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

    { //create rows column
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

    //insert ones in the structure
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
        for (Node *element=column->down; element!=column; element=element->down) { length++; }

        cout << *column << " -> " << length << endl;

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

    for (Node *row=root->down; row!=root; row=row->down) {
        os << row->id[0];
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

    cout << "min column is " << *min_column << endl;

    return true;

    Node *row = min_column->down;
    while (row) {
        cout << "trying row " << *row << endl;

        row = row->down;
    }

    return true;
}

int main(int argc, char *argv[]) {
	Array array(7,6);
	if (not array.parse(cin)) { cerr << "error while parsing array\n"; return 1; }
    cout << array;

    Nodes collector;
    Node *root = build_structure(array,collector);
    cout << "collector has " << collector.size() << " nodes ";
    cout << "expected " << array.width << "+" << array.height << "+1+" << array.get_number_of_ones() <<"=" << (array.width+array.height+1+array.get_number_of_ones()) << endl;
    print_root(root,cout);

    { //folding tests
    cout << endl;
    Node *target0 = root->right->right->down->down;
    Node *target1 = root->right->right->right->down;

    print_root(root,cout);
    cout << "folding column " << target0->id[0] << endl;
    target0->fold_column();
    print_root(root,cout);
    cout << "folding row " << target0->id[1] << endl;
    target0->fold_row();
    print_root(root,cout);
    cout << "folding column " << target1->id[0] << endl;
    target1->fold_column();
    print_root(root,cout);
    cout << "folding row " << target1->id[1] << endl;
    target1->fold_row();
    print_root(root,cout);
    cout << "unfolding column " << target0->id[0] << endl;
    target0->unfold_column();
    print_root(root,cout);
    cout << "unfolding row " << target1->id[1] << endl;
    target1->unfold_row();
    print_root(root,cout);
    cout << "unfolding row " << target0->id[1] << endl;
    target0->unfold_row();
    print_root(root,cout);
    cout << "unfolding column " << target1->id[0] << endl;
    target1->unfold_column();
    print_root(root,cout);
    }

    Solution solution;
    cout << endl << "SOLVING" << endl;
    bool found = solve(root,solution);
    cout << "found=" << found << endl;
    for (Solution::const_iterator i=solution.begin(); i!=solution.end(); i++) {
        cout << *i << " ";
    }
    cout << endl;

	return 0;
}


