#ifndef HEAD_FILE_H
#define HEAD_FILE_H
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <iostream>
#include <string>
#include <errno.h>
#include <typeinfo>
#include <cxxabi.h>
#include <thread>
#include <sstream>
#include <signal.h>
#include <future>
#include <initializer_list>
#include <type_traits>
#include <ctime>
#include <atomic>
#include <cstring>
#include <pthread.h>
#include <assert.h>

#define FILE_CACHE_D 1024
#define FILE_CACHE_Z 512
#define FILE_CACHE_X 256
#define FILE_CACHE_ZX 128
#define FILE_CACHE_ZXX 64

using namespace std;

#endif // HEAD_FILE_H