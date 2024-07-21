#include "command.h"

using namespace std;

int main()
{
    int wfd = open(FILENAME, O_WRONLY);
    // 打开失败
    if (wfd < 0)
    {
        cerr << "errno: " << errno << ",strerror: " << strerror(errno) << endl;
        return 1;
    }

    cout << "open with writing success..." << endl;

    string message;
    while (true)
    {
        cout << "Please enter# ";
        getline(cin, message);

        ssize_t s = write(wfd, message.c_str(), message.size());
        if (s < 0)
        {
            cerr << "errno: " << errno << ",strerror: " << strerror(errno) << endl;
            break;
        }
    }

    close(wfd);
    cout << "close success..." << endl;
    return 0;
}