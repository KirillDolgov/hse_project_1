#include "node.hpp"
#include "func.hpp"

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

    return 0;
}