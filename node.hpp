#ifndef NODE_H
#define NODE_H


class Node{
public:
    Node() = default;
    Node(unsigned char uch, int f) : ch(uch), freq(f) {}

    bool operator<(const Node& oth) const{
        return freq < oth.freq;
    }
    
    int get_freq() const {
        return freq;
    }
private:
    unsigned char ch;
    int freq;
};

class LowestPriority{
public:
    bool operator()(const Node& left, const Node& right) const{
        return left.get_freq() < right.get_freq();
    }
};

#endif
