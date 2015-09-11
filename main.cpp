#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string.h>
#ifndef _WIN32
#include <unistd.h>
#endif
#include "src/ShareMemory.h"

using namespace std;
using namespace shm;

void child(ShareMemory &memory)
{
    char *ptr = memory.getAddress<char>();
    strcpy(ptr, "Hello world");
}

int main(int argc, char *args[])
{
    ShareMemory memory;
    if(argc == 1)
        cout << "create:" << memory.create(0x1234, 1024) << endl;
    else
        cout << "open:" << memory.open(0x1234) << endl;
    char *ptr = memory.getAddress<char>();
    cout << "memory size:" << memory.getShmSize() << endl;
    if(argc == 1)
    {
        strcpy(ptr, "Hello world");
        Sleep(30000);
    }
    else
    {
        if(ptr != NULL)
            cout << ptr << endl;
    }
    return 0;
}