#include "node.hpp"
#include "func.cpp"

int main(){
    
    string frname;
    cout << "Введите имя файла без \".hfs\": ";
    cin >> frname;

    FILE* fr = fopen(frname.c_str(), "rb");
    if (!fr) {
        cerr << "Ошибка: не удалось открыть файл.\n";
        return 1;
    }

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