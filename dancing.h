#ifndef __DANCING_H__
#define __DANCING_H__

#include <ostream>
#include <list>

struct Node {
    typedef std::string Id;
    typedef std::list<Node*> Collector;
    enum Type {ROOT,MOVE,CONSTRAINT,LINK};

    static Node *create_root(Collector &collector);
    static Node *add_constraint(Node *root, Collector &collector,const Id &id);
    static Node *add_move(Node *root, Collector &collector, const Id &id);
    static Node *add_link(Node *move, Node *constraint, Collector &collector, const Id &id);
    static void print_root(const Node *root, std::ostream &os, bool verbose);
    static Node *find_move(Node *root,const Node::Id &id);
	static int get_number_of_moves(const Node *root);
	static int get_number_of_constraints(const Node *root);

    void fold_column();
    void unfold_column();
    void fold_row();
    void unfold_row();

	Node *headertop,*headerleft;
    Node *right,*down;

    const Id &get_id() const;
    const Type &get_type() const;

protected:
	Node(const Id &id, Type type);

    void insert_right(Node *node);
    void insert_left(Node *node);
    void insert_down(Node *node);
    void insert_top(Node *node);

	Node *left,*top;

	const Id id;
    const Type type;
};

std::ostream &operator<<(std::ostream &os,const Node &node);
void delete_collector(Node::Collector &collector);

struct SolveParams {
    typedef std::list<Node*> Nodes;
    typedef Nodes Solution;
    typedef std::list<Nodes> Solutions;

    SolveParams(Node *root,size_t max_solution);
    void solve(std::ostream &log, bool verbose);
    void play_move(Node *move);

    Node *root;
    Solutions solutions;
    const size_t max_solution;

protected:
    void print_indent(std::ostream &os) const;

    Solution partial_solution;
    int indent;
};



#endif
