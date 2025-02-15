#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <memory>
using namespace std;

class Node{
public:
    using pointer = shared_ptr<Node>;

    pointer left {nullptr};
    pointer right {nullptr};
    pointer parent {nullptr};



    Node() = default;
    Node(unsigned char uch, int f) : ch(uch), freq(f) {}

    bool operator<(const Node& oth) const{
        return freq < oth.freq;
    }
    
    int get_freq() const {
        return freq;
    }

    void set_freq(int f){
        freq = f;
    }

    string code() const{
        return code_string;
    }

    void code(const string& c) {
        code_string = c;
    }

    unsigned char get_byte() const {
        return ch;
    }


private:
    unsigned char ch {0};
    int freq {0};
    string code_string {""};
};

class LowestPriority{
public:
    bool operator()(const Node::pointer& left, const Node::pointer& right) const{
        return left->get_freq() > right->get_freq();
    }
};

#endif
