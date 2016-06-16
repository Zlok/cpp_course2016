//
// Created by Appolinariya on 16.06.16.
//

#include <iostream>
#include <string>

#ifndef LIST_LIST_H
#define LIST_LIST_H

struct list {
private:
    struct node {
        std::string value;
        node *Next, *Prev;
    public:
        node();
        node(std::string v);
        node(std::string v, node * N, node * P);
        node(node const& tmp);
        node & operator=(node const& tmp);
        ~node();
    };
public:
    void display() const;
    list();
    list(list const& tmp);
    list & operator=(list const& tmp);
    void push_back(std::string const& v);
//    void pop_back();
private:
    node *end_node;
};

#endif //LIST_LIST_H
