#include <iostream>

using namespace std;









int main(){
    string frname;
    cout << "Введите имя файла: ";
    cin >> frname;

    long freq[256];

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
    

}