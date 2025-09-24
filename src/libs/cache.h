#ifndef CACHE_H 
#define CACHE_H

#include <stdbool.h>
#include <stddef.h>
#include <sys/stat.h>

// 15 minutes in seconds
#define CACHE_MAX_TIME_SINCE_LAST_FETCH 900

int cache_write(const char* city, const char* json);
int cache_read(const char* city, char** output);
bool cache_time_for_new_data(const char* city);

#endif
