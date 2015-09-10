#include <iostream>
#include <string.h>
#include <unistd.h>
#include "src/ShareMemory.h"

using namespace std;
using namespace shm;

void child(ShareMemory &memory)
{
    char *ptr = memory.getAddress<char>();
    strcpy(ptr, "Hello world");
}

int main()
{
    ShareMemory memory2(0x1234);
    memory2.createRandom(1024);
    if(fork() == 0)
    {
        child(memory2);
        exit(0);
    }
    sleep(1);
    char *ptr = memory2.getAddress<char>();
    cout << ptr << endl;

    return 0;
}