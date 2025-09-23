#include "file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void create_cache_filename(const char *city, char *output, size_t size){
  const char *suffix = "_weather.json";
  snprintf(output, size, "%s%s", city, suffix);
}

//max city len 256
int write_cache(const char *city, const char *json) {
  char filename[256 + 14];
  create_cache_filename(city, filename, sizeof(filename));
  

  FILE *fptr = fopen(filename, "w");
  if (fptr == NULL) {
    return -1;
  }

  if (fprintf(fptr, "%s", json) < 0) {
    fclose(fptr);
    return -1;
  }

  fclose(fptr);
  return 0;
}

// TODO: will fill up output as much as it can. should be changed
int read_cache(const char *city, char *output, size_t size) {
  char filename[256 + 14];
  create_cache_filename(city, filename, sizeof(filename));

  FILE *fptr = fopen(filename, "r");
  if (fptr == NULL) {
    return -1;
  }

  size_t nread = fread(output, 1, size - 1, fptr);
  output[nread] = '\0';

  fclose(fptr);

  // file prbl empty
  if (nread == 0) {
    return -1;
  }

  return 0;
}

int getFileModifiedTime(const char *city, time_t *modTime) {
  char filename[256 + 14];
  create_cache_filename(city, filename, sizeof(filename));

  struct stat attr;
  if (stat(filename, &attr) != 0) {
    return -1;
  }

  *modTime = attr.st_mtime;

  return 0;
}

bool time_for_new_data(const char *city) {
  time_t current_time = time(NULL);
  time_t mod_time;

  if (getFileModifiedTime(city, &mod_time) != 0) {
    // needs new data if file does not exist
    return true;
  }

  return difftime(current_time, mod_time) >= MAX_TIME_SINCE_LAST_FETCH;
}
