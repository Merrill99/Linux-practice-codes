#include "process.h"

const char* symbol = "|/-\\";

void process_v1()
{
    // version1
    int rate = 0;

    int len = strlen(symbol);

    char bar[SIZE] = { 0 };
    //char bar[MAX_RATE];
    //memset(bar, '\0', sizeof(bar));
    while (rate <= MAX_RATE)
    {
        printf("[%-100s][%d%%][%c]\r", bar, rate, symbol[rate % len]);
        fflush(stdout);
        usleep(STIME);
        
        bar[rate++] = STYLE;
    }
    printf("\n");
}

void process_v2(int rate)
{

   int len = strlen(symbol);

    static char bar[SIZE] = { 0 };
    if (rate <= MAX_RATE && rate >= 0)
    {
        printf("[%-100s][%d%%][%c]\r", bar, rate, symbol[rate % len]);
        fflush(stdout);
        bar[rate] = STYLE;
    }
    if (rate == MAX_RATE) // 进度条已满清空进度条
    {
        memset(bar, '\0', sizeof(bar));
    }
}

void process_v3(double rate)
{

   int len = strlen(symbol);

    static char bar[SIZE] = { 0 };
    if (rate <= MAX_RATE && rate >= 0)
    {
        printf("[%-100s][%.1lf%%][%c]\r", bar, rate, symbol[(int)rate % len]);
        fflush(stdout);
        bar[(int)rate] = STYLE;
    }
    if (rate == MAX_RATE) // 进度条已满清空进度条
    {
        memset(bar, '\0', sizeof(bar));
    }
}
