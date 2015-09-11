//
// Created by sen on 15-9-10.
//

#include "ShareMemory.h"

shm::ShareMemory::ShareMemory()
{
    setMemoryCreated(false);
    setMemoryAttach(false);
    setMemorySize(0);
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
#ifndef _WIN32
    if((_shmId = shmget(key, 0, IPC_CREAT | 0666)) != -1)
#else
    char buf[20];
    sprintf(buf, "%d", key);
    HANDLE id = OpenFileMappingA(FILE_MAP_ALL_ACCESS, false, buf);
    _shmId = (ShmId)id;
    if(id != NULL)
#endif
    {
        setMemoryCreated(true);
        setMemorySize(0);
        return true;
    }
    return false;
}

bool shm::ShareMemory::create(shm::ShmKey key, shm::ShmSize size)
{
    if(isMemoryCreated())
        free();
    setShmKey(key);
#ifndef _WIN32
    if((_shmId = shmget(key, size, IPC_CREAT | IPC_EXCL | 0666)) != -1)
#else
    char buf[20];
    sprintf(buf, "%d", key);
    HANDLE id = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, size, buf);
    _shmId = (ShmId)id;
    if(id != NULL)
#endif
    {
        setMemoryCreated(true);
        setMemorySize(size);
        return true;
    }
    return false;
}

const shm::ShmSize &shm::ShareMemory::getShmSize() const
{
    return _memSize;
}

void shm::ShareMemory::setMemorySize(shm::ShmSize size)
{
    _memSize = size;
}

void *shm::ShareMemory::getAddress()
{
    void *addr = NULL;
    if(isMemoryCreated())
    {
        if(!isMemoryAttach())
        {
#ifndef _WIN32
            _addr = shmat(getShmId(), NULL, 0);
#else
            _addr = MapViewOfFile((HANDLE)getShmId(), FILE_MAP_ALL_ACCESS, 0, 0, getShmSize());
#endif
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
#ifndef _WIN32
        shmdt(getAddress());
#else
        UnmapViewOfFile(getAddress());
#endif
        setMemoryAttach(false);
    }
    if(isMemoryCreated())
    {
#ifndef _WIN32
        shmctl(getShmId(), IPC_RMID, 0);
#else
        CloseHandle((HANDLE)getShmId());
#endif
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
    ShmKey key = 0;
#ifndef _WIN32
    key = ftok(getcwd(buf, sizeof(buf)), MAGICID);
#else
    DWORD count = GetCurrentDirectoryA(sizeof(buf), buf);
    for(int i = 0; i < (signed)count; ++i)
        key += ((i % 2 == 0) ? buf[i] : buf[i] << 1);
    key += MAGICID;
    if(key < 0)
        key *= -1;
    else if(key == 0)
        key = MAGICID;
#endif
    return create(key, size);
}
