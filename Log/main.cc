#include "Log.hpp"

int main()
{
    log.LogMessage(Debug, "this is a log message: %d, %lf\n", 123, 3.14);
    log.LogMessage(Info, "this is a log message: %d, %lf\n", 123, 3.14);
    log.LogMessage(Warning, "this is a log message: %d, %lf\n", 123, 3.14);
    return 0;
}
