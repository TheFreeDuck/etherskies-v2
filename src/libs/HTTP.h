/*
  includes:
  done: struct for memory buffer
  done: http_get()
  done: http_write_data()
  TODO: http_json_parse()
  TODO_ http_free()
 */

#ifndef __HTTP_H_
#define __HTTP_H_

#include "city.h"
#include "meteo.h"
#include <stdio.h>

/* struct used  as memory buffer */
typedef struct http_membuf http_membuf_t;
struct http_membuf {
    char* data;
    size_t size;
};

char* http_get(city_node_t* city_node);
/* these  func args are standard curl stuff this is our dynamic buffer. */
size_t http_write_data(void* buffer, size_t size, size_t nmemb, void* userp);
int http_json_parse(char* http_response, city_node_t* city_node);

#endif
