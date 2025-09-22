#ifndef FILE_H
#define FILE_H

#include <stdbool.h>
#include <sys/stat.h>

#define CACHE_FILE "weather_cache.json"
#define MAX_TIME_SINCE_LAST_FETCH 900

void getFileCreationTime();
bool time_for_new_data();

int write_cache(const char *city,const char *json);
int read_cache(const char *city, char *output, int size);

#endif
