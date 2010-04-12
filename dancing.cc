#include "dancing.h"

#include <cassert>
#include <iostream>
#include <algorithm>

using std::endl;

Node *Node::create_root(Node::Collector &collector) {
    Node *root = new Node("root",Node::ROOT);
    collector.push_back(root);
    return root;
}

Node *Node::add_constraint(Node *root, Node::Collector &collector, const Node::Id &id) {
    assert(root->type == ROOT);
    Node *constraint = new Node(id,Node::CONSTRAINT);
    root->insert_left(constraint);
    collector.push_back(constraint);
    return constraint;
}

Node *Node::add_move(Node *root, Node::Collector &collector, const Node::Id &id) {
    assert(root->type == ROOT);
    Node *move = new Node(id,Node::MOVE);
    root->insert_top(move);
    collector.push_back(move);
    return move;
}

Node *Node::add_link(Node *move, Node *constraint, Node::Collector &collector, const Node::Id &id) {
    assert(constraint->type == CONSTRAINT);
    assert(move->type == MOVE);
    Node *link = new Node(id,Node::LINK);
    constraint->insert_top(link);
    move->insert_left(link);
    collector.push_back(link);
    return link;
}

Node::Node(const Id &id, Node::Type type) : headertop(this), headerleft(this), right(this), down(this), left(this), top(this), id(id), type(type) {}

const Node::Id &Node::get_id() const { return id; }
const Node::Type &Node::get_type() const { return type; }

void Node::insert_right(Node *node) {
    Node *list_right = this->right;
    this->right = node;
    this->right->left = this;
    list_right->left = node;
    list_right->left->right = list_right;
    node->headerleft = this->headerleft;
}

void Node::insert_left(Node *node) {
    Node *list_left = this->left;
    this->left = node;
    this->left->right = this;
    list_left->right = node;
    list_left->right->left = list_left;
    node->headerleft = this->headerleft;
}

void Node::insert_down(Node *node) {
    Node *list_down = this->down;
    this->down = node;
    this->down->top = this;
    list_down->top = node;
    list_down->top->down = list_down;
    node->headertop = this->headertop;
}

void Node::insert_top(Node *node) {
    Node *list_top = this->top;
    this->top = node;
    this->top->down = this;
    list_top->down = node;
    list_top->down->top = list_top;
    node->headertop = this->headertop;
}

