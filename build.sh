g++ -shared -fpic app1.cpp -o app1.so
#g++ -shared -fpic app2.cpp -o app2.so
g++ -g main.cpp -o main -ldl
