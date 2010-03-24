#include "dancing.h"

#include <cassert>
#include <iostream>
#include <algorithm>

using std::endl;

Node::Node(const Id &id, int data) : headertop(this), headerleft(this), left(this), right(this), top(this), down(this), id(id), data(data) {}

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

void Node::unfold_column() {
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

void Node::fold_row() {
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

void Node::unfold_row() {
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


SolveParams::SolveParams(Node *root,size_t max_solution) : root(root), max_solution(max_solution), indent(0) {}
void SolveParams::print_indent(std::ostream &os) const {
    for (int k=0; k<indent; k++) { os << "-"; };
}

Node *find_minimum_column(Node *root) {
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

void solve(SolveParams &params, std::ostream &log) {
    Node *root = params.root;
    print_root(root,log);

    if (root->right == root and root->down == root) {
        params.print_indent(log);
        log << "found solution ";
        for (SolveParams::Solution::const_iterator i=params.partial_solution.begin(); i!=params.partial_solution.end(); i++) { log << (*i)->id << " "; }
        log << endl;
        params.solutions.push_back(params.partial_solution);
        return;
    }

    Node *min_column = find_minimum_column(root);
    if (not min_column) {
        params.print_indent(log);
        log << "no min column" << endl;
        return;
    }

    params.print_indent(log);
    log << "min column is " << min_column->id[0] << endl;
    for (Node *selected=min_column->down; selected!=min_column; selected=selected->down) {
        params.print_indent(log);
        log << "selected row " << selected->headerleft->id[0] << endl;

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

        params.partial_solution.push_back(selected->headerleft);
        params.indent++;
        for (SolveParams::Nodes::iterator i=folded_columns.begin(); i!=folded_columns.end(); i++) { (*i)->fold_column(); }
        for (SolveParams::Nodes::iterator i=folded_rows.begin(); i!=folded_rows.end(); i++) { (*i)->fold_row(); }
        solve(params,log);
        for (SolveParams::Nodes::iterator i=folded_columns.begin(); i!=folded_columns.end(); i++) { (*i)->unfold_column(); }
        for (SolveParams::Nodes::iterator i=folded_rows.begin(); i!=folded_rows.end(); i++) { (*i)->unfold_row(); }
        params.partial_solution.pop_back();
        params.indent--;

        if (params.solutions.size() >= params.max_solution) return;
    }
}

void print_root(const Node *root, std::ostream &os) {
    if (root->right == root and root->down == root) {
        os << "null matrix" << endl;
        return;
    }

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

