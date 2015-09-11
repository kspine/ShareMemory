//
// Created by sen on 15-9-10.
//

#ifndef SHAREMEMORY_SHAREMEMORY_H
#define SHAREMEMORY_SHAREMEMORY_H

#ifndef _WIN32
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#else
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#endif

#include <stddef.h>
#include <stdio.h>

#ifdef DEBUG
#include <iostream>
using namespace std;
#endif
#ifndef MAGICID
#define MAGICID 736506
#endif

namespace shm
{
#ifndef _WIN32
    using ShmKey = key_t;
#else
    using ShmKey = long;
#endif
    using ShmId = long;
    using ShmSize = size_t;

    class ShareMemory
    {
    public:
        enum ShmType {Open, Create};

        ShareMemory();
        ShareMemory(ShmKey key);
        ShareMemory(ShmKey key, ShmSize size);
        ShareMemory(ShmKey key, ShmSize size, ShmType type);
        ~ShareMemory();

    public:
        bool open(ShmKey key);
        bool create(ShmKey key, ShmSize size);
        bool createRandom(ShmSize size);
        void *getAddress();
        void free();
        const ShmKey &getShmKey() const;
        const ShmId &getShmId() const;
        const ShmSize &getShmSize() const;
        bool isMemoryCreated() const;
        bool isMemoryAttach() const;

    public:
        template<typename Object>
        Object *getAddress()
        {
            auto addr = getAddress();
            if (addr == NULL)
                return NULL;
            return static_cast<Object *>(addr);
        }

    protected:
        void setMemoryCreated(bool state);
        void setMemoryAttach(bool state);
        void setShmKey(ShmKey key);
        void setShmId(ShmId id);
        void setMemorySize(ShmSize size);

    private:
        ShmKey _memKey;
        ShmId _shmId;
        bool _isCreated;
        bool _isAttached;
        void *_addr;
        ShmSize _memSize;
    };
}


#endif //SHAREMEMORY_SHAREMEMORY_H
