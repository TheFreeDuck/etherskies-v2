#include "cache.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CACHE_MAX_CITY_LEN 256
#define CACHE_FILENAME_SUFFIX "_weather.json"
#define CACHE_MAX_FILENAME_LEN (CACHE_MAX_CITY_LEN + sizeof(CACHE_FILENAME_SUFFIX))

static void create_cache_filename(const char* city, char* output, size_t size) {
    snprintf(output, size, "%s%s", city, CACHE_FILENAME_SUFFIX);
}

// max city len 256
int cache_write(const char* city, const char* json) {
    char filename[CACHE_MAX_FILENAME_LEN];
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

int cache_read(const char* city, char** output) {
    char filename[CACHE_MAX_FILENAME_LEN];
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

static int get_file_modified_time(const char* city, time_t* modTime) {
    char filename[CACHE_MAX_FILENAME_LEN];
    create_cache_filename(city, filename, sizeof(filename));

    struct stat attr;
    if (stat(filename, &attr) != 0) {
        return -1;
    }

    *modTime = attr.st_mtime;

    return 0;
}

bool cache_time_for_new_data(const char* city) {
    time_t current_time = time(NULL);
    time_t mod_time;

    if (get_file_modified_time(city, &mod_time) != 0) {
        // needs new data if file does not exist
        return true;
    }

    return difftime(current_time, mod_time) >= CACHE_MAX_TIME_SINCE_LAST_FETCH;
}
