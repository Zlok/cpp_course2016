//
// Created by Appolinariya on 16.06.16.
//

#include "list.h"

list::node::node() : value(""), Next(NULL), Prev(NULL) {
}

list::node::node(std::string v) : value(v), Next(NULL), Prev(NULL) {
}

list::node::node(std::string v, node *N, node *P) : value(v), Next(N), Prev(P) {
}

list::node::node(node const &tmp) : value(tmp.value), Next(tmp.Next), Prev(tmp.Prev) {
}

list::node::~node() {
    delete(Next);
    delete(Prev);
}

list::node& list::node::operator=(node const &tmp) {
    value = tmp.value;
    Next = tmp.Next;
    Prev = tmp.Prev;
    return *this;
}

list::list() : end_node() {
}

list::list(list const &tmp) {
    list();
    node * st = tmp.end_node->Next;
    node * fn = tmp.end_node->Prev;
    while (st != fn) {
        this->push_back(st->value);
        st = st->Next;
    }
    this->push_back(st->value);
}

list& list::operator=(list const &tmp) {
    list();
    if (tmp.end_node->Prev != NULL) {
        node *st = tmp.end_node->Next;
        node *fn = tmp.end_node->Prev;
        while (st != fn) {
            this->push_back(st->value);
            st = st->Next;
        }
        this->push_back(st->value);
    }
    return *this;
}

void list::push_back(std::string const &v) {
    if (end_node == NULL) {
        node *tmp = new node(v, end_node, end_node);
        end_node = new node("", tmp, tmp);
    } else {
        node *tmp = new node(v, end_node, end_node->Prev);
        end_node->Prev->Next = tmp;
        end_node->Prev = tmp;
    }
}

void list::display() const {
    if (end_node == NULL)
        std::cout << "Empty list\n";
    else {
        node *L = end_node->Next;
        while (L != end_node) {
            std::cout << L->value << (L->Next == end_node ? "\n" : ", ");
            L = L->Next;
        }
    }
}
