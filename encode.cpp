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

void make_code(Node::pointer& node, string str, vector<string>& codes){
    if(node->left != nullptr){
        make_code(node->left, str + "0", codes);
    }
    if(node->right != nullptr){
        make_code(node->right, str + "1", codes);
    }

    if(node->left == nullptr && node->right == nullptr){
        node->code(str);
        codes[node->get_byte()] = str;
        // cout << "leaf: " << (int)node->get_byte() << " code: " << node->code() << "\n";
    }
}


string message2code(const string& filename, const vector<string>& codes) {
    FILE* file = fopen(filename.c_str(), "rb");
    if (!file) {
        cerr << "Ошибка: не удалось открыть файл.\n";
        return "";
    }

    string encoded_string {""};
    int ch;
    while ((ch = fgetc(file)) != EOF) {
        encoded_string += codes[(unsigned char)ch];
    }
    
    fclose(file);
    return encoded_string;
}


void write_file(const string& filename, vector<int>& freq, const queue_t& queue, const string& message){
    string new_filename = filename + ".hfc";
    ofstream ofs(new_filename, ofstream::binary);
    if(!ofs){
        return;
    }

    double count = count_if(freq.begin(), freq.end(), [](const double& value) { return (value != 0); });
    cout << "Count: " << (double)count << endl;

    ofs.write(reinterpret_cast<char*>(&count), sizeof count);

    for_each(freq.begin(), freq.end(), [index=uchar(0), &ofs](int& value) mutable {
        if(value != 0){
            ofs.write(reinterpret_cast<char*>(&index), sizeof index);
            ofs.write(reinterpret_cast<char*>(&value), sizeof value);
        }
        ++index;
    });

    int byte_count = message.size() / 8;
    unsigned char modulo = message.size() % 8;
    ofs.write(reinterpret_cast<char*>(&byte_count), sizeof byte_count);
    ofs.write(reinterpret_cast<char*>(&modulo), sizeof modulo);

    int i = 0;
    for(; i<byte_count; ++i){
        bitset<CHAR_BIT> b(message.substr(i * CHAR_BIT, CHAR_BIT));
        unsigned char value = static_cast<unsigned char>(b.to_ulong());
        ofs.write(reinterpret_cast<char*>(&value), sizeof value);
    }
    if(modulo > 0){
        bitset<CHAR_BIT> b(message.substr(i * CHAR_BIT, modulo));
        unsigned char value = static_cast<unsigned char>(b.to_ulong());
        ofs.write(reinterpret_cast<char*>(&value), sizeof value);
    }

}




int main(){
    string frname;
    cout << "Введите имя файла: ";
    cin >> frname;

    vector<int> freq(256, 0);

    FILE* fr = fopen(frname.c_str(), "rb");
    if (!fr) {
        cerr << "Ошибка: не удалось открыть файл.\n";
        return 1;
    }

    int ch;
    while ((ch = fgetc(fr)) != EOF) {
        freq[(unsigned char)ch]++;
    }
    fclose(fr);


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

    vector<string> codes(256, "");
    Node::pointer root = queue.top();
    make_code(root, "", codes);



    string message = message2code(frname, codes);

    write_file(frname, freq, queue, message);



}