#include <iostream>
#include <map>
#include <string>
#include <memory>
using namespace std;
map<int,shared_ptr<string>> ta;
void get(){
    auto b = make_shared<string>("safdasfsa");
    ta.insert(make_pair(1,b));

}
int main(){
    auto b = make_shared<char>('3');
    char *a = b.get();
    b.reset();
    cout<<*(int *)a<<endl;
    return 0;
}
