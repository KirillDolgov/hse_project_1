#include "node.hpp"

#include <fstream>
#include <iostream>
#include <bitset>
#include <algorithm>

#include <queue>
#include <vector>

using namespace std;
using queue_t = priority_queue<Node::pointer, vector<Node::pointer>, LowestPriority>;
using uchar = unsigned char;

void read_decoding_file(string filename, vector<int>& freq, string& message){
    string new_filename = filename + ".hfc";
    ifstream ifs(new_filename, ifstream::binary);
    if(!ifs){
        return;
    }

    uchar count = 0;
    ifs.read(reinterpret_cast<char*>(&count), sizeof count);
    // cout << "Read count: " << (int)count << endl;
    int i = 0;
    while(i < count){
        uchar ch;
        ifs.read(reinterpret_cast<char*>(&ch), sizeof ch);


        int f = 0;
        ifs.read(reinterpret_cast<char*>(&f), sizeof f);
        freq[ch] = f;
        ++i;
    }

    // for_each(freq.begin(), freq.end(), [index = uchar(0)] (const int& value) mutable {
    //     if(value != 0){
    //         cout << 
    //     }
    // });

    int byte_count = 0;
    uchar modulo = 0;
    ifs.read(reinterpret_cast<char*>(&byte_count), sizeof byte_count);
    ifs.read(reinterpret_cast<char*>(&modulo), sizeof modulo);

    int l = 0;
    for(; l < byte_count; ++l){
        uchar byte;
        ifs.read(reinterpret_cast<char*>(&byte), sizeof byte);
        bitset<CHAR_BIT> b(byte);
        message += b.to_string();
    }
    if(modulo > 0){
        uchar byte;
        ifs.read(reinterpret_cast<char*>(&byte), sizeof byte);
        bitset<CHAR_BIT> b(byte);
        message += b.to_string().substr(CHAR_BIT - modulo, CHAR_BIT);
    }
}

void make_char(const Node::pointer& root, const string& message, string& text){
    Node::pointer node = root;
    for(size_t i = 0; i < message.size(); ++i){
        char ch = message[i];
        if(ch == '0'){
            if(node->left != nullptr){
                node = node->left;
                if(node->left == nullptr && node->right == nullptr){
                    text += node->get_byte();
                    node = root;
                }
            }
        } else if (ch == '1'){
            if(node->right != nullptr){
                node = node->right;
                if(node->left == nullptr && node->right == nullptr){
                    text += node->get_byte();
                    node = root;
                }
            }    
        }
        
    }

}

void write_decoding_file(const string& filename, const string& text){
    string new_filename = "decoded_" + filename;
    ofstream ofs(new_filename, ofstream::binary);
    if(!ofs){
        return;
    }

    ofs << text;

}





int main(){
    
    string frname;
    cout << "Введите имя файла без \".hfs\": ";
    cin >> frname;

    vector<int> freq2(256, 0);
    string message2 = "";

    read_decoding_file(frname, freq2, message2);



    queue_t queue2;
    
    for(int i = 0; i < 256; ++i){
        if(freq2[i]!=0){
            Node::pointer node = make_shared<Node>(i, freq2[i]);
            queue2.push(node);
        }
    }


    while(queue2.size() > 1)
    {
    Node::pointer x = queue2.top();
    queue2.pop();
    Node::pointer y = queue2.top();
    queue2.pop();
    Node::pointer z = make_shared<Node>(0, x->get_freq()+y->get_freq());
    z->left = x;
    z->right = y;
    x->parent = z;
    y->parent = z;
    queue2.push(z);
    } 

    
    Node::pointer root2 = queue2.top();
    string text = "";
    make_char(root2, message2, text);
    write_decoding_file(frname, text);

    return 0;
}