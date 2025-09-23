/*
   city.c
   done: city_make_list()
   done: city_make_node()
   done: city_make_data()
   done: city_add_tail()
   done: city_parse()
   done: city_get()
   done: city_node_free()
   done: city_list_free()
*/

#include "city.h"
#include "meteo.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* cities = "Stockholm:59.3293:18.0686\n" "Göteborg:57.7089:11.9746\n" "Malmö:55.6050:13.0038\n" "Uppsala:59.8586:17.6389\n" "Västerås:59.6099:16.5448\n" "Örebro:59.2741:15.2066\n" "Linköping:58.4109:15.6216\n" "Helsingborg:56.0465:12.6945\n" "Jönköping:57.7815:14.1562\n" "Norrköping:58.5877:16.1924\n" "Lund:55.7047:13.1910\n" "Gävle:60.6749:17.1413\n" "Sundsvall:62.3908:17.3069\n" "Umeå:63.8258:20.2630\n" "Luleå:65.5848:22.1567\n" "Kiruna:67.8558:20.2253\n";

/* creates our list */
city_list_t* city_make_list() {
    
    city_list_t* list = (city_list_t *)malloc(sizeof(city_list_t));
    if (!list) {
	printf("Malloc failed in city_make_list\n");
	return NULL;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    
    return list;
}

/* create node for list */
city_node_t* city_make_node(city_data_t *city_data) {
    
    city_node_t* city_node = (city_node_t *)malloc(sizeof(city_node_t));
    if (!city_node) {
	printf("Malloc failed in city_make_node\n");
	return NULL;
    }

    city_node->data = city_data;
    city_node->prev = NULL;
    city_node->next = NULL;

    return city_node;
}

city_data_t* city_make_data(char* city_name, double lat, double lon, double temp, double windspeed, double rel_hum) {

    city_data_t* city_data = (city_data_t *)malloc(sizeof(city_data_t));
    if (!city_data) {
      printf("Malloc failed in city_data_t\n");
      return NULL;
    }

    city_data->lat  = lat;
    city_data->lon = lon;
    city_data->temp = temp;
    city_data->windspeed = windspeed;
    city_data->rel_hum = rel_hum;
    city_data->name = malloc(strlen(city_name) +1);
    if (!city_data->name) {
      free(city_data);
      return NULL;
    }
    strcpy(city_data->name, city_name);
    /* calls meteo_url to craft the url, good solution? */
    city_data->url = meteo_url(lat, lon);

    
    return city_data;
}

void city_add_tail(city_node_t* city_node, city_list_t* city_list) {

  if (city_list->tail == NULL) {
      /* if list empty make city node head and tail*/
    city_list->head = city_node;
    city_list->tail = city_node;
  } else {
    city_node->prev = city_list->tail; /* point backwards to old (current) tail */
    city_list->tail->next = city_node; /* old tail points forward to new node */
    city_list->tail = city_node;       /* new node is now the tail */
  }
  
  city_list->size++;
  
}

void city_parse(char* cities, city_list_t* city_list) {

    if (!cities || !city_list) {
      printf("No string to parse or list to append too in city_parse\n");
      return;
    }
    
    char *str_cpy = malloc(strlen(cities) + 1);
    if (!str_cpy) {
      printf("Malloc failed in city_parse\n");
      return;
    }
    strcpy(str_cpy, cities);

    char* ptr = str_cpy;
    char* name = NULL;
    char* lat_str = NULL;
    char* lon_str = NULL;
    /* char* url = NULL;  we set this in meteo_url via city_data_maker */

    do {
      if (name == NULL) {
	  name = ptr;
      } else if (lat_str == NULL) {
        if (*ptr == ':') {
          lat_str = ptr + 1;
	  *ptr = '\0';
        }
      } else if (lon_str == NULL) {
        if (*ptr == ':') {
          lon_str = ptr + 1;
	  *ptr = '\0';
        }
      } else {
        if (*ptr == '\n') {
          *ptr = '\0';

	  /* send the parsed data to city_make_data */
	  char* end;
          double lat = (double)strtod(lat_str, &end);
          double lon = (double)strtod(lon_str, &end);
          double temp = 0.0;
          double windspeed = 0.0;
	  double rel_hum = 0.0;

	  /* create data for city node and the node itself */
          city_data_t* city_data = city_make_data(name, lat, lon, temp, windspeed, rel_hum);
          /* check malloc */
          if (!city_data) {
            printf("city_maker_data failed but nothing to free\n");
	    continue;		
          }
          city_node_t* city_node = city_make_node(city_data);
          if (!city_node) {
            printf("city_make_node failed freeing data\n");
            free(city_data->name);
            free(city_data);
	    continue;
          }
	  
	  city_add_tail(city_node, city_list);

	  /* reset for next loop */
          name    = NULL;
          lat_str = NULL;
	  lon_str = NULL;
        }
      }

      ptr++;

    } while (*ptr != '\0');
    
    free(str_cpy);
}

city_node_t *city_get(city_list_t *city_list) {

    
    /* get user input */
    char buf[32];
    if (fgets(buf, sizeof(buf), stdin) == NULL) {
	printf("fgets returned NULL inside city_get\n");
	return NULL;
    }
    /* remove trailing new-line if present */
    buf[strcspn(buf, "\n")] = '\0';

    /* look for user input in list */
    city_node_t *current = city_list->head;
    /* check if list is empyy */
    if (current == NULL) {
      printf("List is empty in city_get\n");
      return NULL;
    }

    while (current != NULL) {
      if (strcmp(current->data->name, buf) == 0) {
        return current;
      }
      current = current->next;
    }
    
    return NULL;
}

void city_data_free(city_data_t* city_data) {
    /* if NULL we return */
    if (!city_data) {
	return;
    }

    free(city_data->name);
    free(city_data->url);
    free(city_data);
    /* done */
}

void city_free_list(city_list_t *city_list) {
    /* if NULL we return */
    if (!city_list) {
	return;
    }
    /* go to start */
    city_node_t *current = city_list->head;
    while (current != NULL) {

	/* we need a temp otherwise we'll be lost */
        city_node_t *current_node_next = current->next;

        if (current->data) {
	    /* if theres something here, we clean up */
	    city_data_free(current->data);
        }

        free(current);
	current = current_node_next;
    }

    /* clear to free list */
    free(city_list);
    
}
