#include <fstream>
#include <iostream>
#include <bitset>
#include <algorithm>
#include <time.h>

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
    //cout << "Уникальных символов: " << (double)count << endl;

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

void read_decoding_file(string filename, vector<int>& freq, string& message){
    string new_filename = filename + ".hfc";
    ifstream ifs(new_filename, ifstream::binary);
    if(!ifs){
        throw "Ошибка: этот файл не расширения .hfs.";
        return;
    }

    double count = 0;
    ifs.read(reinterpret_cast<char*>(&count), sizeof count);
    int i = 0;
    while(i < count){
        uchar ch;
        ifs.read(reinterpret_cast<char*>(&ch), sizeof ch);

        int f = 0;
        ifs.read(reinterpret_cast<char*>(&f), sizeof f);
        freq[ch] = f;
        ++i;
    }

    int byte_count = 0;
    int modulo = 0;
    uchar dick = 0;
    ifs.read(reinterpret_cast<char*>(&byte_count), sizeof byte_count);
    ifs.read(reinterpret_cast<char*>(&modulo), sizeof dick);
    if (modulo == 0 && byte_count == 0) {
        throw "Ошибка: файл слишком маленький.";
        return;
    }

    int l = 0;
    //cerr << byte_count << " - это int\n";
    //cerr << modulo << " - это char\n";
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