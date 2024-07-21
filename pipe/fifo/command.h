#pragma once

#include <iostream>
#include <cerrno>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define FILENAME ".fifo"