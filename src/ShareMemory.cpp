//
// Created by sen on 15-9-10.
//

#include <stddef.h>
#include <unistd.h>
#include "ShareMemory.h"

shm::ShareMemory::ShareMemory()
{
    setMemoryCreated(false);
    setMemoryAttach(false);
}

shm::ShareMemory::ShareMemory(shm::ShmKey key, shm::ShmSize size, ShmType type)
    :ShareMemory()
{
    switch(type)
    {
        case Create:
            create(key, size);
            break;
        case Open:
        default:
            open(key);
            break;
    }
}

shm::ShareMemory::~ShareMemory()
{
    free();
}

bool shm::ShareMemory::open(shm::ShmKey key)
{
    if(isMemoryCreated())
        free();
    setShmKey(key);
    if((_shmId = shmget(key, 0, IPC_CREAT | 0666)) != -1)
    {
        setMemoryCreated(true);
        return true;
    }
    return false;
}

bool shm::ShareMemory::create(shm::ShmKey key, shm::ShmSize size)
{
    if(isMemoryCreated())
        free();
    setShmKey(key);
    if((_shmId = shmget(key, size, IPC_CREAT | IPC_EXCL | 0666)) != -1)
    {
        setMemoryCreated(true);
        return true;
    }
    return false;
}

void *shm::ShareMemory::getAddress()
{
    void *addr = NULL;
    if(isMemoryCreated())
    {
        if(!isMemoryAttach())
        {
            _addr = shmat(getShmId(), NULL, 0);
            setMemoryAttach(true);
        }
        addr = _addr;
    }
    return addr;
}

void shm::ShareMemory::free()
{
    if(isMemoryAttach())
    {
        shmdt(getAddress());
        setMemoryAttach(false);
    }
    if(isMemoryCreated())
    {
        shmctl(getShmId(), IPC_RMID, 0);
        setMemoryCreated(false);
    }
}

bool shm::ShareMemory::isMemoryCreated() const
{
    return _isCreated;
}

void shm::ShareMemory::setMemoryCreated(bool state)
{
    _isCreated = state;
}

const shm::ShmKey &shm::ShareMemory::getShmKey() const
{
    return _memKey;
}

void shm::ShareMemory::setShmKey(shm::ShmKey key)
{
    _memKey = key;
}

void shm::ShareMemory::setMemoryAttach(bool state)
{
    _isAttached = state;
}

bool shm::ShareMemory::isMemoryAttach() const
{
    return _isAttached;
}

const shm::ShmId &shm::ShareMemory::getShmId() const
{
    return _shmId;
}

void shm::ShareMemory::setShmId(shm::ShmId id)
{
    _shmId = id;
}

shm::ShareMemory::ShareMemory(shm::ShmKey key)
    :ShareMemory(key, 0, ShareMemory::Open)
{

}

shm::ShareMemory::ShareMemory(shm::ShmKey key, shm::ShmSize size)
    :ShareMemory(key, size, ShareMemory::Create)
{

}

bool shm::ShareMemory::createRandom(shm::ShmSize size)
{
    char buf[2048];
    ShmKey key = ftok(getcwd(buf, sizeof(buf)), MAGICID);
    return create(key, size);
}
