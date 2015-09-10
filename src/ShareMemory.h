//
// Created by sen on 15-9-10.
//

#ifndef SHAREMEMORY_SHAREMEMORY_H
#define SHAREMEMORY_SHAREMEMORY_H

#include <sys/ipc.h>
#include <sys/shm.h>
#define DEBUG
#ifdef DEBUG
#include <iostream>
using namespace std;
#endif
#ifndef MAGICID
#define MAGICID 736506
#endif

namespace shm
{
    using ShmKey = key_t;
    using ShmId = int;
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

    private:
        ShmKey _memKey;
        ShmId _shmId;
        bool _isCreated;
        bool _isAttached;
        void *_addr;
    };
}


#endif //SHAREMEMORY_SHAREMEMORY_H
