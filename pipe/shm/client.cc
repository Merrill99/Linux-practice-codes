#include "command.hpp"

int main()
{
    key_t key = GetKey();
    int shmid = GetShm(key);

    char *s = (char*)shmat(shmid, nullptr, 0); // 让系统帮我选择如何挂接

    sleep(5);

    cout << "attaching server successfully ! " << endl;
    
    shmdt(s);
    cout << "detach successfully !" << endl;
    return 0;
}