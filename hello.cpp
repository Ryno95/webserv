#include<stdio.h>
#include<unistd.h>

int main(int argc,char *argv[],char *envp[]){
    write(STDOUT_FILENO, "Hello World", 12);
    return 5;
}