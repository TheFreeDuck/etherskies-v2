#include "file.h"
#include <stdio.h>
#include <time.h>

int write_cache(const char *json) {
  FILE *fptr = fopen(CACHE_FILE, "w");
  if (!fptr) {
    return -1;
  }
  if (fprintf(fptr, "%s", json) < 0) {
    fclose(fptr);
    return -1;
  }
  fclose(fptr);
  return 0;
}

int read_cache(char *output, int size) {
  FILE *fptr = fopen(CACHE_FILE, "r");
  if (!fptr) {
    return -1;
  }

  if (fgets(output, size, fptr) == NULL) {
    fclose(fptr);
    return -1;
  }

  fclose(fptr);
  return 0;
}

int getFileModifiedTime(time_t *modTime) {
  struct stat attr;
  if (stat(CACHE_FILE, &attr) != 0) {
    perror("stat");
    return -1;
  }
  *modTime = attr.st_mtime;
  return 0;
}

bool time_for_new_data() {
  time_t current_time = time(NULL);
  time_t mod_time;
  if (getFileModifiedTime(&mod_time) != 0) {
    return false;
  }
  if (difftime(current_time, mod_time) >= MAX_TIME_SINCE_LAST_FETCH) {
    return true;
  } else {
    return false;
  }
}
