#include "node.hpp"
#include <iostream>
#include <queue>
#include <vector>

using namespace std;


void make_code(Node::pointer& node, string str, vector<string>& codes){
    if(node->left != nullptr){
        make_code(node->left, str + "0", codes);
    }
    if(node->right != nullptr){
        make_code(node->left, str + "1", codes);
    }

    if(node->left == nullptr && node->right == nullptr){
        node->code(str);
        codes[node->get_byte()] = str;
        cout << "leaf: " << node << " code: " << node->code() << "\n";
    }
}


int main(){
    string frname;
    cout << "Введите имя файла: ";
    cin >> frname;

    long freq[256] = {0};

    FILE* fr = fopen(frname.c_str(), "rb");
    if(!fr){
        return 1;
    }
    fseek(fr, 0L, SEEK_END);
    long length = ftell(fr);
    fseek(fr, 0, SEEK_SET);
    for(int i = 0; i < length; ++i)
    {
        freq[(unsigned char)fgetc(fr)] ++;
    }
    fclose(fr);


    using queue_t = priority_queue<Node::pointer, vector<Node::pointer>, LowestPriority>;
    queue_t queue;

    for(int i = 0; i < 256; ++i){
        if(freq[i]!=0){
            Node::pointer node = make_shared<Node>(i, freq[i]);
            queue.push(node);
        }
    }


    while(queue.size() > 1)
    {
    Node::pointer x = queue.top();
    queue.pop();
    Node::pointer y = queue.top();
    queue.pop();
    Node::pointer z = make_shared<Node>(0, x->get_freq()+y->get_freq());
    z->left = x;
    z->right = y;
    x->parent = z;
    y->parent = z;
    queue.push(z);
    } 

    vector<string> codes(0x100, "");
    Node::pointer root = queue.top();
    make_code(root, "", codes);

    // for(int i = 0; i < 256; ++i){
    //     if(freq[i]!=0){
    //         cout << "[" << i << "] = " << freq[i] << endl; 
    //     }
    // }

}