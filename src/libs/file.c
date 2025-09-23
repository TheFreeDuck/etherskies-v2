#include "file.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CITY_LEN 256
#define FILENAME_SUFFIX "_weather.json"
#define MAX_FILENAME_LEN (MAX_CITY_LEN + sizeof(FILENAME_SUFFIX))

void create_cache_filename(const char* city, char* output, size_t size) {
    snprintf(output, size, "%s%s", city, FILENAME_SUFFIX);
}

// max city len 256
int write_cache(const char* city, const char* json) {
    char filename[MAX_FILENAME_LEN];
    create_cache_filename(city, filename, sizeof(filename));

    FILE* fptr = fopen(filename, "w");
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

int read_cache(const char* city, char** output) {
    char filename[MAX_FILENAME_LEN];
    create_cache_filename(city, filename, sizeof(filename));

    struct stat st;
    if (stat(filename, &st) != 0) {
        return -1;
    }
    size_t filesize = st.st_size;

    FILE* fptr = fopen(filename, "rb");
    if (fptr == NULL) {
        return -1;
    }

    char* filedata = malloc(filesize + 1);
    if (!filedata) {
        fclose(fptr);
        return -1;
    }

    size_t nread    = fread(filedata, 1, filesize, fptr);
    filedata[nread] = '\0';

    fclose(fptr);

    // file prbl empty
    if (nread == 0) {
        free(filedata);
        return -1;
    }

    *output = filedata;

    return 0;
}

int getFileModifiedTime(const char* city, time_t* modTime) {
    char filename[MAX_FILENAME_LEN];
    create_cache_filename(city, filename, sizeof(filename));

    struct stat attr;
    if (stat(filename, &attr) != 0) {
        return -1;
    }

    *modTime = attr.st_mtime;

    return 0;
}

bool time_for_new_data(const char* city) {
    time_t current_time = time(NULL);
    time_t mod_time;

    if (getFileModifiedTime(city, &mod_time) != 0) {
        // needs new data if file does not exist
        return true;
    }

    return difftime(current_time, mod_time) >= MAX_TIME_SINCE_LAST_FETCH;
}
