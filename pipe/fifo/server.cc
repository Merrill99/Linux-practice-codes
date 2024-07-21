#include "command.h"

using namespace std;

int main()
{
    int n = mkfifo(FILENAME, 0666);
    if (n < 0)
    {
        cerr << "errno: " << errno << ",strerror: " << strerror(errno) << endl;
        return 1;
    }

    cout << "mkfifo success..." << endl;

    int rfd = open(FILENAME, O_RDONLY);
    if (rfd < 0)
    {
        cerr << "errno: " << errno << ",strerror: " << strerror(errno) << endl;
        return 2;
    }

    cout << "open with reading success..." << endl;

    char buffer[1024];
    while (true)
    {
        ssize_t s = read(rfd, buffer, sizeof(buffer) - 1);
        if (s > 0)
        {
            buffer[s] = 0; // 将字符串最后一位置为'\0'
            cout << "Client say# " << buffer << endl;
        }
        else if (s == 0)
        {
            cout << "client quit, and server quit too..." << endl;
            
        }
    }

    close(rfd);
    cout << "close success..." << endl;

    return 0;
}