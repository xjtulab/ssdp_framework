#include <iostream>
#include <map>
#include <string>
#include <memory>
using namespace std;


class counter{
    private:
    int* target;
    int count;
    friend class shared_int;
    public:
    counter(int* a){
        target = a;
        count = 1;
    }
    ~counter(){
        delete target;
    }
};

class shared_int{
    private:
    counter* ptr;
    public:
    shared_int(int *a){
        cout<<"0"<<endl;
        ptr = new counter(a);
    }
    shared_int& operator=( shared_int& tar){
        cout<<"2"<<endl;
        ptr->count--;
        if (ptr->count == 0){
            delete ptr;
        }
        ptr = tar.ptr;
        ptr->count++;
        return *this;
    }
    shared_int(const shared_int& tar){
        cout<<"1"<<endl;
        ptr=tar.ptr;
        ptr->count++;
    }
    int use_count(){
        return ptr->count;
    }
    ~shared_int(){
        ptr->count--;
        if(ptr->count == 0){
            delete ptr;
        }
    }
};



int  main(){
    int* k = new int(42);
    shared_int t = k;

    //shared_int a = t;
    //t = a;
    //cout<<a.use_count()<<endl;
    return 0;
  }
