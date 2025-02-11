#include "node.hpp"
#include <iostream>
#include <queue>
#include <vector>

using namespace std;





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


    priority_queue<Node, vector<Node>, LowestPriority> queue;
    for(int i = 0; i < 256; ++i){
        if(freq[i]!=0){
            Node node(i, freq[i]);
            queue.push(node);
        }
    }





    // for(int i = 0; i < 256; ++i){
    //     if(freq[i]!=0){
    //         cout << "[" << i << "] = " << freq[i] << endl; 
    //     }
    // }

}