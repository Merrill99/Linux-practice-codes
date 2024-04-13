#include "process.h"

#define LOAD_DATA 1024*1024 //1MB数据大小
#define LOAD_RATE 1024*2 // 下载速率

void download(callback cb)
{
   int data = LOAD_DATA;
   int load = 0;
   while (load < data)
   {
       cb((load*100)/data);
       usleep(LOAD_RATE); // 利用休眠时间模仿下载
       load += LOAD_RATE;
   }
   cb(MAX_RATE);
   printf("\n");
    
}

int main()
{
    //int rate = 0;
    //while (rate <= MAX_RATE)
    //{
    //    process_v2(rate++);
    //    usleep(100000); // 利用休眠时间模仿下载   
    //}
    //printf("\n");
    
    download(process_v3);

    return 0;
}
