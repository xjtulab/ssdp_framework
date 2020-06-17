#include <iostream>
#include <map>
#include <string>
#include <memory>
using namespace std;
map<int,shared_ptr<string>> ta;
void get(string &a){
   string b = "sadfasf";
   a= b;
}
int main(){
    string c;
    get(c);
    cout<<c<<endl;
    return 0;
}