void Node::fold_column() {
    assert(this->type == LINK or this->type == CONSTRAINT); //this is not the rows column
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

void Node::unfold_column() {
    assert(this->type == LINK or this->type == CONSTRAINT); //this is not the rows column
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

void Node::fold_row() {
    assert(this->type == LINK or this->type == MOVE); //this is not the columns row
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

void Node::unfold_row() {
    assert(this->type == LINK or this->type == MOVE); //this is not the columns row
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

std::ostream &operator<<(std::ostream &os,const Node &node) {
    switch (node.get_type()) {
    case Node::ROOT:
        os << "ROOT";
        break;
    case Node::CONSTRAINT:
        os << "CONSTRAINT";
        break;
    case Node::MOVE:
        os << "MOVE";
        break;
    case Node::LINK:
        os << "LINK";
        break;
    }
    os << " ";
    os << node.get_id(); // << " ";
    //os << " headerleft=" << (node.headerleft==&node ? "self" : node.headerleft->get_id());
    //os << " left=" << (node.left==&node ? "self" : node.left->get_id());
    //os << " right=" << (node.right==&node ? "self" : node.right->get_id());
    //os << " headertop=" << (node.headertop==&node ? "self" : node.headertop->get_id());
    //os << " top=" << (node.top==&node ? "self" : node.top->get_id());
    //os << " down=" << (node.down==&node ? "self" : node.down->get_id());
    return os;
}

void print_root(const Node *root, std::ostream &os, bool verbose) {
    assert(root->get_type() == Node::ROOT);

    int possible_count = 0;
    for (Node *row=root->down; row!=root; row=row->down) { possible_count++; }
    os << "root has " << possible_count << " possible moves" << endl;
    for (Node *row=root->down; row!=root and verbose; row=row->down) {
        os << *row << " -> ";
        for (Node *element=row->right; element!=row; element=element->right) { os << *element->headertop << " "; }
        os << endl;
    }

    int requirement_count = 0;
    for (Node *column=root->right; column!=root; column=column->right) { requirement_count++; }
    os << "root has " << requirement_count << " requirements" << endl;
    for (Node *column=root->right; column!=root and verbose; column=column->right) { os << *column << endl; }
}


SolveParams::SolveParams(Node *root,size_t max_solution) : root(root), max_solution(max_solution), indent(0) {
    assert(root->get_type() == Node::ROOT);
}

void SolveParams::print_indent(std::ostream &os) const {
    for (int k=0; k<indent; k++) { os << "-"; };
}

static Node *find_minimum_column(Node *root) {
    Node *column_min = NULL;
    int length_min = 0;

    for (Node *column=root->right; column!=root; column=column->right) {
        int length = 0;
        for (Node *element=column->down; element!=column; element=element->down) { length++; }

        if (length == 0) return NULL;

        if (column_min == NULL || length < length_min) {
            length_min = length;
            column_min = column;
        }
    }

    return column_min;
}

void SolveParams::solve(std::ostream &log, bool verbose) {
    assert(root->get_type() == Node::ROOT);

    if (root->right == root and root->down == root) {
        solutions.push_back(partial_solution);
        if (verbose) {
            print_indent(log);
            log << "found solution ";
            for (SolveParams::Solution::const_iterator i=partial_solution.begin(); i!=partial_solution.end(); i++) { log << **i << " "; }
            log << endl;
        }
        log << (max_solution-solutions.size()) << " remaining" << endl;
        return;
    }

    Node *min_column = find_minimum_column(root);
    if (not min_column) {
        if (verbose) {
            print_indent(log);
            log << "no min column" << endl;
        }
        return;
    }

    if (verbose) {
        print_indent(log);
        log << "min column is " << *min_column << endl;
    }

    for (Node *selected=min_column->down; selected!=min_column; selected=selected->down) {
        if (verbose) {
            print_indent(log);
            log << "selected row " << *selected->headerleft << endl;
        }

        SolveParams::Nodes folded_columns;
        SolveParams::Nodes folded_rows;
        for (Node *element_row=selected->headerleft->right; element_row!=selected->headerleft; element_row=element_row->right) {
            Node *column = element_row->headertop;
            folded_columns.push_back(column);
            for (Node *element_column=column->down; element_column!=column; element_column=element_column->down) {
                Node *row = element_column->headerleft;
                bool row_foldable = (std::find(folded_rows.begin(),folded_rows.end(),row) == folded_rows.end());
                if (row_foldable) folded_rows.push_back(row);
            }

        }

        partial_solution.push_back(selected->headerleft);
        indent++;
        for (SolveParams::Nodes::iterator i=folded_columns.begin(); i!=folded_columns.end(); i++) { (*i)->fold_column(); }
        for (SolveParams::Nodes::iterator i=folded_rows.begin(); i!=folded_rows.end(); i++) { (*i)->fold_row(); }
        solve(log,verbose);
        for (SolveParams::Nodes::iterator i=folded_columns.begin(); i!=folded_columns.end(); i++) { (*i)->unfold_column(); }
        for (SolveParams::Nodes::iterator i=folded_rows.begin(); i!=folded_rows.end(); i++) { (*i)->unfold_row(); }
        partial_solution.pop_back();
        indent--;

        if (solutions.size() >= max_solution) return;
    }
}

void SolveParams::play_move(Node *move) {
    assert(move->get_type() == Node::MOVE);

    SolveParams::Nodes folded_columns;
    SolveParams::Nodes folded_rows;
    for (Node *element_row=move->right; element_row!=move; element_row=element_row->right) {
        Node *column = element_row->headertop;
        folded_columns.push_back(column);
        for (Node *element_column=column->down; element_column!=column; element_column=element_column->down) {
            Node *aarow = element_column->headerleft;
            bool row_foldable = (std::find(folded_rows.begin(),folded_rows.end(),aarow) == folded_rows.end());
            if (row_foldable) folded_rows.push_back(aarow);
        }

    }

    partial_solution.push_back(move);
    for (SolveParams::Nodes::iterator i=folded_columns.begin(); i!=folded_columns.end(); i++) { (*i)->fold_column(); }
    for (SolveParams::Nodes::iterator i=folded_rows.begin(); i!=folded_rows.end(); i++) { (*i)->fold_row(); }
}

void delete_collector(Node::Collector &collector) {
    for (Node::Collector::iterator i=collector.begin(); i!=collector.end(); i++) { delete *i; }
}

