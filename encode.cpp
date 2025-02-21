#include "node.hpp"
#include "func.cpp"

int main(){
    string frname;
    cout << "Введите имя файла: ";
    cin >> frname;

    clock_t startTime = clock();

    vector<int> freq(256, 0);

    FILE* fr = fopen(frname.c_str(), "rb");
    if (!fr) {
        cerr << "Ошибка: не удалось открыть файл.\n";
        return 1;
    }

    int ch, k = 0;
    while ((ch = fgetc(fr)) != EOF) {
        freq[(uchar)ch]++;
        k = 1;
    }
    fclose(fr);

    if (k == 0) {
        cerr << "Ошибка: файл пустой.\n";
        return 1;
    }

    queue_t queue;

    for(int i = 0; i < 256; ++i){
        if(freq[i]!=0){
            Node::pointer node = make_shared<Node>(i, freq[i]);
            queue.push(node);
        }
    }

    if (queue.size() == 1) {
    Node::pointer root = queue.top();
    queue.pop();
    Node::pointer new_root = make_shared<Node>(0, root->get_freq());
    new_root->left = root;
    root->parent = new_root;
    queue.push(new_root);
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

    cout << "Заняло времени: " << double( clock() - startTime ) / (double)CLOCKS_PER_SEC<< " секунд" << endl;

    return 0;
}