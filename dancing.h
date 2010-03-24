#ifndef __DANCING_H__
#define __DANCING_H__

#include <ostream>
#include <list>

struct Node {
    typedef std::string Id;

	Node(const Id &id, int data);
    void insert_right(Node *node);
    void insert_left(Node *node);
    void insert_down(Node *node);
    void insert_top(Node *node);
    void fold_column();
    void unfold_column();
    void fold_row();
    void unfold_row();

	Node *headertop,*headerleft;
	Node *left,*right;
	Node *top,*down;
	const Id id;
    const int data;
};

void print_root(const Node *root, std::ostream &os);
std::ostream &operator<<(std::ostream &os,const Node &node);

struct SolveParams {
    typedef std::list<Node*> Nodes;
    typedef Nodes Solution;
    typedef std::list<Nodes> Solutions;

    SolveParams(Node *root,size_t max_solution);
    void print_indent(std::ostream &os) const;

    Node *root;
    Solution partial_solution;
    Solutions solutions;
    const size_t max_solution;
    int indent;
};

Node *find_minimum_column(Node *root);
void solve(SolveParams &params, std::ostream &log);

#endif
