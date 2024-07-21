#include "command.hpp"

int main()
{
    key_t key = GetKey();
    cout << "key:" << ToHex(key) << endl;
    
    int shmid = CreatShm(key);
    sleep(5);
    cout << "shmid:" << shmid << endl;
    cout << "attaching... " << endl;
    
    char *s = (char*)shmat(shmid, nullptr, 0);
    sleep(5);

    shmdt(s);

    sleep(5);

    shmctl(shmid, IPC_RMID, nullptr);

    return 0;
}