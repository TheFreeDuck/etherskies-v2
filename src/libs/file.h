#ifndef FILE_H
#define FILE_H

#include <stdbool.h>
#include <sys/stat.h>

#define CACHE_FILE "weather_cache.json"

// 15 minutes in seconds
#define MAX_TIME_SINCE_LAST_FETCH 900

int write_cache(const char *city, const char *json);
int read_cache(const char *city, char *output, int size);

void getFileCreationTime();
bool time_for_new_data();

#endif
