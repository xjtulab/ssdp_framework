#include <iostream>

template<class t>
class newhandle{
    public:
    static int a;
    private:
    static int b;
};

template<class t>
int newhandle<t>::a =0;
template<class t>
int newhandle<t>::b = 1;

class x:public newhandle<x>{
    public:
    const int b = 0;
};


int main(){
    x* a = new x;
    return 0;
}
