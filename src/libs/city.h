/*
   city.h
   contains:
   const char* cities string with cities
   done: city_list struct
   done: city_node struct
   done: city_data struct

   and functions:
   done: city_make_list()
   done: city_make_node()
   done: city_make_data()
   done: city_add_tail()
   done: city_parse()
   done: city_get()
   done: city_list_free()
   done: city_data_free()
   TO-DO city_remove()
       removes city_node from list
 */

extern char *cities;

#ifndef __CITY_H_
#define __CITY_H_

typedef struct city_data city_data_t;
struct city_data {
    char* name;
    char* url;
    double lat;
    double lon;
    double temp;
    double windspeed;
    double rel_hum;
    
};

typedef struct city_node city_node_t;
struct city_node {
    city_data_t* data;
    city_node_t* prev;
    city_node_t* next;
};

typedef struct city_list city_list_t;
struct city_list {
    city_node_t* head;
    city_node_t* tail;
    unsigned int size; 
};

city_list_t* city_make_list();
city_node_t* city_make_node(city_data_t *city_data);
city_data_t* city_make_data(char *city_name, double lat, double lon, double temp, double windspeed, double rel_hum);
void city_add_tail(city_node_t *city_node, city_list_t* city_list);
void city_parse(char* cities, city_list_t *city_list);
city_node_t* city_get(city_list_t *city_list);
void city_data_free(city_data_t* city_data);
void city_free_list(city_list_t* city_list);

/* NOT NEEDED NOW?
void city_remove(city_list_t *city_list);
*/

#endif /* __CITY_H_ */
