#include "city.h"
#include "HTTP.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    // 1. Create city list and parse predefined cities
    city_list_t* list = city_make_list();
    if (!list) {
        fprintf(stderr, "Failed to create city list\n");
        return 1;
    }
    city_parse(cities, list);
    printf("Parsed %u cities\n", list->size);

    // 2. Let user choose a city
    printf("Enter a city name: ");
    city_node_t* user_city = city_get(list);
    if (!user_city) {
        printf("City not found.\n");
        city_free_list(list);
        return 1;
    }

    printf("You selected: %s\n", user_city->data->name);
    printf("Coordinates: lat=%.2f, lon=%.2f\n", user_city->data->lat, user_city->data->lon);
    printf("Weather API URL: %s\n", user_city->data->url);

    // 3. Fetch weather data via HTTP
    char* http_response = http_get(user_city);
    if (!http_response) {
        fprintf(stderr, "HTTP request failed.\n");
        city_free_list(list);
        return 1;
    }
    printf("\nHTTP response size: %zu bytes\n", strlen(http_response));

    // 4. Parse JSON and update city data
    if (http_json_parse(http_response, user_city) == 0) {
        printf("\nCurrent Weather for %s:\n", user_city->data->name);
        printf("Temperature: %.2f °C\n", user_city->data->temp);
        printf("Wind speed: %.2f m/s\n", user_city->data->windspeed);
        printf("Humidity: %.2f %%\n", user_city->data->rel_hum);
    } else {
        fprintf(stderr, "Failed to parse JSON.\n");
    }

    // 5. Free HTTP response
    free(http_response);

    // 6. Cleanup city list and all nodes/data
    city_free_list(list);

    return 0;
}
