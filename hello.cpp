#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<iostream>

int main(int argc,char *argv[],char *envp[]){
    std::cerr << "Return value of getenv(): " << getenv("QUERY_STRING") << std::endl;
    write(STDOUT_FILENO, "Hello World", 12);
    return 5;
}