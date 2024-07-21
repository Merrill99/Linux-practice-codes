#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <sys/ipc.h> // Inter-Process Communication
#include <sys/shm.h>
#include <unistd.h>

using namespace std;

const string pathname = "/home/minnow/test/pipe/shm";
const int proj_id = 0x11223344;
const size_t size = 4096;

key_t GetKey()
{
    key_t key = ftok(pathname.c_str(), proj_id);
    if (key < 0)
    {
        cout << "errno:" << errno << ", errstring:" << strerror(errno) << endl;
        exit(1);
    }
    return key;
}

string ToHex(int id)
{
    char buf[1024];
    snprintf(buf, sizeof(buf), "0x%x", id);
    return buf;
}

int CreatShmHelper(key_t key, int flag)
{
    int shmid = shmget(key, size, flag);
    if (shmid < 0)
    {
        cout << "errno:" << errno << ", errstring:" << strerror(errno) << endl;
        exit(2);
    }
    return key;
}

int CreatShm(key_t key)
{
    return CreatShmHelper(key, IPC_CREAT | IPC_EXCL | 0644);
}

int GetShm(key_t key)
{
    return CreatShmHelper(key, IPC_CREAT/*0也可以！*/);
}