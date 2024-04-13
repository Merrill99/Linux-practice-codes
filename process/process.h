#include<stdio.h>

#include <unistd.h>
#include <string.h>

#define MAX_RATE 100
#define SIZE 101
#define STIME 1000*40
#define STYLE '-'

typedef void (*callback)(double);

void process_v1();

void process_v2(int);

void process_v3(double);
